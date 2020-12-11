/*
 *		ctrls::toolbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/toolbar.h"
#include "filemanager.h"

#define DEF_IMG_SIZE	16 // get from nana source


extern filemanager		g_file_mgr;


namespace ctrls
{

	//toolbar
	toolbar::toolbar(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		tlb.create(*parent->nanawdg);
		ctrl::init(&tlb, CTRL_TOOLBAR, name);

		// common
		properties.append("buttons").label("Buttons").category(CAT_COMMON).type(pg_type::collection_toolbar) = "";
		// appearance
		properties.append("tools_height").label("Tools height").category(CAT_APPEARANCE).type(pg_type::string_uint) = DEF_IMG_SIZE;
		// layout
		// ...
	}


	void toolbar::init_item(properties_collection& item, const std::string& type)
	{
		ctrl::init_item(item);

		item.property("type") = type;
		if(type == "separator" || type == "go_right")
			return;

		item.append("text").label("Text").category(CAT_COMMON).type(pg_type::string) = "New Item";
		item.append("image").label("Image").category(CAT_COMMON).type(pg_type::image) = "";
		item.append("enable").label("Enable").category(CAT_COMMON).type(pg_type::check) = true;
		if(type != "dropdown-item")
		{
			item.append("textout").label("Textout").category(CAT_COMMON).type(pg_type::check) = false;
		}
		if(type == "toggle")
		{
			item.append("toggle_pushed").label("Pushed").category(CAT_COMMON).type(pg_type::check).enabled("toggle_type", true) = false;
			item.append("toggle_group").label("Group").category(CAT_COMMON).type(pg_type::string).enabled("toggle_type", true) = "";
		}
	}


	void toolbar::update()
	{
		ctrl::update();

		tlb.clear();

		tlb.tools_height(properties.property("tools_height").as_uint());

		// buttons - START
		items.for_each([this](tree_node<ctrls::properties_collection>* node) -> bool
			{
				std::string type = node->value.property("type").as_string();
				if(type == "go_right")
				{
					tlb.go_right();
					return true;
				}
				if(type == "separator")
				{
					tlb.append_separator();
					return true;
				}

				nana::toolbar::tools t = nana::toolbar::tools::button;
				if(type == "toggle")
					t = nana::toolbar::tools::toggle;
				else if(type == "dropdown")
				{
					t = nana::toolbar::tools::dropdown;
				}
				else if(type == "dropdown-item")
				{
					auto ip = tlb.at(tlb.count() - 1);
					if(node->value.property("image").as_string().empty())
						ip.dropdown_append(node->value.property("text").as_string());
					else
						ip.dropdown_append(node->value.property("text").as_string(), nana::paint::image(g_file_mgr.to_relative(node->value.property("image").as_string())));

					ip.dropdown_enable(node->pos(), node->value.property("enable").as_bool());
					return true;
				}

				if(node->value.property("image").as_string().empty())
					tlb.append(t, node->value.property("text").as_string());
				else
					tlb.append(t, node->value.property("text").as_string(), nana::paint::image(g_file_mgr.to_relative(node->value.property("image").as_string())));

				auto ip = tlb.at(tlb.count() - 1);
				ip.enable(node->value.property("enable").as_bool());
				ip.textout(node->value.property("textout").as_bool());

				if(type == "toggle")
				{
					if(!node->value.property("toggle_group").as_string().empty())
						ip.toggle_group(node->value.property("toggle_group").as_string());
					ip.toggle(node->value.property("toggle_pushed").as_bool());
				}

				return true;
			});
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
		if(properties.property("tools_height").as_uint() != DEF_IMG_SIZE)
			cd->init.push_back(name + ".tools_height(" + properties.property("tools_height").as_string() + ");");

		// buttons - START
		items.for_each([cd, &name](tree_node<ctrls::properties_collection>* node) -> bool
			{
				std::string type = node->value.property("type").as_string();
				if(type == "go_right")
				{
					cd->init.push_back(name + ".go_right();");
					return true;
				}
				if(type == "separator")
				{
					cd->init.push_back(name + ".append_separator();");
					return true;
				}

				std::string str;

				if(type == "dropdown" && node->child)
				{
					str = "auto ip_" + name + "_" + std::to_string(node->pos()) + " = ";
				}
				else if(type == "dropdown-item")
				{
					str = "ip_" + name + "_" + std::to_string(node->owner->pos()) + ".dropdown_append(\"" + node->value.property("text").as_string() + "\"";
					if(!node->value.property("image").as_string().empty())
						str.append(", nana::paint::image(\"" + g_file_mgr.to_relative(node->value.property("image").as_string()) + "\")");
					str.append(")");
					cd->init.push_back(str + ";");

					if(!node->value.property("enable").as_bool())
					{
						cd->init.push_back("ip_" + name + "_" + std::to_string(node->owner->pos()) + ".dropdown_enable(" + std::to_string(node->pos()) + ", false);");
					}
					return true;
				}

				str.append(name + ".append(nana::toolbar::tools::" + type + ", \"" + node->value.property("text").as_string() + "\"");
				if(!node->value.property("image").as_string().empty())
					str.append(", nana::paint::image(\"" + g_file_mgr.to_relative(node->value.property("image").as_string()) + "\")");
				str.append(")");
				if(!node->value.property("enable").as_bool())
					str.append(".enable(false)");
				if(node->value.property("textout").as_bool())
					str.append(".textout(true)");

				if(type == "toggle")
				{
					if(!node->value.property("toggle_group").as_string().empty())
						str.append(".toggle_group(\"" + node->value.property("toggle_group").as_string() + "\")");
					if(node->value.property("toggle_pushed").as_bool())
						str.append(".toggle(true)");
				}

				cd->init.push_back(str + ";");

				return true;
			});
		// buttons - END
	}

}//end namespace ctrls
