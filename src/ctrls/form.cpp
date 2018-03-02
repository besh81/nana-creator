/*
 *		ctrls::form Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/form.h"
#include "nana_extra/color_helper.h"
#include "filemanager.h"


extern filemanager		g_file_mgr;


namespace ctrls
{

	//form
	form::form(nana::window wd, const std::string& name, bool visible)
		: ctrl()
	{
		frm.create(wd, visible);
		_place.bind(frm);
		_place.div("abc");
		_place.collocate();

		// form IS ALWAYS mainclass
		properties.append("mainclass") = true;
		// project
		properties.append("filename").label("File Name").category(CAT_CPPCODE).type(pg_type::string) = DEFAULT_FILENAME;
		properties.append("rel_path").label("Relative Path").category(CAT_CPPCODE).type(pg_type::check) = true;
		properties.append("work_dir").label("Working Dir").category(CAT_CPPCODE).type(pg_type::folder).enabled("rel_path", true) = "";

		ctrl::init(&frm, CTRL_FORM, name);

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = "Form";
		// appearance
		properties.append("decoration").label("Decoration").category(CAT_APPEARANCE).type(pg_type::check) = true;
		properties.append("taskbar").label("Taskbar").category(CAT_APPEARANCE).type(pg_type::check) = true;
		properties.append("floating").label("Floating").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("no_activate").label("NoActivate").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("min").label("Min").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("max").label("Max").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("sizable").label("Sizable").category(CAT_APPEARANCE).type(pg_type::check) = false;
		// layout
		properties.remove("weight");
		properties.remove("margin");
		properties.append("width").label("Width").category(CAT_LAYOUT).type(pg_type::string_uint) = MAIN_WDG_W;
		properties.append("height").label("Height").category(CAT_LAYOUT).type(pg_type::string_uint) = MAIN_WDG_H;
	}


	void form::update()
	{
		//ctrl::update();

		// manage absolute/relative path
		g_file_mgr.enable_relative(properties.property("rel_path").as_bool());
		g_file_mgr.basedir(properties.property("work_dir").as_string());

		auto pw = nana::API::get_widget(nanawdg->parent());
		pw = nana::API::get_widget(pw->parent());
		bool inherited;
		nana::color col;

		nanawdg->enabled(properties.property("enabled").as_bool());

		col = nana::to_color(properties.property("bgcolor").as_string(), inherited);
		nanawdg->bgcolor(inherited ? pw->bgcolor() : col);

		col = nana::to_color(properties.property("fgcolor").as_string(), inherited);
		nanawdg->fgcolor(inherited ? pw->fgcolor() : col);

		frm.size(nana::size(properties.property("width").as_uint(), properties.property("height").as_uint()));

		_place.collocate();
	}


	void form::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);

		// form IS ALWAYS mainclass
		auto name = properties.property("name").as_string();

		// mainclass definition
		std::string sz = properties.property("width").as_string() + ", " + properties.property("height").as_string();
		std::string apr = properties.property("decoration").as_string() + ", " + properties.property("taskbar").as_string() + ", ";
		apr.append(properties.property("floating").as_string() + ", " + properties.property("no_activate").as_string() + ", ");
		apr.append(properties.property("min").as_string() + ", " + properties.property("max").as_string() + ", " + properties.property("sizable").as_string());

		cd->mainclass = name;
		cd->mainclass_base = "nana::form";
		//cd->mainclass_ctor = "(nana::window wd, const ::nana::size& sz = {" + sz + "}, const nana::appearance& apr = {" + apr + "})";
		//cd->mainclass_base_ctor = "(wd, sz, apr)";
		cd->mainclass_ctor = "(const nana::rectangle& rect = nana::API::make_center(" + sz + "), const nana::appearance& apr = {" + apr + "})";
		cd->mainclass_base_ctor = "(rect, apr)";

		// filename
		cd->filename = properties.property("filename").as_string();

		// headers
		cd->hpps.add("#include <nana/gui.hpp>");
		// declaration
		// ...
		// init
		cd->init.push_back("enabled(" + properties.property("enabled").as_string() + ");");
		cd->init.push_back("caption(\"" + properties.property("caption").as_string() + "\");");
		// color
		bool inherited;
		std::string col;
		// bg
		col = properties.property("bgcolor").as_string();
		nana::to_color(col, inherited);
		if(!inherited)
			cd->init.push_back("bgcolor(nana::color(" + col + "));");
		// fg
		col = properties.property("fgcolor").as_string();
		nana::to_color(col, inherited);
		if(!inherited)
			cd->init.push_back("fgcolor(nana::color(" + col + "));");
		// placement
		// ...
		// children
		ci->place = "";
	}


	bool form::append(nana::window child)
	{
		if (haschild())
			return false;

		_child = true;

		_place.field("abc") << child;
		_place.collocate();
		return true;
	}


	bool form::remove(nana::window child)
	{
		if (!haschild())
			return false;

		_place.field_display("abc", false);
		_place.erase(child);
		_place.field_display("abc", true);
		_place.collocate();

		_child = false;
		return true;
	}

}//end namespace ctrls
