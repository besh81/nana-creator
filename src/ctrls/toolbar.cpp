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
		properties.append("scale").label("Scale").category(CAT_APPEARANCE).type(pg_type::string_uint) = DEF_IMG_SIZE;
		// layout
		// ...
	}


	void toolbar::init_item(properties_collection& item)
	{
		ctrl::init_item(item);
		item.property("type") = "button";
		//
		item.append("text").label("Text").category(CAT_COMMON).type(pg_type::string) = "New Item";
		item.append("image").label("Image").category(CAT_COMMON).type(pg_type::image) = "";
		item.append("enable").label("Enable").category(CAT_COMMON).type(pg_type::check) = true;
		item.append("textout").label("Textout").category(CAT_COMMON).type(pg_type::check) = false;
		//
		item.append("toggle_type").label("Toggle").category(CAT_TYPE).type(pg_type::check) = false;
		item.append("toggle_pushed").label("Pushed").category(CAT_TYPE).type(pg_type::check).enabled("toggle_type", true) = false;
		item.append("toggle_group").label("Group").category(CAT_TYPE).type(pg_type::string).enabled("toggle_type", true) = "";
		//
		item.append("separator") = false;
		item.append("goright") = false;
	}


	void toolbar::update()
	{
		ctrl::update();

		tlb.clear();

		tlb.scale(properties.property("scale").as_uint());

		// buttons - START
		std::size_t pos = 0;
		for(auto& i : items)
		{
			if(i.property("goright").as_bool())
			{
				tlb.go_right();
				continue;
			}

			if(i.property("separator").as_bool())
				tlb.separate();
			else
			{
				if(i.property("image").as_string().empty())
					tlb.append(i.property("text").as_string());
				else
					tlb.append(i.property("text").as_string(), nana::paint::image(g_file_mgr.to_relative(i.property("image").as_string())));

				tlb.enable(pos, i.property("enable").as_bool());
				tlb.textout(pos, i.property("textout").as_bool());
				
				if(i.property("toggle_type").as_bool())
				{
					tlb.tooltype(pos, nana::toolbar::tool_type::toggle);
					if(!i.property("toggle_group").as_string().empty())
						tlb.toggle_group(pos, i.property("toggle_group").as_string());
					tlb.toggle(pos, i.property("toggle_pushed").as_bool());
				}
			}

			++pos;
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
		if(properties.property("scale").as_uint() != DEF_IMG_SIZE)
			cd->init.push_back(name + ".scale(" + properties.property("scale").as_string() + ");");

		// buttons - START
		std::size_t pos = 0; //TODO : nella nuova versione non dovrebbe più servire
		for(auto& i : items)
		{
			if(i.property("goright").as_bool())
			{
				cd->init.push_back(name + ".go_right();");
				continue;
			}

			if(i.property("separator").as_bool())
				cd->init.push_back(name + ".separate();");
			else
			{
				/*
				auto str = name + ".append(\"" + i.property("text").as_string() + "\"";
				if(i.property("image").as_string().empty())
					str.append(");");
				else
					str.append(", nana::paint::image(\"" + g_file_mgr.to_relative(i.property("image").as_string()) + "\"));");
				cd->init.push_back(str);

				if(!i.property("enable").as_bool())
					cd->init.push_back(name + ".enable(" + std::to_string(pos) + ", false);");
				if(i.property("textout").as_bool())
					cd->init.push_back(name + ".textout(" + std::to_string(pos) + ", true);");

				if(i.property("toggle_type").as_bool())
				{
					cd->init.push_back(name + ".tooltype(" + std::to_string(pos) + ", nana::toolbar::tool_type::toggle);");
					if(!i.property("toggle_group").as_string().empty())
						cd->init.push_back(name + ".toggle_group(" + std::to_string(pos) + ", \"" + i.property("toggle_group").as_string() + "\");");
					if(i.property("toggle_pushed").as_bool())
						cd->init.push_back(name + ".toggle(" + std::to_string(pos) + ", true);");
				}
				*/
				auto str = name + ".append(\"" + i.property("text").as_string() + "\"";
				if(i.property("image").as_string().empty())
					str.append(")");
				else
					str.append(", nana::paint::image(\"" + g_file_mgr.to_relative(i.property("image").as_string()) + "\"))");

				if(!i.property("enable").as_bool())
					str.append(".enable(false)");
				if(i.property("textout").as_bool())
					str.append(".textout(true)");

				if(i.property("toggle_type").as_bool())
				{
					// TODO: questo va direttamente dentro la funzione append
					//cd->init.push_back(name + ".tooltype(" + std::to_string(pos) + ", nana::toolbar::tool_type::toggle);");
					if(!i.property("toggle_group").as_string().empty())
						str.append(".toggle_group(\"" + i.property("toggle_group").as_string() + "\")");
					if(i.property("toggle_pushed").as_bool())
						str.append(".toggle(true)");
				}

				str.append(";");
				cd->init.push_back(str);
			}

			++pos;
		}
		// buttons - END
	}

}//end namespace ctrls
