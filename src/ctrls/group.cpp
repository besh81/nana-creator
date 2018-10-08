/*
 *		ctrls::group Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/group.h"
#include "nana_extra/color_helper.h"


namespace ctrls
{

	//group
	group::group(nana::window wd, const std::string& name)
		: ctrl()
	{
		grp.create(wd);
		grp.caption(CTRL_GROUP); //BUG: if removed the group caption is not displayed at the beginning !!!
		boxmodel.bind(&grp);

		ctrl::init(&grp, CTRL_GROUP, name);

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = CTRL_GROUP;
		properties.append("format").label("Format").category(CAT_COMMON).type(pg_type::check) = false;
		// appearance
		properties.property("bgcolor") = nana::to_string(grp.bgcolor(), false);
		properties.append("halign").label("Caption Alignment").category(CAT_APPEARANCE).type(pg_type::halign) = static_cast<int>(nana::align::left);
		// layout
		properties.remove("weight");
		properties.append("layout").label("Layout").category(CAT_LAYOUT).type(pg_type::layout) = static_cast<int>(layout_orientation::horizontal);
		properties.append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_weight) = -1;
		properties.append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 5;
	}


	void group::update()
	{
		ctrl::update();

		// format and caption
		if(properties.property("format").as_bool())
		{
			try
			{
				grp.caption(properties.property("caption").as_string());
				grp.enable_format_caption(true);
			}
			catch(...)
			{
				nana::msgbox m(0, CREATOR_NAME, nana::msgbox::ok);
				m.icon(m.icon_warning);
				m << "Errore formato!"; //TODO
				m();

				grp.enable_format_caption(false);
				grp.caption(properties.property("caption").as_string());
			}
		}
		else
		{
			grp.enable_format_caption(false);
			grp.caption(properties.property("caption").as_string());
		}


		grp.caption_align(static_cast<nana::align>(properties.property("halign").as_int()));


		boxmodel.set(static_cast<layout_orientation>(properties.property("layout").as_int()), "",
			properties.property("margin").as_string());
		boxmodel.update();

		grp.collocate();

		// needed to correctly redraw the group control when the caption change
		nana::API::refresh_window(grp);
	}


	void group::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/group.hpp>");
		// declaration
		cd->decl.push_back("nana::group " + name + ";");
		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");
		
		if(properties.property("format").as_bool())
			cd->init.push_back(name + ".enable_format_caption(" + properties.property("format").as_string() + ");");
		if(properties.property("format").as_string() != "left")
			cd->init.push_back(name + ".caption_align(static_cast<nana::align>(" + properties.property("halign").as_string() + "));");
		if(children())
			cd->init.push_back(name + ".div(\"" + boxmodel.get(DEFAULT_FIELD, true) + "\");");

		cd->init.push_back(name + ".caption(\"" + properties.property("caption").as_string() + "\");");

		if(!properties.property("enabled").as_bool())
			cd->init.push_back(name + ".enabled(" + properties.property("enabled").as_string() + ");");
		generatecode_colors(cd, ci, name);
		// placement
		cd->init.push_back(ci->place + "[\"" + ci->field + "\"] << " + name + ";");
		// collocate
		if(children())
			cd->init_post.push_back(name + ".collocate();");
		// children
		ci->create = name;
		ci->place = name;
		ci->field = DEFAULT_FIELD;
	}


	void group::update_children_info(nana::window child, const std::string& divtext, const std::string& weight)
	{
		boxmodel.update_children_info(child, divtext, weight);
	}


	bool group::children()
	{
		return boxmodel.children();
	}


	bool group::children_fields()
	{
		return boxmodel.children_fields();
	}


	bool group::append(nana::window ctrl)
	{
		return boxmodel.append(ctrl);
	}


	bool group::insert(nana::window pos, nana::window ctrl, bool after)
	{
		return boxmodel.insert(pos, ctrl, after);
	}


	bool group::remove(nana::window ctrl)
	{
		return boxmodel.remove(ctrl);
	}


	bool group::moveup(nana::window ctrl)
	{
		return boxmodel.moveup(ctrl);
	}


	bool group::movedown(nana::window ctrl)
	{
		return boxmodel.movedown(ctrl);
	}

}//end namespace ctrls
