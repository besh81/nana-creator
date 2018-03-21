/*
 *		ctrls::toolbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/toolbar.h"
#include "filemanager.h"
#include "tokenizer/Tokenizer.h"


extern filemanager		g_file_mgr;


namespace ctrls
{

	//toolbar
	toolbar::toolbar(nana::window wd, const std::string& name)
		: ctrl()
	{
		tlb.create(wd);
		ctrl::init(&tlb, CTRL_TOOLBAR, name);

		// common
		properties.append("buttons").label("Buttons").category(CAT_COMMON).type(pg_type::collection_toolbar) = "";
		// appearance
		properties.append("scale").label("Scale").category(CAT_APPEARANCE).type(pg_type::string_uint) = 16; // get from nana source
		// layout
		// ...
	}


	void toolbar::update()
	{
		ctrl::update();

		tlb.scale(properties.property("scale").as_uint());

		// buttons - START
		tlb.clear();
		// split buttons into item (delimiter = CITEM_TKN)
		Tokenizer items_tkn(properties.property("buttons").as_string());
		items_tkn.setDelimiter(CITEM_TKN);

		std::string item;
		while((item = items_tkn.next()) != "")
		{
			if(item == CITEM_TLB_SEPARATOR)
				tlb.separate();
			else
			{
				// split item into properties (delimiter = CITEM_INNER_TKN)
				Tokenizer item_tkn(item);
				item_tkn.setDelimiter(CITEM_INNER_TKN);

				auto text = item_tkn.next();
				auto img = item_tkn.next();

				if(img == "")
					tlb.append(text);
				else
					tlb.append(text, nana::paint::image(g_file_mgr.to_relative(img)));
			}
		}
		// buttons - END
	}


	void toolbar::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/toolbar.hpp>");
		// declaration
		cd->decl.push_back("nana::toolbar " + name + ";");
		// init
		cd->init.push_back(name + ".scale(" + properties.property("scale").as_string() + ");");

		// buttons - START
		// split buttons into item (delimiter = CITEM_TKN)
		Tokenizer items_tkn(properties.property("buttons").as_string());
		items_tkn.setDelimiter(CITEM_TKN);

		std::string item;
		while((item = items_tkn.next()) != "")
		{
			std::string ln;
			if(item == CITEM_TLB_SEPARATOR)
				ln = name + ".separate();";
			else
			{
				// split item into properties (delimiter = CITEM_INNER_TKN)
				Tokenizer item_tkn(item);
				item_tkn.setDelimiter(CITEM_INNER_TKN);

				ln = name + ".append(\"" + item_tkn.next() + "\"";
				auto img = item_tkn.next();
				if(img == "")
					ln.append(");");
				else
					ln.append(", nana::paint::image(\"" + g_file_mgr.to_relative(img) + "\"));");
			}
			cd->init.push_back(ln);
		}
		// buttons - END
	}

}//end namespace ctrls
