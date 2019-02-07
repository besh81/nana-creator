/*
 *		ctrls::picture Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/picture.h"
#include "filemanager.h"
#include "nana_extra/color_helper.h"


extern filemanager		g_file_mgr;


namespace ctrls
{

	//picture
	picture::picture(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		pct.create(*parent->nanawdg);
		ctrl::init(&pct, CTRL_PICTURE, name, false);

		// common
		properties.append("image").label("Image").category(CAT_COMMON).type(pg_type::image) = "";
		// appearance
		properties.append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check) = pct.transparent();
		properties.append("halign").label("Horizontal alignment").category(CAT_APPEARANCE).type(pg_type::halign) = static_cast<int>(nana::align::left);
		properties.append("valign").label("Vertical alignment").category(CAT_APPEARANCE).type(pg_type::valign) = static_cast<int>(nana::align_v::top);
		properties.append("stretchable").label("Stretchable").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("gradualbg").label("Gradual background").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("gbg_from").label("From color").category(CAT_APPEARANCE).type(pg_type::color).enabled("gradualbg", true) = nana::to_string(nana::colors::white);
		properties.append("gbg_to").label("To color").category(CAT_APPEARANCE).type(pg_type::color).enabled("gradualbg", true) = nana::to_string(nana::colors::black);
		properties.append("gbg_dir").label("Direction").category(CAT_APPEARANCE).type(pg_type::layout).enabled("gradualbg", true) = 0;
		// layout
		// ...
	}


	void picture::update()
	{
		ctrl::update();

		pct.load(nana::paint::image(properties.property("image").as_string()));
		pct.transparent(properties.property("transparent").as_bool());
		pct.align(static_cast<nana::align>(properties.property("halign").as_int()), static_cast<nana::align_v>(properties.property("valign").as_int()));
		pct.stretchable(properties.property("stretchable").as_bool());
		if(properties.property("gradualbg").as_bool())
		{
			auto from_col = nana::to_color(properties.property("gbg_from").as_string());
			auto to_col = nana::to_color(properties.property("gbg_to").as_string());
			pct.set_gradual_background(from_col, to_col, !properties.property("gbg_dir").as_bool());
		}
		else
			pct.set_gradual_background(pct.bgcolor(), pct.bgcolor(), true);
	}


	void picture::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/picture.hpp>");
		// declaration
		cd->decl.push_back("nana::picture " + name + ";");
		// init
		if(!properties.property("image").as_string().empty())
			cd->init.push_back(name + ".load(nana::paint::image(\"" + g_file_mgr.to_relative(properties.property("image").as_string()) + "\"));");
		if(properties.property("transparent").as_bool())
			cd->init.push_back(name + ".transparent(true);");
		cd->init.push_back(name + ".align(static_cast<nana::align>(" + properties.property("halign").as_string() + "), static_cast<nana::align_v>(" + properties.property("valign").as_string() + "));");
		if(properties.property("stretchable").as_bool())
			cd->init.push_back(name + ".stretchable(true);");
		if(properties.property("gradualbg").as_bool())
		{
			auto from_col = properties.property("gbg_from").as_string();
			auto to_col = properties.property("gbg_to").as_string();
			auto dir = !properties.property("gbg_dir").as_bool() ? "true" : "false";
			cd->init.push_back(name + ".set_gradual_background(nana::color(" + from_col + "), nana::color(" + to_col + "), " + dir + ");");
		}
	}

}//end namespace ctrls
