/*
 *		ctrls::tabbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/tabbar.h"
#include "nana_extra/color_helper.h"
#include "tokenizer/Tokenizer.h"
#include "filemanager.h"


extern filemanager		g_file_mgr;


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
		properties.append("addbtn").label("Add button").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("closebtn").label("Close button").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("closefly").label("Close fly").category(CAT_APPEARANCE).type(pg_type::check).enabled("closebtn", true) = false;
		properties.append("listbtn").label("List button").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("scrollbtn").label("Scroll button").category(CAT_APPEARANCE).type(pg_type::check) = true;
		// layout
		// ...


		// events
		//----------
		// avoid use of add button
		tbb.events().adding([this](const nana::arg_tabbar_adding<size_t> & arg)
		{
			if(!internal_use)
			{
				arg.add = false;
				arg.stop_propagation();
			}
		});
		// avoid use of close button
		tbb.events().removed([this](const nana::arg_tabbar_removed<size_t> & arg)
		{
			if(!internal_use)
			{
				arg.remove = false;
				arg.stop_propagation();
			}
		});
	}


	void tabbar::update()
	{
		ctrl::update();

		// tabs - START
		internal_use = true;
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

			auto img = item_tkn.next();
			if(img != CITEM_EMPTY)
				tbb.tab_image(tbb.length()-1, nana::paint::image(img));

			bool inherited;
			auto bgcolor_txt = item_tkn.next();
			auto bgcolor = nana::to_color(bgcolor_txt, inherited);
			if(!inherited)
				tbb.tab_bgcolor(tbb.length()-1, bgcolor);

			auto fgcolor_txt = item_tkn.next();
			auto fgcolor = nana::to_color(fgcolor_txt, inherited);
			if(!inherited)
				tbb.tab_fgcolor(tbb.length()-1, fgcolor);
		}
		internal_use = false;
		// tabs - END

		tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::add, properties.property("addbtn").as_bool());
		tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::close, properties.property("closebtn").as_bool());
		tbb.close_fly(properties.property("closefly").as_bool());
		tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::list, properties.property("listbtn").as_bool());
		tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::scroll, properties.property("scrollbtn").as_bool());
	}


	void tabbar::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/tabbar.hpp>");
		// declaration
		cd->decl.push_back("nana::tabbar<size_t> " + name + ";");
		// init

		// tabs - START
		// split columns into item (delimiter = CITEM_TKN)
		Tokenizer items_tkn(properties.property("tabs").as_string());
		items_tkn.setDelimiter(CITEM_TKN);

		int i = 0;
		std::string item;
		while((item = items_tkn.next()) != "")
		{
			// split item into properties (delimiter = CITEM_INNER_TKN)
			Tokenizer item_tkn(item);
			item_tkn.setDelimiter(CITEM_INNER_TKN);

			cd->init.push_back(name + ".push_back(\"" + item_tkn.next() + "\");");

			auto img = item_tkn.next();
			if(img != CITEM_EMPTY)
				cd->init.push_back(name + ".tab_image(" + std::to_string(i) + ", nana::paint::image(\"" + g_file_mgr.to_relative(img) + "\"));");

			bool inherited;
			auto bgcolor_txt = item_tkn.next();
			auto bgcolor = nana::to_color(bgcolor_txt, inherited);
			if(!inherited)
				cd->init.push_back(name + ".tab_bgcolor(" + std::to_string(i) + ", nana::color(" + bgcolor_txt + "));");

			auto fgcolor_txt = item_tkn.next();
			auto fgcolor = nana::to_color(fgcolor_txt, inherited);
			if(!inherited)
				cd->init.push_back(name + ".tab_fgcolor(" + std::to_string(i) + ", nana::color(" + fgcolor_txt + "));");

			i++;
		}
		// tabs - END

		if(properties.property("addbtn").as_bool())
			cd->init.push_back(name + ".toolbox(nana::drawerbase::tabbar::trigger::kits::add, true);");
		if(properties.property("closebtn").as_bool())
		{
			cd->init.push_back(name + ".toolbox(nana::drawerbase::tabbar::trigger::kits::close, true);");
			if(properties.property("closefly").as_bool())
				cd->init.push_back(name + ".close_fly(true);");
		}
		if(properties.property("listbtn").as_bool())
			cd->init.push_back(name + ".toolbox(nana::drawerbase::tabbar::trigger::kits::list, true);");
		if(!properties.property("scrollbtn").as_bool()) // Only scrolling button is enabled by default
			cd->init.push_back(name + ".toolbox(nana::drawerbase::tabbar::trigger::kits::scroll, false);");
	}

}//end namespace ctrls
