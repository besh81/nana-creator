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
		properties.append("options").label("Options").category(CAT_COMMON).type(pg_type::collection_combox) = "";
		properties.append("option").label("Option").category(CAT_COMMON).type(pg_type::string_uint) = cmb.option();
		properties.append("editable").label("Editable").category(CAT_COMMON).type(pg_type::check) = cmb.editable();
		// appearance
		// ...
		// layout
		// ...
	}


	void combox::update()
	{
		ctrl::update();

		//options: I don't think it's usefull to add options here

		cmb.editable(properties.property("editable").as_bool());
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
		
		// options - START
		// split options into item (delimiter = CITEM_TKN)
		Tokenizer items_tkn(properties.property("options").as_string());
		items_tkn.setDelimiter(CITEM_TKN);

		std::string item;
		while((item = items_tkn.next()) != "")
		{
			// split item into properties (delimiter = CITEM_INNER_TKN)
			Tokenizer item_tkn(item);
			item_tkn.setDelimiter(CITEM_INNER_TKN);

			cd->init.push_back(name + ".push_back(\"" + item_tkn.next() + "\");");

			//TODO aggiungere immagine
		}
		// options - END

		cd->init.push_back(name + ".option(" + properties.property("option").as_string() + ");");
		cd->init.push_back(name + ".editable(" + properties.property("editable").as_string() + ");");
	}

}//end namespace ctrls
