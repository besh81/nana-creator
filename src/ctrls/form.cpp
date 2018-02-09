/*
 *		ctrls::form Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/form.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"



namespace ctrls
{

	//form
	form::form(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::panel<true>(wd)
	{
		_place.div("abc");
		_place.collocate();


		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);


		_initproperties(properties, name);
	}


	void form::update(properties_collection* properties)
	{
		// form IS ALWAYS mainclass

		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;

		pw = nana::API::get_widget(pw->parent());

		//
		enabled(properties->property("enabled").as_bool());
		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);

		//
		size(nana::size(properties->property("width").as_uint(), properties->property("height").as_uint()));

		_place.collocate();
	}


	void form::generatecode(properties_collection* properties, code_data_struct* cd, code_info_struct* ci)
	{
		// form IS ALWAYS mainclass

		auto name = properties->property("name").as_string();

		std::string sz = properties->property("width").as_string() + ", " + properties->property("height").as_string();

		std::string apr = properties->property("decoration").as_string() + ", " + properties->property("taskbar").as_string() + ", ";
		apr.append(properties->property("floating").as_string() + ", " + properties->property("no_activate").as_string() + ", ");
		apr.append(properties->property("min").as_string() + ", " + properties->property("max").as_string() + ", " + properties->property("sizable").as_string());

		cd->mainclass = name;
		cd->mainclass_base = "nana::form";
		cd->mainclass_ctor = "(nana::window wd, const ::nana::size& sz = {" + sz + "}, const nana::appearance& apr = {" + apr + "})";
		cd->mainclass_base_ctor = "(wd, sz, apr)";

		// filename
		cd->filename = properties->property("filename").as_string();

		// headers
		cd->hpps.add("#include <nana/gui.hpp>");

		// init
		cd->init.push_back("caption(\"" + properties->property("caption").as_string() + "\");");
		cd->init.push_back("enabled(" + properties->property("enabled").as_string() + ");");
		// color
		bool inherited;
		std::string col;
		// bg
		col = properties->property("bgcolor").as_string();
		nana::to_color(col, inherited);
		if(!inherited)
			cd->init.push_back(name + "bgcolor(nana::color(" + col + "));");
		// fg
		col = properties->property("fgcolor").as_string();
		nana::to_color(col, inherited);
		if(!inherited)
			cd->init.push_back(name + "fgcolor(nana::color(" + col + "));");

		// placement


		// children


		ci->place = "";
	}


	void form::_initproperties(properties_collection* properties, const std::string& name)
	{
		// form IS ALWAYS mainclass

		// properties - main
		properties->append("type") = CTRL_FORM;
		properties->append("name") = name;

		// project
		properties->append("filename").label("File Name").category(CAT_CPPCODE).type(pg_type::string) = DEFAULT_FILENAME;
		
		// common
		properties->append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = "Form";
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = enabled();
		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(bgcolor(), true);
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(fgcolor(), true);
		//
		properties->append("decoration").label("Decoration").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties->append("taskbar").label("Taskbar").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties->append("floating").label("Floating").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties->append("no_activate").label("NoActivate").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties->append("min").label("Min").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties->append("max").label("Max").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties->append("sizable").label("Sizable").category(CAT_APPEARANCE).type(pg_type::check) = false;

		// layout
		properties->append("width").label("Width").category(CAT_LAYOUT).type(pg_type::string_uint) = MAIN_WDG_W;
		properties->append("height").label("Height").category(CAT_LAYOUT).type(pg_type::string_uint) = MAIN_WDG_H;
	}


	bool form::append(nana::window child)
	{
		if(haschild())
			return false;

		_child = true;

		_place.field("abc") << child;
		_place.collocate();
		return true;
	}


	bool form::remove(nana::window child)
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
