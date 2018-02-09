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
#include "wdir.h"
////////////////////////////////
#include "ctrls/layout.h"
#include "ctrls/panel.h"
#include "ctrls/button.h"
#include "ctrls/label.h"
#include "ctrls/textbox.h"
#include "ctrls/combox.h"
#include "ctrls/spinbox.h"
#include "ctrls/listbox.h"
#include "ctrls/checkbox.h"
#include "ctrls/date_chooser.h"
#include "ctrls/toolbar.h"
#include "ctrls/form.h"


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


bool codegenerator::generate(nana::window wd, const std::string& path)
{
	bool create_file = false;

	// generate code from controls
	_code_data.clear();
	_generate(g_gui_mgr._ctrls.get_root()->child, "*this", "", 1);


	_filename = path;
	if(_filename.empty())
	{
		// use name file in panel - mainclass
		_filename = GetCurrentWorkingDir() + "\\" + _code_data.filename + ".h";
	}

	// check file
	struct stat buf;
	if(stat(_filename.c_str(), &buf) == -1)
	{
		// file doesn't exist -> create a new one
		create_file = true;
	}
	else
	{
		// file exist -> check
		std::fstream in_file(_filename.c_str(), std::ios::out | std::ios::in);

		in_file.seekg(0, std::ios::end);
		_buffer.clear();
		_buffer.reserve(static_cast<size_t>(in_file.tellg()));
		in_file.seekg(0, std::ios::beg);
		_buffer.assign((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());
		in_file.close();

		if(!_parse())
		{
			nana::msgbox m(wd, CREATOR_NAME, nana::msgbox::yes_no);
			m.icon(m.icon_question);
			m << "Existing file corrupted!";
			m << "Overwrite the file?";
			if(m() == nana::msgbox::pick_yes)
			{
				create_file = true;
			}
			else
				return false;
		}
	}

	if(create_file)
		_write_new_file();
	else
		_write_existing_file();

	// write buffer into file
	std::ofstream out_file(_filename.c_str());
	out_file << _buffer;

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


void codegenerator::_write_headers_tag()
{
	_code.clear();

	for(auto& i : _tags)
		if(i.name == HEADERS_TAG)
		{
			_indent = i.indent;

			_write_line();
			std::vector<std::string> hpps = _code_data.hpps.getlist();
			for(auto it = hpps.begin(); it != hpps.end(); ++it)
				_write_line(*it);
			_write_line("", false);

			_buffer.replace(i.begin, i.end - i.begin, _code);
			break;
		}
}

void codegenerator::_write_init_tag()
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

void codegenerator::_write_initfunc_tag()
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
			for(auto it = _code_data.init.begin(); it != _code_data.init.end(); ++it)
				_write_line(*it);
			_write_line();
			for(auto it = _code_data.init_post.begin(); it != _code_data.init_post.end(); ++it)
				_write_line(*it);
			--_indent;
			_write_line("}");
			_write_line("", false);

			_buffer.replace(i.begin, i.end - i.begin, _code);
			break;
		}
}

void codegenerator::_write_declarations_tag()
{
	_code.clear();

	for(auto& i : _tags)
		if(i.name == DECLARATIONS_TAG)
		{
			_indent = i.indent;

			_write_line();
			for(auto it = _code_data.decl.begin(); it != _code_data.decl.end(); ++it)
				_write_line(*it);
			_write_line("", false);

			_buffer.replace(i.begin, i.end - i.begin, _code);
			break;
		}
}


