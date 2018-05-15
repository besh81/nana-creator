/*
 *		ctrls::tabbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/tabbar.h"
#include "tokenizer/Tokenizer.h"


namespace ctrls
{

	//tabbar
	tabbar::tabbar(nana::window wd, const std::string& name)
		: ctrl()
	{
		tbb.create(wd);
		ctrl::init(&tbb, CTRL_TABBAR, name);

		// common
		properties.append("tabs").label("Tabs").category(CAT_COMMON).type(pg_type::collection_tabbar) = "";
		// appearance
		// ...
		// layout
		// ...
	}


	void tabbar::update()
	{
		ctrl::update();

		// tabs - START
		while(tbb.length())
			tbb.erase(0);
		// split columns into item (delimiter = CITEM_TKN)
		Tokenizer items_tkn(properties.property("tabs").as_string());
		items_tkn.setDelimiter(CITEM_TKN);

		std::string item;
		while((item = items_tkn.next()) != "")
		{
			// split item into properties (delimiter = CITEM_INNER_TKN)
			Tokenizer item_tkn(item);
			item_tkn.setDelimiter(CITEM_INNER_TKN);

			auto text = item_tkn.next();

			tbb.push_back(text);
		}
		// tabs - END
	}


	void tabbar::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/tabbar.hpp>");
		// declaration
		cd->decl.push_back("nana::tabbar " + name + ";");
		// init

		// tabs - START
		// split columns into item (delimiter = CITEM_TKN)
		Tokenizer items_tkn(properties.property("tabs").as_string());
		items_tkn.setDelimiter(CITEM_TKN);

		std::string item;
		while((item = items_tkn.next()) != "")
		{
			// split item into properties (delimiter = CITEM_INNER_TKN)
			Tokenizer item_tkn(item);
			item_tkn.setDelimiter(CITEM_INNER_TKN);

			cd->init.push_back(name + ".push_back(\"" + item_tkn.next() + "\");");
		}
		// tabs - END
	}

}//end namespace ctrls
