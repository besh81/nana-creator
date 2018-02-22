/*
 *		ctrls::picture Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/picture.h"


namespace ctrls
{

	//picture
	picture::picture(nana::window wd, const std::string& name)
		: ctrl()
	{
		pct.create(wd);
		ctrl::init(&pct, CTRL_PICTURE, name);

		// common
		// ...
		// appearance
		properties.append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check) = pct.transparent();
		properties.append("halign").label("Horizontal Alignment").category(CAT_APPEARANCE).type(pg_type::choice).type_hints(
			std::vector<std::string>{ CITEM_LEFT, CITEM_CENTER, CITEM_RIGHT }) = static_cast<int>(nana::align::left);
		properties.append("valign").label("Vertical Alignment").category(CAT_APPEARANCE).type(pg_type::choice).type_hints(
			std::vector<std::string>{ CITEM_TOP, CITEM_CENTER, CITEM_BOTTOM }) = static_cast<int>(nana::align_v::top);
		properties.append("stretchable").label("Stretchable").category(CAT_APPEARANCE).type(pg_type::check) = false;
		// layout
		// ...
	}


	void picture::update()
	{
		ctrl::update();

		pct.transparent(properties.property("transparent").as_bool());
		pct.align(static_cast<nana::align>(properties.property("halign").as_int()), static_cast<nana::align_v>(properties.property("valign").as_int()));
		pct.stretchable(properties.property("stretchable").as_bool());
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
		cd->init.push_back(name + ".transparent(" + properties.property("transparent").as_string() + ");");
		cd->init.push_back(name + ".align(static_cast<nana::align>(" + properties.property("halign").as_string() + "), static_cast<nana::align_v>(" + properties.property("valign").as_string() + "));");
		cd->init.push_back(name + ".stretchable(" + properties.property("stretchable").as_string() + ");");
	}

}//end namespace ctrls
