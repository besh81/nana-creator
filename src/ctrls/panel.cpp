/*
 *		ctrls::panel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/panel.h"


namespace ctrls
{

	//panel
	panel::panel(ctrl* parent, const std::string& name, bool visible)
		: ctrl(parent)
	{
		pnl.create(*parent->nanawdg, visible);
		boxmodel.bind(pnl);

		ctrl::init(&pnl, CTRL_PANEL, name, false);

		// common
		// ...
		// appearance
		properties.append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check) = pnl.transparent();
		// layout
		properties.remove("weight");
		properties.append("layout").label("Layout").category(CAT_LAYOUT).type(pg_type::layout) = static_cast<int>(layout_orientation::horizontal);
		properties.append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_weight) = -1;
		properties.append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::margin) = "5,5,5,5";
		properties.append("gap").label("Gap").category(CAT_LAYOUT).type(pg_type::string_uint) = 2;
	}


	void panel::update()
	{
		ctrl::update();
		
		pnl.transparent(properties.property("transparent").as_bool());

		boxmodel.set_type(static_cast<layout_orientation>(properties.property("layout").as_int()));
		boxmodel.set_attributes(properties.property("weight").as_string(), properties.property("margin").as_string(), properties.property("gap").as_string());
		boxmodel.update();
	}


	void panel::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);

		auto name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/panel.hpp>");
		cd->hpps.add("#include <nana/gui/place.hpp>");
		// declaration
		cd->decl.push_back("nana::panel<true> " + name + ";"); //TODO: add hasbackground prop
		if(children())
			cd->decl.push_back("nana::place " + name + "_place_;");	
		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");

		if(children())
		{
			cd->init.push_back(name + "_place_.bind(" + name + ");");
			cd->init.push_back(name + "_place_.div(\"" + boxmodel.get(DEFAULT_FIELD, true) + "\");");
		}

		if(!properties.property("enabled").as_bool())
			cd->init.push_back(name + ".enabled(false);");
		generatecode_colors(cd, ci, name);
		if(properties.property("transparent").as_bool())
			cd->init.push_back(name + ".transparent(true);");
		// placement
		cd->init.push_back(ci->place + "[\"" + ci->field + "\"] << " + name + ";");
		// collocate
		if(children())
			cd->init_post.push_back(name + "_place_.collocate();");
		// children
		ci->create = name;
		ci->place = name + "_place_";
		ci->field = DEFAULT_FIELD;
	}

}//end namespace ctrls
