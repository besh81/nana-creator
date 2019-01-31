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

		def_scheme = lst.scheme();

		// common
		properties.append("columns").label("Columns").category(CAT_COMMON).type(pg_type::collection_listbox) = "";
		properties.append("checkable").label("Checkable").category(CAT_COMMON).type(pg_type::check) = false;
		properties.append("single_selection").label("Single selection").category(CAT_COMMON).type(pg_type::check) = false;
		properties.append("sortable").label("Sortable").category(CAT_COMMON).type(pg_type::check) = lst.sortable();
		// appearance
		properties.append("show_header").label("Show header").category(CAT_APPEARANCE).type(pg_type::check) = lst.visible_header();
		// layout
		// ...
		//scheme
		properties.append("header_bgcolor").label("Header bg").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.header_bgcolor);
		properties.append("header_fgcolor").label("Header fg").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.header_fgcolor);
		properties.append("header_grabbed").label("Header grabbed").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.header_grabbed);
		properties.append("header_floated").label("Header floated").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.header_floated);
		properties.append("item_selected").label("Item selected").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.item_selected);
		properties.append("item_highlighted").label("Item highlighted").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.item_highlighted);
		properties.append("selection_box").label("Selection box").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.selection_box);
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

		//scheme
		lst.scheme().header_bgcolor = nana::to_color(properties.property("header_bgcolor").as_string());
		lst.scheme().header_fgcolor = nana::to_color(properties.property("header_fgcolor").as_string());
		lst.scheme().header_grabbed = nana::to_color(properties.property("header_grabbed").as_string());
		lst.scheme().header_floated = nana::to_color(properties.property("header_floated").as_string());
		lst.scheme().item_selected = nana::to_color(properties.property("item_selected").as_string());
		lst.scheme().item_highlighted = nana::to_color(properties.property("item_highlighted").as_string());
		lst.scheme().selection_box = nana::to_color(properties.property("selection_box").as_string());
		nana::API::refresh_window(lst);
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

		//scheme
		if(properties.property("header_bgcolor").as_string() != nana::to_string(def_scheme.header_bgcolor))
			cd->init.push_back(name + ".scheme().header_bgcolor = nana::color(" + properties.property("header_bgcolor").as_string() + ");");
		if(properties.property("header_fgcolor").as_string() != nana::to_string(def_scheme.header_fgcolor))
			cd->init.push_back(name + ".scheme().header_fgcolor = nana::color(" + properties.property("header_fgcolor").as_string() + ");");
		if(properties.property("header_grabbed").as_string() != nana::to_string(def_scheme.header_grabbed))
			cd->init.push_back(name + ".scheme().header_grabbed = nana::color(" + properties.property("header_grabbed").as_string() + ");");
		if(properties.property("header_floated").as_string() != nana::to_string(def_scheme.header_floated))
			cd->init.push_back(name + ".scheme().header_floated = nana::color(" + properties.property("header_floated").as_string() + ");");
		if(properties.property("item_selected").as_string() != nana::to_string(def_scheme.item_selected))
			cd->init.push_back(name + ".scheme().item_selected = nana::color(" + properties.property("item_selected").as_string() + ");");
		if(properties.property("item_highlighted").as_string() != nana::to_string(def_scheme.item_highlighted))
			cd->init.push_back(name + ".scheme().item_highlighted = nana::color(" + properties.property("item_highlighted").as_string() + ");");
		if(properties.property("selection_box").as_string() != nana::to_string(def_scheme.selection_box))
			cd->init.push_back(name + ".scheme().selection_box = nana::color(" + properties.property("selection_box").as_string() + ");");

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
