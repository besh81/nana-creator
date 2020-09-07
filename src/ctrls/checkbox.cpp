/*
 *		ctrls::checkbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/checkbox.h"
#include "nana_extra/color_helper.h"


namespace ctrls
{

	//checkbox
	checkbox::checkbox(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		chk.create(*parent->nanawdg);
		ctrl::init(&chk, CTRL_CHECKBOX, name);

		def_scheme = chk.scheme();

		chk.react(false); // needs to avoid user can change the state clicking on control

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = "";
		properties.append("check").label("Checked").category(CAT_COMMON).type(pg_type::check) = chk.checked();
		properties.append("group").label("Group name").category(CAT_COMMON).type(pg_type::string) = "";
		// appearance
		properties.append("radio").label("Radio").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check) = chk.transparent();
		// layout
		// ...
		//scheme
		// def values from checkbox.cpp - function: void drawer::refresh(graph_reference graph)
		properties.append("square_bgcolor").label("Square bg").category(CAT_SCHEME).type(pg_type::color_inherited) = nana::to_string(def_scheme.square_bgcolor.get(chk.bgcolor()), true);
		properties.append("square_border_color").label("Square border").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.square_border_color.get(chk.fgcolor()));
	}


	void checkbox::update()
	{
		ctrl::update();

		chk.caption(properties.property("caption").as_string());
		chk.check(properties.property("check").as_bool());
		chk.radio(properties.property("radio").as_bool());
		chk.transparent(properties.property("transparent").as_bool());

		//scheme
		if(nana::is_color_inherited(properties.property("square_bgcolor").as_string()))
			chk.scheme().square_bgcolor = static_cast<nana::color_argb>(0x0); // def values from checkbox.hpp - drawerbase::checkbox::scheme
		else
			chk.scheme().square_bgcolor = nana::to_color(properties.property("square_bgcolor").as_string());
		chk.scheme().square_border_color = nana::to_color(properties.property("square_border_color").as_string());

		nana::API::refresh_window(chk);
	}


	void checkbox::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/checkbox.hpp>");
		// declaration
		cd->decl.push_back("nana::checkbox " + name + ";");
		// init
		cd->init.push_back(name + ".caption(\"" + properties.property("caption").as_string() + "\");");

		if(!properties.property("group").as_string().empty())
		{
			//TODO: il nome del gruppo non dovrebbe essere presente tra i vari controlli
			std::string group = "rg_" + properties.property("group").as_string();

			// check if the group is been already declared
			bool first_time = true;
			std::string group_decl("nana::radio_group " + group + ";");
			for(auto i : cd->decl)
				if(i == group_decl)
				{
					first_time = false;
					break;
				}
			if(first_time)
				cd->decl.push_back(group_decl);

			cd->init.push_back(group + ".add(" + name + ");");
		}

		if(properties.property("check").as_bool())
			cd->init.push_back(name + ".check(true);");
		if(properties.property("radio").as_bool())
			cd->init.push_back(name + ".radio(true);");
		if(properties.property("transparent").as_bool())
			cd->init.push_back(name + ".transparent(true);");

		//scheme
		if(properties.property("square_bgcolor").as_string() != nana::to_string(def_scheme.square_bgcolor))
			cd->init.push_back(name + ".scheme().square_bgcolor = nana::color(" + properties.property("square_bgcolor").as_string() + ");");
		if(properties.property("square_border_color").as_string() != nana::to_string(def_scheme.square_border_color))
			cd->init.push_back(name + ".scheme().square_border_color = nana::color(" + properties.property("square_border_color").as_string() + ");");
	}

}//end namespace ctrls
