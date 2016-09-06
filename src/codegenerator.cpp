/*
 *		codegenerator Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include <fstream>
#include <streambuf>
#include "codegenerator.h"
#include "guimanager.h"
////////////////////////////////
#include "ctrls/layout.h"
#include "ctrls/panel.h"
#include "ctrls/button.h"
#include "ctrls/label.h"
#include "ctrls/textbox.h"


#define BEGIN_TAG			"//<*"
#define END_TAG				"//*>"

#define HEADERS_TAG			"Headers()"
#define INIT_TAG			"Initialize()"
#define INITFUNC_TAG		"InitFunction()"
#define DECLARATIONS_TAG	"Declarations()"


extern guimanager	g_gui_mgr;


codegenerator::codegenerator()
{
	_reset_tags();
}


bool codegenerator::load_file(const std::string& path)
{
	_buffer.clear();

	struct stat buf;
	if(stat(path.c_str(), &buf) == -1)
		return false;

	std::ifstream f(path.c_str());

	f.seekg(0, std::ios::end);
	_buffer.reserve(static_cast<size_t>(f.tellg()));
	f.seekg(0, std::ios::beg);

	_buffer.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

	if(!_parse())
	{
		std::cout << "Parsing error\n";
		return false;
	}

	return true;
}

bool codegenerator::save_file(const std::string& path)
{
	std::ofstream t(path.c_str());

	t << _buffer;

	return true;
}


bool codegenerator::generate()
{
	_headers.clear();
	_declarations.clear();
	_initfunc.clear();
	
	_generate(g_gui_mgr._ctrls.get_root()->child, "*this", "", 1);


	// replace code between tags
	_indent = 0;
	while(1)
	{
		size_t imax = 0;
		size_t vmax = 0;
		for(size_t i = 0; i < _tags.size(); ++i)
		{
			if(_tags[i].begin > vmax)
			{
				vmax = _tags[i].begin;
				imax = i;
			}
		}

		if(vmax == 0)
			break;

		if(_tags[imax].name == HEADERS_TAG)
			_write_headers();
		else if(_tags[imax].name == INIT_TAG)
			_write_init();
		else if(_tags[imax].name == INITFUNC_TAG)
			_write_initfunc();
		else if(_tags[imax].name == DECLARATIONS_TAG)
			_write_declarations();

		// set as done
		_tags[imax].begin = 0;
	}
	
	return true;
}

void codegenerator::print(std::basic_ostream<char, std::char_traits<char> >& stream) const
{
	stream << "codegenerator:" << std::endl;
	stream << _buffer << std::endl;
}


void codegenerator::_reset_tags()
{
	_tags.clear();

	_tags.push_back(tag{ HEADERS_TAG, 0, 0, 0 });
	_tags.push_back(tag{ INIT_TAG, 0, 0, 0 });
	_tags.push_back(tag{ INITFUNC_TAG, 0, 0, 0 });
	_tags.push_back(tag{ DECLARATIONS_TAG, 0, 0, 0 });
}


bool codegenerator::_parse()
{
	_reset_tags();

	if(_buffer.empty())
		return false;

	std::size_t begin = 0;
	while(1)
	{
		// look for begin tag
		begin = _buffer.find(BEGIN_TAG, begin);
		if(begin == std::string::npos)
			break;

		begin += std::strlen(BEGIN_TAG);

		bool found = false;
		size_t i = 0;
		for(; i < _tags.size(); ++i)
		{
			auto pos = _buffer.find(_tags[i].name, begin);
			if(pos == begin)
			{
				if(_tags[i].begin != 0)
				{
					// error: duplicated tag
					_reset_tags();
					return false;
				}
				else
				{
					_tags[i].begin = pos + std::strlen(_tags[i].name.c_str());
					found = true;
					break;
				}
			}
		}

		if(!found)
		{
			// error: unknown tag
			_reset_tags();
			return false;
		}

		_tags[i].end = _buffer.find(END_TAG, begin);
		if(_tags[i].end == std::string::npos)
		{
			// error: missing end tag
			_reset_tags();
			return false;
		}


		// indent
		if(_buffer[_tags[i].begin] == '\n')
		{
			_tags[i].indent = 0;
			while(_buffer[_tags[i].begin + 1 + _tags[i].indent] == '\t')
				++_tags[i].indent;
		}


		begin = _tags[i].end;
	}

	// check
	for(auto i : _tags)
		if(i.begin == 0)
		{
			// error: missing tag
			return false;
		}

	return true;
}


void codegenerator::_write_line(const std::string& line, bool endl)
{
	std::string ln;
	if(line.empty() && endl)
	{
		ln = "\n";
	}
	else
	{
		// indent
		for(size_t i = _indent; i; --i)
			ln.append("\t");
		// line
		ln.append(line);
		// new line
		if(endl)
			ln.append("\n");
	}

	_code.append(ln);
}


void codegenerator::_write_file_header()
{
	const char* header =
		"/*************************************************\n"
		" *\tC++ code generated with " CREATOR_NAME " (" CREATOR_VERSION ")\n"
		" *\tGitHub repo:\n"
		"*************************************************/\n";

	_write_line(header);
}


