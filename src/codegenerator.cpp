/*
 *		codegenerator Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include <fstream>
#include <streambuf>
#include <cstring>
#include "codegenerator.h"
#include "guimanager.h"
#include "filemanager.h"
#include "ctrls/ctrl.h"


#define BEGIN_TAG			"//<*"
#define END_TAG				"//*>"

#define INFO_TAG			"tag"
#define INCLUDES_TAG		"includes"
#define CTOR_TAG			"ctor"
#define DTOR_TAG			"dtor"
#define DECLARATIONS_TAG	"declarations"


extern guimanager	g_gui_mgr;


codegenerator::codegenerator()
{
	_reset_tags();
}


bool codegenerator::generate(nana::window wd, tree_node<control_obj>* node, const std::string& path)
{
	bool create_file = false;

	// generate code from controls
	_code_data.clear();
	_generate(node, "*this", "", DEFAULT_FIELD);


	if(path.empty())
	{
		// use current working dir
		_filename = get_working_dir() + "\\" + _code_data.filename + ".h";
	}
	else
	{
		_filename = path + "\\" + _code_data.filename + ".h";
	}

	// check file
	if(!file_exists(_filename))
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
		_reset_tags();
	_write_file();

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

	_tags.push_back(tag{ INFO_TAG, 0, 0 });
	_tags.push_back(tag{ INCLUDES_TAG, 0, 0 });
	_tags.push_back(tag{ CTOR_TAG, 0, 0 });
	_tags.push_back(tag{ DTOR_TAG, 0, 0 });
	_tags.push_back(tag{ DECLARATIONS_TAG, 0, 0 });
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


void codegenerator::_append_tag(const std::string& tag)
{
	for(auto& i : _tags)
		if(i.name == tag)
		{
			if(i.begin == i.end)
			{
				_write_line(BEGIN_TAG + tag);
				_write_line();
				_write_line(END_TAG);
			}
			else
			{
				_write_line(BEGIN_TAG + tag, false);
				_code.append(_buffer.substr(i.begin, i.end - i.begin));
				_code.append(END_TAG "\n");
			}
			break;
		}
}


void codegenerator::_generate(tree_node<control_obj>* node, const std::string& create, const std::string& place, const std::string& field)
{
	if(!node)
		return;

	code_info_struct ci;
	ci.create = create;
	ci.place = place;
	ci.field = field;

	node->value->generatecode(&_code_data, &ci);
	
	// children
	_generate(node->child, ci.create, ci.place, ci.field);
	// siblings
	_generate(node->next, create, place, field);
}


bool codegenerator::_write_file()
{
	_code.clear();
	_indent = 0;

	std::string def_str = _code_data.filename;
	std::transform(def_str.begin(), def_str.end(), def_str.begin(), ::toupper);
	def_str.append("_H");


	//---- file header - START
	const char* header =
		"/*****************************************************\n"
		" *\tC++ code generated with " CREATOR_NAME " (" CREATOR_VERSION ")\n"
		" *\tGitHub repo: https://github.com/besh81/nana-creator\n"
		" *\n"
		" * PLEASE EDIT ONLY INSIDE THE TAGS:\n"
		" *		//<*" INFO_TAG "\n"
		" *			user code\n"
		" *		//*>\n"
		"*****************************************************/\n";
	_write_line(header);

	_write_line("#ifndef " + def_str);
	_write_line("#define " + def_str);


	//---- file header - END

	_write_line();

	//---- c++ headers - START
	std::vector<std::string> hpps = _code_data.hpps.getlist();
	for(auto it = hpps.begin(); it != hpps.end(); ++it)
		_write_line(*it);
	//---- c++ headers - END

	_write_line();
	_append_tag(INCLUDES_TAG);
	_write_line();
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

	_write_line("init_();");

	_write_line();
	_append_tag(CTOR_TAG);

	_indent--;
	_write_line("}");
	//---- ctor - END

	_write_line();

	//---- dtor - START
	_write_line("~" + _code_data.mainclass + "()");
	_write_line("{");
	_indent++;

	_append_tag(DTOR_TAG);

	_indent--;
	_write_line("}");
	//---- dtor - END
	//---- public - END

	_write_line();

	//---- private - START
	_indent--;
	_write_line();
	_write_line("private:");
	_indent++;

	//---- init function - START
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
	//---- init function - END
	//---- private - END

	_write_line();

	//---- protected - START
	_indent--;
	_write_line();
	_write_line("protected:");
	_indent++;

	//---- ctrls declarations - STAT
	for(auto it = _code_data.decl.begin(); it != _code_data.decl.end(); ++it)
		_write_line(*it);
	//---- ctrls declarations - END
	//---- protected - END

	_write_line();

	_write_line();
	_append_tag(DECLARATIONS_TAG);

	_indent--;
	_write_line("};");
	_write_line();

	_write_line("#endif //" + def_str);
	_write_line();
	
	_buffer = _code;

	return true;
}
