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
#include "codegenerator_data.h"
#include "ctrls/ctrl.h"



class codegenerator
{
public:
	codegenerator();

	bool generate(nana::window wd, tree_node<control_obj>* node, const std::string& path = "");

	const std::string& getcode() const { return _code; }
	void print(std::basic_ostream<char, std::char_traits<char> >& stream) const;

protected:
	void _reset_tags();
	bool _parse();

	void _write_line(const std::string& line = "", bool endl = true);

	void _append_tag(const std::string& tag);

	bool _write_file();

	void _generate(tree_node<control_obj>* node, const std::string& create, const std::string& place, const std::string& field);


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
