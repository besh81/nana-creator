/*
 *		ctrls::menubar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/menubar.h"
#include "tokenizer/Tokenizer.h"


namespace ctrls
{

	//menubar
	menubar::menubar(nana::window wd, const std::string& name)
		: ctrl()
	{
		mnb.create(wd);
		ctrl::init(&mnb, CTRL_MENUBAR, name);

		// common
		properties.append("menus").label("Menus").category(CAT_COMMON).type(pg_type::collection) = "";
		// appearance
		// ...
		// layout
		// ...
	}


	void menubar::update()
	{
		ctrl::update();

		// menus
#ifdef NANA_1_5_6_FORK
		mnb.clear(); // added in my fork -> not present in 1.5.6
#endif
		std::string menus = properties.property("menus").as_string();
		if(!menus.empty())
		{
			// menus: string with this format  ->  "item1" "item2" "item3" ...
			Tokenizer strtkn(menus);
			strtkn.setDelimiter("\"");
			std::string item;
			while((item = strtkn.next()) != "")
			{
				if(item != " ")
					mnb.push_back(item);
			}
		}
	}


	void menubar::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/menubar.hpp>");
		// declaration
		cd->decl.push_back("nana::menubar " + name + ";");
		// init
		std::string menus = properties.property("menus").as_string();
		if(!menus.empty())
		{
			// menus: string with this format  ->  "item1" "item2" "item3" ...
			Tokenizer strtkn(menus);
			strtkn.setDelimiter("\"");
			std::string item;
			while((item = strtkn.next()) != "")
			{
				if(item != " ")
					cd->init.push_back(name + ".push_back(\"" + item + "\");");
			}
		}
	}

}//end namespace ctrls
