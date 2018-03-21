/*
 *		ctrls::listbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/listbox.h"
#include "tokenizer/Tokenizer.h"
#include "nana_extra/color_helper.h"


namespace ctrls
{

	//listbox
	listbox::listbox(nana::window wd, const std::string& name)
		: ctrl()
	{
		lst.create(wd);
		ctrl::init(&lst, CTRL_LISTBOX, name);

		// common
		properties.append("columns").label("Columns").category(CAT_COMMON).type(pg_type::collection_combox) = "";
		properties.append("checkable").label("Checkable").category(CAT_COMMON).type(pg_type::check) = false;
		properties.append("single_selection").label("Single selection").category(CAT_COMMON).type(pg_type::check) = false;
		// appearance
		properties.property("bgcolor") = nana::to_string(lst.bgcolor(), false);
		properties.append("show_header").label("Show header").category(CAT_APPEARANCE).type(pg_type::check) = lst.visible_header();
		// layout
		// ...
	}


	void listbox::update()
	{
		ctrl::update();

		// columns
		lst.clear_headers();
		std::string options = properties.property("columns").as_string();
		if(!options.empty())
		{
			// columns: string with this format  ->  "item1" "item2" "item3" ...
			Tokenizer strtkn(options);
			strtkn.setDelimiter("\"");
			std::string item;
			while((item = strtkn.next()) != "")
			{
				if(item != " ")
					lst.append_header(item);
			}
		}

		lst.show_header(properties.property("show_header").as_bool());
	}


	void listbox::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/listbox.hpp>");
		// declaration
		cd->decl.push_back("nana::listbox " + name + ";");
		// init
		cd->init.push_back(name + ".checkable(" + properties.property("checkable").as_string() + ");");
		if(properties.property("single_selection").as_bool())
			cd->init.push_back(name + ".enable_single(true, true);");
		cd->init.push_back(name + ".show_header(" + properties.property("show_header").as_string() + ");");

		// columns
		std::string options = properties.property("columns").as_string();
		if(!options.empty())
		{
			// columns: string with this format  ->  "item1" "item2" "item3" ...
			Tokenizer strtkn(options);
			strtkn.setDelimiter("\"");
			std::string item;
			while((item = strtkn.next()) != "")
			{
				if(item != " ")
					cd->init.push_back(name + ".append_header(\"" + item + "\");");
			}
		}
	}

}//end namespace ctrls
