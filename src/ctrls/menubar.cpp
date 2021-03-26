/*
 *		ctrls::menubar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/menubar.h"
#include "filemanager.h"


extern filemanager		g_file_mgr;


namespace ctrls
{

	//menubar
	menubar::menubar(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		mnb.create(*parent->nanawdg);
		ctrl::init(&mnb, CTRL_MENUBAR, name);

		// common
		properties.append("menus").label("Menus").category(CAT_COMMON).type(pg_type::collection_menubar) = "";
		// appearance
		// ...
		// layout
		// ...
	}


	void menubar::init_item(properties_collection& item, const std::string& type)
	{
		ctrl::init_item(item);
		item.property("type") = type;

		if(type == "separator")
			return;

		item.append("text").label("Text").category(CAT_COMMON).type(pg_type::string) = "New Menu";

		if(type == "menu")
		{
			item.append("image").label("Image").category(CAT_COMMON).type(pg_type::image) = "";
			item.append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = true;
			item.append("check_style").label("Check style").category(CAT_COMMON).type(pg_type::check_style) = static_cast<int>(nana::menu::checks::none);
			item.append("checked").label("Checked").category(CAT_COMMON).type(pg_type::check) = false;
		}
	}


	void menubar::update()
	{
		ctrl::update();

		// menus - START
		mnb.clear();
		create_menu_tree();

		// Create menus
		//--------------------------------
		auto pmnb = &mnb;
		menu_tree.for_each([&pmnb](tree_node<menu_data>* node) -> bool
			{
				auto* pval = &node->value;

				if(node->level() == 0)
				{
					//this is a menubar item
					if(node->child)
						pval->submenu = &(pmnb->push_back(pval->item->property("text").as_string()));
					else
						pmnb->push_back(pval->item->property("text").as_string());
				}
				else
				{
					//this is a menu item
					auto* pval_parent = &node->owner->value;

					if(pval->item->property("type").as_string() == "separator")
					{
						pval_parent->submenu->append_splitter();
						return true;
					}

					pval_parent->submenu->append(pval->item->property("text").as_string());

					if(!pval->item->property("image").as_string().empty())
						pval_parent->submenu->image(pval_parent->submenu->size()-1, nana::paint::image(g_file_mgr.to_relative(pval->item->property("image").as_string())));

					pval_parent->submenu->enabled(pval_parent->submenu->size() - 1, pval->item->property("enabled").as_bool() ? true : false);
					pval_parent->submenu->check_style(pval_parent->submenu->size() - 1, static_cast<nana::menu::checks>(pval->item->property("check_style").as_int()));
					pval_parent->submenu->checked(pval_parent->submenu->size() - 1, pval->item->property("checked").as_bool() ? true : false);

					if(node->child)
						pval->submenu = pval_parent->submenu->create_sub_menu(pval_parent->submenu->size()-1);
				}

				return true;
			});
		// menus - END
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

		// menus - START
		//create_menu_tree(); //already created on update()

		// Generate menus code
		//--------------------------------
		menu_tree.for_each([&cd, &name](tree_node<menu_data>* node) -> bool
		{
			auto* pval = &node->value;

			if(node->level() == 0)
			{
				//this is a menubar item
				if(node->child)
				{
					pval->submenu_name = name + "_" + std::to_string(node->pos()) + "_submenu_";
					cd->init.push_back("auto* " + pval->submenu_name + " = &" + name + ".push_back(\"" + pval->item->property("text").as_escaped_string() + "\");");
				}
				else
					cd->init.push_back(name + ".push_back(\"" + pval->item->property("text").as_escaped_string() + "\");");
			}
			else
			{
				//this is a menu item
				auto* pval_parent = &node->owner->value;

				if(pval->item->property("type").as_string() == "separator")
				{
					cd->init.push_back(pval_parent->submenu_name + "->append_splitter();");
					return true;
				}

				cd->init.push_back(pval_parent->submenu_name + "->append(\"" + pval->item->property("text").as_escaped_string() + "\");");

				if(!pval->item->property("image").as_string().empty())
					cd->init.push_back(pval_parent->submenu_name + "->image(" + std::to_string(node->pos()) + ", nana::paint::image(\"" + g_file_mgr.to_relative(pval->item->property("image").as_string()) + "\"));");

				if(!pval->item->property("enabled").as_bool())
					cd->init.push_back(pval_parent->submenu_name + "->enabled(" + std::to_string(node->pos()) + ", false);");
				if(pval->item->property("check_style").as_int() != static_cast<int>(nana::menu::checks::none))
					cd->init.push_back(pval_parent->submenu_name + "->check_style(" + std::to_string(node->pos()) + ", static_cast<nana::menu::checks>(" + pval->item->property("check_style").as_string() + "));");
				if(pval->item->property("checked").as_bool())
					cd->init.push_back(pval_parent->submenu_name + "->checked(" + std::to_string(node->pos()) + ", true);");

				if(node->child)
				{
					pval->submenu_name = pval_parent->submenu_name + "_" + std::to_string(node->pos());
					cd->init.push_back("auto* " + pval->submenu_name + " = " + pval_parent->submenu_name + "->create_sub_menu(" + std::to_string(node->pos()) + ");");
				}
			}

			return true;
		});
	}


	void menubar::create_menu_tree()
	{
		menu_tree.clear();
		
		// populate the menu_tree
		items.for_each([this](tree_node<ctrls::properties_collection>* node) -> bool
			{
				menu_data md;
				md.item = &node->value;

				if(node->value.property("owner").as_string().empty())
				{
					menu_tree.append(md);
				}
				else
				{
					auto owner = node->value.property("owner").as_string();

					menu_tree.for_each([&owner, &md](tree_node<menu_data>* node) -> bool
						{
							if(owner == node->value.item->property("key").as_string())
							{
								node->append(md);
								return false;
							}
							return true;
						});
				}
				return true;
			});
	}

}//end namespace ctrls
