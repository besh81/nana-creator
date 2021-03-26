/*
 *		ctrls::button Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/button.h"
#include "filemanager.h"


extern filemanager		g_file_mgr;


namespace ctrls
{

	//button
	button::button(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		btn.create(*parent->nanawdg);
		ctrl::init(&btn, CTRL_BUTTON, name);

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = CTRL_BUTTON;
		properties.append("icon").label("Icon").category(CAT_COMMON).type(pg_type::image) = "";
		// appearance
		properties.append("enable_pushed").label("Enable pushed").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("pushed").label("Pushed").category(CAT_APPEARANCE).type(pg_type::check).enabled("enable_pushed", true) = false;
		properties.append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check) = btn.transparent();
		// layout
		// ...
	}


	void button::update()
	{
		ctrl::update();

		btn.caption(properties.property("caption").as_string());
		btn.icon(nana::paint::image(properties.property("icon").as_string()));
		btn.enable_pushed(properties.property("enable_pushed").as_bool());
		if(properties.property("enable_pushed").as_bool())
			btn.pushed(properties.property("pushed").as_bool());
		else
			btn.pushed(false);
		btn.transparent(properties.property("transparent").as_bool());
	}


	void button::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/button.hpp>");
		// declaration
		cd->decl.push_back("nana::button " + name + ";");
		// init
		cd->init.push_back(name + ".caption(\"" + properties.property("caption").as_escaped_string() + "\");");
		if(!properties.property("icon").as_string().empty())
			cd->init.push_back(name + ".icon(nana::paint::image(\"" + g_file_mgr.to_relative(properties.property("icon").as_string()) + "\"));");
		if(properties.property("enable_pushed").as_bool())
		{
			cd->init.push_back(name + ".enable_pushed(true);");
			if(properties.property("pushed").as_bool())
				cd->init.push_back(name + ".pushed(true);");
		}
		if(properties.property("transparent").as_bool())
			cd->init.push_back(name + ".transparent(true);");
	}

}//end namespace ctrls
