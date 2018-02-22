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



namespace ctrls
{

	//panel
	panel::panel(nana::window wd, const std::string& name, bool mainclass, bool visible)
		: ctrl()
	{
		pnl.create(wd, visible);
		_place.bind(pnl);
		_place.div("abc");
		_place.collocate();

		// project
		if(mainclass)
		{
			properties.append("mainclass") = true;
			properties.append("filename").label("File Name").category(CAT_CPPCODE).type(pg_type::string) = DEFAULT_FILENAME;
		}

		ctrl::init(&pnl, CTRL_PANEL, name);

		// common
		//properties.append(property_t{ "hasbackground", "", "Has Background", "", pg_type::check }) = true; //TODO
		// appearance
		// ...
		// layout
		if(mainclass)
		{
			properties.remove("weight");
			properties.remove("margin");
			properties.append("width").label("Width").category(CAT_LAYOUT).type(pg_type::string_uint) = MAIN_WDG_W;
			properties.append("height").label("Height").category(CAT_LAYOUT).type(pg_type::string_uint) = MAIN_WDG_H;
		}
	}


	void panel::update()
	{
		//ctrl::update();

		auto pw = nana::API::get_widget(nanawdg->parent());
		if(properties.property("mainclass").as_bool())
			pw = nana::API::get_widget(pw->parent());
		bool inherited;
		nana::color col;

		nanawdg->enabled(properties.property("enabled").as_bool());

		col = nana::to_color(properties.property("bgcolor").as_string(), inherited);
		nanawdg->bgcolor(inherited ? pw->bgcolor() : col);

		col = nana::to_color(properties.property("fgcolor").as_string(), inherited);
		nanawdg->fgcolor(inherited ? pw->fgcolor() : col);

		if(properties.property("mainclass").as_bool())
		{
			pnl.size(nana::size(properties.property("width").as_uint(), properties.property("height").as_uint()));
		}

		_place.collocate();
	}


	void panel::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);

		auto name = properties.property("name").as_string();

		// mainclass definition
		if(properties.property("mainclass").as_bool())
		{
			cd->mainclass = name;
			cd->mainclass_base = "nana::panel<true>"; //TODO: controllare hasbackground
			cd->mainclass_ctor = "(nana::window wd, bool visible = true)";
			cd->mainclass_base_ctor = "(wd, visible)";

			// filename
			cd->filename = properties.property("filename").as_string();
		}

		// headers
		cd->hpps.add("#include <nana/gui/widgets/panel.hpp>");
		// declaration
		if(!properties.property("mainclass").as_bool())
			cd->decl.push_back("nana::panel<true> " + name + ";"); //TODO: controllare hasbackground
		// init
		if(!properties.property("mainclass").as_bool())
		{
			cd->init.push_back("// " + name);
			cd->init.push_back(name + ".create(" + ci->create + ");");
			cd->init.push_back(name + ".enabled(" + properties.property("enabled").as_string() + ");");
			// color
			bool inherited;
			std::string col;
			// bg
			col = properties.property("bgcolor").as_string();
			nana::to_color(col, inherited);
			if(!inherited)
				cd->init.push_back(name + ".bgcolor(nana::color(" + col + "));");
			// fg
			col = properties.property("fgcolor").as_string();
			nana::to_color(col, inherited);
			if(!inherited)
				cd->init.push_back(name + ".fgcolor(nana::color(" + col + "));");
		}
		else
		{
			cd->init.push_back("enabled(" + properties.property("enabled").as_string() + ");");
			// color
			bool inherited;
			std::string col;
			// bg
			col = properties.property("bgcolor").as_string();
			nana::to_color(col, inherited);
			if(!inherited)
				cd->init.push_back(name + "bgcolor(nana::color(" + col + "));");
			// fg
			col = properties.property("fgcolor").as_string();
			nana::to_color(col, inherited);
			if(!inherited)
				cd->init.push_back(name + "fgcolor(nana::color(" + col + "));");
		}
		// placement
		if(!properties.property("mainclass").as_bool())
			cd->init.push_back(ci->place + "[\"field" + std::to_string(ci->field) + "\"] << " + name + ";");
		// children
		if(!properties.property("mainclass").as_bool())
			ci->create = name;
		ci->place = "";
	}


	bool panel::append(nana::window child)
	{
		if(haschild())
			return false;

		_child = true;

		_place.field("abc") << child;
		_place.collocate();
		return true;
	}


	bool panel::remove(nana::window child)
	{
		if(!_child)
			return false;

		_place.field_display("abc", false);
		_place.erase(child);
		_place.field_display("abc", true);
		_place.collocate();

		_child = false;
		return true;
	}

}//end namespace ctrls
