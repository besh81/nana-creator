/*
 *		codegenerator_data Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_CODEGEN_DATA_H
#define NANA_CREATOR_CODEGEN_DATA_H

#include "config.h"
#include <vector>
#include <string>
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
	std::string field;
};


#endif //NANA_CREATOR_CODEGEN_DATA_H
