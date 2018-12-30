/*
 *		ctrls::listbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/listbox.h"
#include "nana_extra/color_helper.h"


namespace ctrls
{

	//listbox
	listbox::listbox(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		lst.create(*parent->nanawdg);
		ctrl::init(&lst, CTRL_LISTBOX, name);

		// common
		properties.append("columns").label("Columns").category(CAT_COMMON).type(pg_type::collection_listbox) = "";
		properties.append("checkable").label("Checkable").category(CAT_COMMON).type(pg_type::check) = false;
		properties.append("single_selection").label("Single selection").category(CAT_COMMON).type(pg_type::check) = false;
		properties.append("sortable").label("Sortable").category(CAT_COMMON).type(pg_type::check) = lst.sortable();
		// appearance
		properties.append("show_header").label("Show header").category(CAT_APPEARANCE).type(pg_type::check) = lst.visible_header();
		// layout
		// ...
	}


	void listbox::init_item(properties_collection& item)
	{
		ctrl::init_item(item);
		item.property("type") = "column";
		//
		item.append("text").label("Text").category(CAT_COMMON).type(pg_type::string) = "New Item";
		item.append("width").label("Width").category(CAT_COMMON).type(pg_type::string_int) = -1;
	}


	void listbox::update()
	{
		ctrl::update();

		// columns - START
		lst.clear_headers();

		for(auto& i : items)
		{
			if(i.property("width").as_int() < 0)
				lst.append_header(i.property("text").as_string());
			else
				lst.append_header(i.property("text").as_string(), i.property("width").as_uint());
		}
		// columns - END

		lst.sortable(properties.property("sortable").as_bool());
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
		if(properties.property("checkable").as_bool())
			cd->init.push_back(name + ".checkable(true);");
		if(!properties.property("sortable").as_bool())
			cd->init.push_back(name + ".sortable(false);");
		if(properties.property("single_selection").as_bool())
			cd->init.push_back(name + ".enable_single(true, true);");
		if(!properties.property("show_header").as_bool())
			cd->init.push_back(name + ".show_header(false);");

		// columns - START
		for(auto& i : items)
		{
			std::string str = name + ".append_header(\"" + i.property("text").as_string() + "\"";

			if(i.property("width").as_int() < 0)
				str.append(");");
			else
				str.append(", " + i.property("width").as_string() + ");");

			cd->init.push_back(str);
		}
		// columns - END
	}

}//end namespace ctrls
