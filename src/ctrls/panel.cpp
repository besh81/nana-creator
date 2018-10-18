/*
 *		ctrls::panel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/panel.h"
#include "nana_extra/color_helper.h"
#include "filemanager.h"


extern filemanager		g_file_mgr;


namespace ctrls
{

	//panel
	panel::panel(nana::window wd, const std::string& name, bool mainclass, bool visible)
		: ctrl()
	{
		_mainclass = mainclass;

		pnl.create(wd, visible);
		boxmodel.bind(pnl);

		// project
		if(_mainclass)
		{
			properties.append("mainclass") = true;
			properties.append("filename").label("File Name").category(CAT_CPPCODE).type(pg_type::string) = DEFAULT_FILENAME;
			properties.append("rel_path").label("Relative Path").category(CAT_CPPCODE).type(pg_type::check) = true;
			properties.append("work_dir").label("Working Dir").category(CAT_CPPCODE).type(pg_type::folder).enabled("rel_path", true) = "";
		}

		ctrl::init(&pnl, CTRL_PANEL, name, false);

		// common
		if(_mainclass)
		{
			properties.append("width").label("Width").category(CAT_COMMON).type(pg_type::string_uint) = MAIN_WDG_W;
			properties.append("height").label("Height").category(CAT_COMMON).type(pg_type::string_uint) = MAIN_WDG_H;
		}
		// appearance
		// ...
		// layout
		properties.remove("weight");
		if(_mainclass)
		{
			properties.append("layout").label("Layout").category(CAT_LAYOUT).type(pg_type::layout) = static_cast<int>(layout_orientation::horizontal);
			properties.append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 5;
			properties.append("gap").label("Gap").category(CAT_LAYOUT).type(pg_type::string_uint) = 2;
		}
		else
		{
			properties.append("layout").label("Layout").category(CAT_LAYOUT).type(pg_type::layout) = static_cast<int>(layout_orientation::horizontal);
			properties.append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_weight) = -1;
			properties.append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 5;
			properties.append("gap").label("Gap").category(CAT_LAYOUT).type(pg_type::string_uint) = 2;
		}
	}


	void panel::update()
	{
		//ctrl::update();

		// manage absolute/relative path
		if(_mainclass)
		{
			g_file_mgr.enable_relative(properties.property("rel_path").as_bool());
			g_file_mgr.basedir(properties.property("work_dir").as_string());
		}

		auto pw = nana::API::get_widget(nanawdg->parent());
		if(_mainclass)
			pw = nana::API::get_widget(pw->parent());
		bool inherited;
		nana::color col;

		nanawdg->enabled(properties.property("enabled").as_bool());

		col = nana::to_color(properties.property("bgcolor").as_string(), inherited);
		nanawdg->bgcolor(inherited ? pw->bgcolor() : col);

		col = nana::to_color(properties.property("fgcolor").as_string(), inherited);
		nanawdg->fgcolor(inherited ? pw->fgcolor() : col);

		if(_mainclass)
		{
			pnl.size(nana::size(properties.property("width").as_uint(), properties.property("height").as_uint()));
		}


		boxmodel.set_type(static_cast<layout_orientation>(properties.property("layout").as_int()));
		if(_mainclass)
			boxmodel.set_attributes("", properties.property("margin").as_string(), properties.property("gap").as_string());
		else
			boxmodel.set_attributes(properties.property("weight").as_string(), properties.property("margin").as_string(), properties.property("gap").as_string());
		boxmodel.update();
	}


	void panel::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);

		auto name = properties.property("name").as_string();

		// mainclass definition
		if(_mainclass)
		{
			cd->mainclass = name;
			cd->mainclass_base = "nana::panel<true>"; //TODO: add hasbackground prop
			cd->mainclass_ctor = "(nana::window wd, bool visible = true)";
			cd->mainclass_base_ctor = "(wd, visible)";

			// filename
			cd->filename = properties.property("filename").as_string();
		}

		// headers
		cd->hpps.add("#include <nana/gui/widgets/panel.hpp>");
		cd->hpps.add("#include <nana/gui/place.hpp>");
		// declaration
		if(!_mainclass)
		{
			cd->decl.push_back("nana::panel<true> " + name + ";"); //TODO: add hasbackground prop
			if(children())
				cd->decl.push_back("nana::place " + name + "_place;");
		}
		else
		{
			// mainclass
			cd->decl.push_back("nana::place _place{ *this };");
		}
		// init
		if(!_mainclass)
		{
			cd->init.push_back("// " + name);
			cd->init.push_back(name + ".create(" + ci->create + ");");

			if(children())
			{
				cd->init.push_back(name + "_place.bind(" + name + ");");
				cd->init.push_back(name + "_place.div(\"" + boxmodel.get(DEFAULT_FIELD, true) + "\");");
			}

			if(!properties.property("enabled").as_bool())
				cd->init.push_back(name + ".enabled(" + properties.property("enabled").as_string() + ");");

			generatecode_colors(cd, ci, name);
		}
		else
		{
			// mainclass
			cd->init.push_back("_place.div(\"" + boxmodel.get(DEFAULT_FIELD, true) + "\");");
			
			if(!properties.property("enabled").as_bool())
				cd->init.push_back("enabled(" + properties.property("enabled").as_string() + ");");

			generatecode_colors(cd, ci);
		}
		// placement
		if(!_mainclass)
		{
			cd->init.push_back(ci->place + "[\"" + ci->field + "\"] << " + name + ";");
		}
		// collocate
		if(!_mainclass)
		{
			if(children())
				cd->init_post.push_back(name + "_place.collocate();");
		}
		else
		{
			// mainclass
			cd->init_post.push_back("_place.collocate();");
		}
		// children
		if(!_mainclass)
		{
			ci->create = name;
			ci->place = name + "_place";
		}
		else
		{
			// mainclass
			ci->place = "_place";
		}
		ci->field = DEFAULT_FIELD;
	}


	void panel::update_children_info(nana::window child, const std::string& divtext, const std::string& weight)
	{
		boxmodel.update_children_info(child, divtext, weight);
	}


	bool panel::children()
	{
		return boxmodel.children();
	}


	bool panel::children_fields()
	{
		return boxmodel.children_fields();
	}
	

	bool panel::append(nana::window ctrl)
	{
		return boxmodel.append(ctrl);
	}


	bool panel::insert(nana::window pos, nana::window ctrl, bool after)
	{
		return boxmodel.insert(pos, ctrl, after);
	}


	bool panel::remove(nana::window ctrl)
	{
		return boxmodel.remove(ctrl);
	}


	bool panel::moveup(nana::window ctrl)
	{
		return boxmodel.moveup(ctrl);
	}


	bool panel::movedown(nana::window ctrl)
	{
		return boxmodel.movedown(ctrl);
	}

}//end namespace ctrls
