/*
 *		ctrls::slider Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/slider.h"


namespace ctrls
{

	//slider
	slider::slider(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		sld.create(*parent->nanawdg);
		ctrl::init(&sld, CTRL_SLIDER, name);

		// common
		properties.append("value").label("Value").category(CAT_COMMON).type(pg_type::string_uint) = sld.value();
		properties.append("maximum").label("Maximum").category(CAT_COMMON).type(pg_type::string_uint) = sld.maximum();
		properties.append("seek").label("Seek").category(CAT_COMMON).type(pg_type::seekdir) = static_cast<int>(nana::drawerbase::slider::seekdir::bilateral);
		// appearance
		properties.append("vertical").label("Vertical").category(CAT_APPEARANCE).type(pg_type::check) = sld.vertical();
		properties.append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check) = sld.transparent();
		// layout
		// ...
	}


	void slider::update()
	{
		ctrl::update();

		sld.maximum(properties.property("maximum").as_uint());
		sld.value(properties.property("value").as_uint());
		sld.seek(static_cast<nana::drawerbase::slider::seekdir>(properties.property("seek").as_int()));

		sld.vertical(properties.property("vertical").as_bool());
		sld.transparent(properties.property("transparent").as_bool());
	}


	void slider::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/slider.hpp>");
		// declaration
		cd->decl.push_back("nana::slider " + name + ";");
		// init
		cd->init.push_back(name + ".maximum(" + properties.property("maximum").as_string() + ");");
		cd->init.push_back(name + ".value(" + properties.property("value").as_string() + ");");
		cd->init.push_back(name + ".seek(static_cast<nana::drawerbase::slider::seekdir>(" + properties.property("seek").as_string() + "));");
		cd->init.push_back(name + ".vertical(" + properties.property("vertical").as_string() + ");");
		cd->init.push_back(name + ".transparent(" + properties.property("transparent").as_string() + ");");
	}

}//end namespace ctrls
