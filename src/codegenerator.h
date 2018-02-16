/*
 *		codegenerator Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_CODEGEN_H
#define NANA_CREATOR_CODEGEN_H

#include "config.h"
#include <vector>
#include <string>
#include "tree.h"
#include "ctrls/ctrl.h"
#include "namemanager.h"



struct code_data_struct
{
	namemanager					hpps;	// necessary headers
	std::vector<std::string>	decl;	// members declarations
	std::vector<std::string>	init;
	std::vector<std::string>	init_post;

	std::string					mainclass;
	std::string					mainclass_ctor;
	std::string					mainclass_base;
	std::string					mainclass_base_ctor;
	std::string					filename;

	void clear()
	{
		hpps.clear();
		decl.clear();
		init.clear();
		init_post.clear();

		mainclass.clear();
		filename = "";
	}
};

struct code_info_struct
{
	std::string create;
	std::string place;

	int field;
};


class codegenerator
{
public:
	codegenerator();

	bool generate(nana::window wd, const std::string& path = "");

	const std::string& getcode() const { return _code; }
	void print(std::basic_ostream<char, std::char_traits<char> >& stream) const;

protected:
	void _reset_tags();
	bool _parse();

	void _write_line(const std::string& line = "", bool endl = true);

	void _append_tag(const std::string& tag);

	bool _write_file();

	void _generate(tree_node<control_struct>* node, const std::string& create, const std::string& place, int field);


	//
	struct tag
	{
		std::string		name;
		size_t			begin;
		size_t			end;
	};
	std::vector<tag>	_tags;		// tags positions
	//
	code_data_struct	_code_data;

	std::string			_buffer;	// file in memory
	std::string			_code;		// generated code to write
	size_t				_indent{ 0 };

	std::string			_filename;
};

#endif //NANA_CREATOR_CODEGEN_H
