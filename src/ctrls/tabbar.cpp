/*
 *		ctrls::tabbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/tabbar.h"
#include "nana_extra/color_helper.h"
#include "filemanager.h"
#include "style.h"


extern filemanager		g_file_mgr;


namespace ctrls
{

	//tabbar
	tabbar::tabbar(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		tbb.create(*parent->nanawdg);
		ctrl::init(&tbb, CTRL_TABBAR, name);

		// common
		properties.append("tabs").label("Tabs").category(CAT_COMMON).type(pg_type::collection_tabbar) = "";
		properties.append("activated").label("Activated").category(CAT_COMMON).type(pg_type::string_uint) = 0;
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


	void tabbar::init_item(properties_collection& item)
	{
		ctrl::init_item(item);
		item.property("type") = "tab";
		//
		item.append("text").label("Text").category(CAT_COMMON).type(pg_type::string) = "New Tab";
		item.append("image").label("Image").category(CAT_COMMON).type(pg_type::image) = "";

		item.append("bgcolor").label("Background").category(CAT_COMMON).type(pg_type::color_inherited) = nana::to_string(ITEMS_DEF_BGCOL);
		item.append("fgcolor").label("Foreground").category(CAT_COMMON).type(pg_type::color_inherited) = nana::to_string(ITEMS_DEF_FGCOL);
	}


	void tabbar::update()
	{
		ctrl::update();

		// tabs - START
		internal_use = true;
		while(tbb.length())
			tbb.erase(0);

		items.for_each([this](tree_node<ctrls::properties_collection>* node) -> bool
			{
				tbb.push_back(node->value.property("text").as_string());

				if(!node->value.property("image").as_string().empty())
					tbb.tab_image(tbb.length() - 1, nana::paint::image(node->value.property("image").as_string()));

				bool inherited;
				auto bgcolor = nana::to_color(node->value.property("bgcolor").as_string(), inherited);
				if(!inherited)
					tbb.tab_bgcolor(tbb.length() - 1, bgcolor);

				auto fgcolor = nana::to_color(node->value.property("fgcolor").as_string(), inherited);
				if(!inherited)
					tbb.tab_fgcolor(tbb.length() - 1, fgcolor);
				return true;
			});

		internal_use = false;
		// tabs - END

		if(properties.property("activated").as_uint() < tbb.length())
			tbb.activated(properties.property("activated").as_uint());
		else if(tbb.length())
			tbb.activated(0);

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
		items.for_each([cd, &name](tree_node<ctrls::properties_collection>* node) -> bool
			{
				cd->init.push_back(name + ".push_back(\"" + node->value.property("text").as_escaped_string() + "\");");

				if(!node->value.property("image").as_string().empty())
					cd->init.push_back(name + ".tab_image(" + std::to_string(node->pos()) + ", nana::paint::image(\"" + g_file_mgr.to_relative(node->value.property("image").as_string()) + "\"));");

				auto bgcolor_txt = node->value.property("bgcolor").as_string();
				if(!nana::is_color_inherited(bgcolor_txt))
					cd->init.push_back(name + ".tab_bgcolor(" + std::to_string(node->pos()) + ", nana::color(" + bgcolor_txt + "));");

				auto fgcolor_txt = node->value.property("fgcolor").as_string();
				if(!nana::is_color_inherited(fgcolor_txt))
					cd->init.push_back(name + ".tab_fgcolor(" + std::to_string(node->pos()) + ", nana::color(" + fgcolor_txt + "));");
				return true;
			});
		// tabs - END

		if(properties.property("activated").as_uint() < tbb.length())
			cd->init.push_back(name + ".activated(" + properties.property("activated").as_string() + ");");
		else if(tbb.length())
			cd->init.push_back(name + ".activated(0);");

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