void codegenerator::_write_headers()
{
	_code.clear();

	for(auto& i : _tags)
		if(i.name == HEADERS_TAG)
		{
			_indent = i.indent;

			_write_line();
			std::vector<std::string> hpps = _headers.getlist();
			for(auto it = hpps.begin(); it != hpps.end(); ++it)
				_write_line(*it);
			_write_line("", false);

			_buffer.replace(i.begin, i.end - i.begin, _code);
			break;
		}
}

void codegenerator::_write_init()
{
	_code.clear();

	for(auto& i : _tags)
		if(i.name == INIT_TAG)
		{
			_indent = i.indent;

			_write_line();
			_write_line("init_();");
			_write_line("", false);

			_buffer.replace(i.begin, i.end - i.begin, _code);
			break;
		}
}

void codegenerator::_write_initfunc()
{
	_code.clear();

	for(auto& i : _tags)
		if(i.name == INITFUNC_TAG)
		{
			_indent = i.indent;

			_write_line();
			_write_line("void init_()");
			_write_line("{");
			++_indent;
			for(auto it = _initfunc.begin(); it != _initfunc.end(); ++it)
				_write_line(*it);
			_write_line();
			for(auto it = _initfunc_post.begin(); it != _initfunc_post.end(); ++it)
				_write_line(*it);
			--_indent;
			_write_line("}");
			_write_line("", false);

			_buffer.replace(i.begin, i.end - i.begin, _code);
			break;
		}
}

void codegenerator::_write_declarations()
{
	_code.clear();

	for(auto& i : _tags)
		if(i.name == DECLARATIONS_TAG)
		{
			_indent = i.indent;

			_write_line();
			for(auto it = _declarations.begin(); it != _declarations.end(); ++it)
				_write_line(*it);
			_write_line("", false);

			_buffer.replace(i.begin, i.end - i.begin, _code);
			break;
		}
}


