/*
 *		ctrls::combox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/combox.h"
#include "tokenizer/Tokenizer.h"


namespace ctrls
{

	//combox
	combox::combox(nana::window wd, const std::string& name)
		: ctrl()
	{
		cmb.create(wd);
		ctrl::init(&cmb, CTRL_COMBOX, name);

		// common
		properties.append("options").label("Options").category(CAT_COMMON).type(pg_type::collection) = "";
		properties.append("editable").label("Editable").category(CAT_COMMON).type(pg_type::check) = cmb.editable();
		// appearance
		// ...
		// layout
		// ...
	}


	void combox::update()
	{
		ctrl::update();

		cmb.editable(properties.property("editable").as_bool());
		/* not usefull in the gui creation process
		// options
		cmb.clear();
		std::string options = properties.property("options").as_string();
		if(!options.empty())
		{
			// options: string with this format  ->  "item1" "item2" "item3" ...
			Tokenizer strtkn(options);
			strtkn.setDelimiter("\"");
			std::string item;
			while((item = strtkn.next()) != "")
			{
				if(item != " ")
					cmb.push_back(item);
			}
		}
		*/
	}


	void combox::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/combox.hpp>");
		// declaration
		cd->decl.push_back("nana::combox " + name + ";");
		// init
		cd->init.push_back(name + ".editable(" + properties.property("editable").as_string() + ");");

		std::string options = properties.property("options").as_string();
		if(!options.empty())
		{
			// options: string with this format  ->  "item1" "item2" "item3" ...
			Tokenizer strtkn(options);
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
