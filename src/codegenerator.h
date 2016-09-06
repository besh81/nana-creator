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



struct code_struct
{
	std::vector<std::string> hpps;
	std::vector<std::string> decl;
	std::vector<std::string> init;
	std::vector<std::string> init_post;

	std::string create;
	std::string place;

	int field;
};


class codegenerator
{
public:
	codegenerator();

	bool load_file(const std::string& path);
	bool save_file(const std::string& path);

	bool generate();
	bool generateOLD();
	const std::string& getcode() const { return _code; }

	void print(std::basic_ostream<char, std::char_traits<char> >& stream) const;

protected:
	void _reset_tags();
	bool _parse();

	void _write_line(const std::string& line = "", bool endl = true);
	void _write_file_header();
	
	void _write_headers();
	void _write_init();
	void _write_initfunc();
	void _write_declarations();

	void _generate(tree_node<control_struct>* node, const std::string& create, const std::string& place, int field);


	std::string			_buffer;			// files in memory
	//
	struct tag
	{
		std::string		name;
		size_t			indent;
		size_t			begin;
		size_t			end;
	};
	std::vector<tag>	_tags;				// tags positions
	//
	namemanager					_headers;		// necessary headers
	std::vector<std::string>	_declarations;	// members declarations
	std::vector<std::string>	_initfunc;
	std::vector<std::string>	_initfunc_post;

	std::string			_code;
	size_t				_indent{ 0 };
	

	struct widget
	{
		std::string		name;
		std::string		type;
		std::string		owner;
		std::string		xxx;
	};

	widget				_mainwidget;
	std::vector<widget>	_widgets;
};

#endif //NANA_CREATOR_CODEGEN_H
