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
		// appearance
		properties.property("bgcolor") = nana::to_string(grp.bgcolor(), false);
		// layout
		properties.remove("weight");
		properties.remove("margin");
		properties.append("layout").label("Layout").category(CAT_LAYOUT).type(pg_type::layout) = static_cast<int>(layout_orientation::horizontal);
		properties.append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties.append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
		properties.append("padding").label("Padding").category(CAT_LAYOUT).type(pg_type::string_uint) = 5;
	}


	void group::update()
	{
		ctrl::update();

		grp.caption(properties.property("caption").as_string());

		boxmodel.orientation(static_cast<layout_orientation>(properties.property("layout").as_int()));
		boxmodel.padding(properties.property("padding").as_int());
		boxmodel.update();

		grp.collocate();
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
		cd->init.push_back(name + ".caption(\"" + properties.property("caption").as_string() + "\");");  //BUG: if positioned after: the group caption is not displayed correctly !!!
		if(children())
			cd->init.push_back(name + ".div(\"" + boxmodel.getdiv() + "\");");

		if(!properties.property("enabled").as_bool())
			cd->init.push_back(name + ".enabled(" + properties.property("enabled").as_string() + ");");
		generatecode_colors(cd, ci, name);
		// placement
		cd->init.push_back(ci->place + "[\"field" + std::to_string(ci->field) + "\"] << " + name + ";");
		// collocate
		if(children())
			cd->init_post.push_back(name + ".collocate();");
		// children
		ci->create = name;
		ci->place = name;
	}


	void group::updatefield(nana::window ctrl, const std::string& weight, const std::string& margin)
	{
		boxmodel.updatefield(ctrl, weight, margin);
	}


	bool group::children()
	{
		return boxmodel.children();
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