void codegenerator::_generate(tree_node<control_struct>* node, const std::string& create, const std::string& place, int field)
{
	if(!node)
		return;

	auto attr = &node->value->properties;

	code_info_struct ci;
	ci.create = create;
	ci.place = place;
	ci.field = field;

	auto ctrl = node->value;

	auto type = ctrl->properties.property("type").as_string();
	if(type == CTRL_LAYOUT)
	{
		static_cast<ctrls::layout*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}
	else if(type == CTRL_PANEL)
	{
		static_cast<ctrls::panel*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}
	else if(type == CTRL_BUTTON)
	{
		static_cast<ctrls::button*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}
	else if(type == CTRL_LABEL)
	{
		static_cast<ctrls::label*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}
	else if(type == CTRL_TEXTBOX)
	{
		static_cast<ctrls::textbox*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}
	else if(type == CTRL_COMBOX)
	{
		static_cast<ctrls::combox*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}
	else if(type == CTRL_SPINBOX)
	{
		static_cast<ctrls::spinbox*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}
	else if(type == CTRL_LISTBOX)
	{
		static_cast<ctrls::listbox*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}
	else if(type == CTRL_CHECKBOX)
	{
		static_cast<ctrls::checkbox*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}
	else if(type == CTRL_DATECHOOSER)
	{
		static_cast<ctrls::date_chooser*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}
	else if(type == CTRL_TOOLBAR)
	{
		static_cast<ctrls::toolbar*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}
	else if(type == CTRL_FORM)
	{
		static_cast<ctrls::form*>(ctrl->nanawdg.get())->generatecode(&ctrl->properties, &_code_data, &ci);
	}

	// children
	_generate(node->child, ci.create, ci.place, 1);
	// siblings
	_generate(node->next, create, place, field + 1);
}


bool codegenerator::_write_new_file()
{
	_code.clear();
	_indent = 0;

	//---- file header - START
	const char* header =
		"/*****************************************************\n"
		" *\tC++ code generated with " CREATOR_NAME " (" CREATOR_VERSION ")\n"
		" *\tGitHub repo:\n"
		"*****************************************************/\n";
	_write_line(header);
	//---- file header - END

	_write_line();

	//---- c++ headers - START
	_write_line(BEGIN_TAG HEADERS_TAG);
	std::vector<std::string> hpps = _code_data.hpps.getlist();
	for(auto it = hpps.begin(); it != hpps.end(); ++it)
		_write_line(*it);
	_write_line(END_TAG);
	//---- c++ headers - END

	_write_line();

	//---- class declaration - START
	_write_line("class " + _code_data.mainclass);
	_indent++;
	_write_line(": public " + _code_data.mainclass_base);
	_indent--;
	_write_line("{");

	//---- public - START
	_write_line("public:");
	_indent++;

	//---- ctor - START
	_write_line(_code_data.mainclass + _code_data.mainclass_ctor);
	_indent++;
	_write_line(": " + _code_data.mainclass_base + _code_data.mainclass_base_ctor);
	_indent--;
	_write_line("{");
	_indent++;

	_write_line(BEGIN_TAG INIT_TAG);
	_write_line("init_();");
	_write_line(END_TAG);

	_indent--;
	_write_line("}");
	//---- ctor - END

	//---- public - END

	_write_line();

	//---- private - START
	_indent--;
	_write_line();
	_write_line("private:");
	_indent++;


	_write_line(BEGIN_TAG INITFUNC_TAG);
	_write_line("void init_()");
	_write_line("{");
	_indent++;

	for(auto it = _code_data.init.begin(); it != _code_data.init.end(); ++it)
		_write_line(*it);
	_write_line();
	for(auto it = _code_data.init_post.begin(); it != _code_data.init_post.end(); ++it)
		_write_line(*it);

	_indent--;
	_write_line("}");
	_write_line(END_TAG);
	//---- private - END

	_write_line();

	//---- protected - START
	_indent--;
	_write_line();
	_write_line("protected:");
	_indent++;


	_write_line(BEGIN_TAG DECLARATIONS_TAG);

	for(auto it = _code_data.decl.begin(); it != _code_data.decl.end(); ++it)
		_write_line(*it);

	_write_line(END_TAG);
	//---- protected - END


	_indent--;
	_write_line("};");
	_write_line();

	_buffer = _code;

	return true;
}


bool codegenerator::_write_existing_file()
{
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
			_write_headers_tag();
		else if(_tags[imax].name == INIT_TAG)
			_write_init_tag();
		else if(_tags[imax].name == INITFUNC_TAG)
			_write_initfunc_tag();
		else if(_tags[imax].name == DECLARATIONS_TAG)
			_write_declarations_tag();

		// set as done
		_tags[imax].begin = 0;
	}

	return true;
}
