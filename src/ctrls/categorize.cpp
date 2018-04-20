/*
 *		ctrls::categorize Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/categorize.h"
#include "nana_extra/color_helper.h"


namespace ctrls
{

	//categorize
	categorize::categorize(nana::window wd, const std::string& name)
		: ctrl()
	{
		ctg.create(wd);
		ctrl::init(&ctg, CTRL_CATEGORIZE, name);

		// common
		//properties.append("categories").label("Categories").category(CAT_COMMON).type(pg_type::collection_categorize) = "";
		properties.append("splitter").label("Splitter").category(CAT_COMMON).type(pg_type::string) = ctg.splitstr();
		// appearance
		properties.property("bgcolor") = nana::to_string(ctg.bgcolor(), false);
		// layout
		// ...
	}


	void categorize::update()
	{
		ctrl::update();

		ctg.splitstr(properties.property("splitter").as_string());
	}


	void categorize::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/categorize.hpp>");
		// declaration
		cd->decl.push_back("nana::categorize<int> " + name + ";");
		// init
		// ...
	}

}//end namespace ctrls