bool codegenerator::generateOLD()
{
	_headers.clear();
	_indent = 0;

	_generate(g_gui_mgr._ctrls.get_root()->child, "*this", "", 1);


	//---- file header - START
	_write_file_header();
	//---- file header - END

	_write_line();

	//---- c++ headers - START
	std::vector<std::string> hpps = _headers.getlist();
	for(auto it = hpps.begin(); it != hpps.end(); ++it)
		_write_line("#include <" + *it + ">");
	//---- c++ headers - END

	_write_line();
	_write_line();

	//---- class declaration - START
	_write_line("class " + _mainwidget.name);
	_indent++;
	_write_line(": public " CTRL_NAMESPACE + _mainwidget.type);
	_indent--;
	_write_line("{");


	// public
	_write_line("public:");
	_indent++;


	//---- ctor - START
	_write_line(_mainwidget.name + "(nana::window wd, bool visible = true)");
	_indent++;
	_write_line(": " CTRL_NAMESPACE + _mainwidget.type + "(wd, visible)");
	_indent--;
	_write_line("{");
	_indent++;

	//---- widgets creation - START
	for(auto it = _widgets.begin(); it != _widgets.end(); ++it)
	{
		if(it->type != "place")
		{
			if(it->owner == _mainwidget.name)
				_write_line(it->name + ".create(*this);");
			else
				_write_line(it->name + ".create(" + it->owner + ");");
		}
	}
	//---- widgets creation - END

	_write_line();

	//---- place binding - START
	for(auto it = _widgets.begin(); it != _widgets.end(); ++it)
	{
		if(it->type == "place")
		{
			if(it->owner == _mainwidget.name)
				_write_line(it->name + ".bind(*this);");
			else
				_write_line(it->name + ".bind(" + it->owner + ");");
			_write_line(it->name + ".div(\"" + it->xxx + "\");");
		}
	}
	//---- place binding - END

	_write_line();

	//---- widgets placement - START
	for(auto it = _widgets.begin(); it != _widgets.end(); ++it)
	{
		if(it->type != "place")
			_write_line(it->xxx + ".field(\"abc\") << " + it->name + ";");
	}
	//---- widgets placement - END

	_write_line();

	//---- place collocate - START
	for(auto it = _widgets.rbegin(); it != _widgets.rend(); ++it)
	{
		if(it->type == "place")
			_write_line(it->name + ".collocate();");
		
	}
	//---- place collocate - END

	_indent--;
	_write_line("};");
	//---- ctor - END

	_write_line();
	_write_line();

	//---- widgets declaration - START
	for(auto it = _widgets.begin(); it != _widgets.end(); ++it)
	{
		_write_line(CTRL_NAMESPACE + it->type + " " + it->name + ";");
	}
	//---- widgets declaration - END

	_indent--;
	_write_line("};");
	//---- class declaration - END

	return true;
}


void codegenerator::_generate(tree_node<control_struct>* node, const std::string& create, const std::string& place, int field)
{
	if(!node)
		return;

	auto attr = &node->value->properties;

	code_struct cc;
	cc.create = create;
	cc.place = place;
	cc.field = field;

	auto ctrl = node->value;

	auto type = ctrl->properties.property("type").as_string();
	if(type == CTRL_LAYOUT)
	{
		static_cast<ctrls::layout*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &cc);
	}
	else if(type == CTRL_PANEL)
	{
		static_cast<ctrls::panel*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &cc);
	}
	else if(type == CTRL_BUTTON)
	{
		static_cast<ctrls::button*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &cc);
	}
	else if(type == CTRL_LABEL)
	{
		static_cast<ctrls::label*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &cc);
	}
	else if(type == CTRL_TEXTBOX)
	{
		static_cast<ctrls::textbox*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &cc);
	}


	for(auto i : cc.hpps)
		_headers.add(i);
	for(auto i : cc.decl)
		_declarations.push_back(i);
	for(auto i : cc.init)
		_initfunc.push_back(i);
	for(auto i : cc.init_post)
		_initfunc_post.push_back(i);

	// children
	_generate(node->child, cc.create, cc.place, 1);
	// siblings
	_generate(node->next, create, place, field + 1);
}


/*void codegenerator::writecode(std::fstream& stream)
{
	// load content in memory
	std::string file_str;

	stream.seekg(0, std::ios::end);
	file_str.reserve(stream.tellg());
	stream.seekg(0, std::ios::beg);

	//file_str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());


	// look for tags
	// replace code inside tags
	// write file

	//stream << "Work in progress ..." << std::endl;
}
*/
