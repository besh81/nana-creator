/*
 *		ctrls::page Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/page.h"
#include "ctrls/notebook.h"
#include "filemanager.h"


extern filemanager		g_file_mgr;



namespace ctrls
{

	//page
	page::page(ctrl* parent, const std::string& name, bool visible)
		: ctrl(parent)
	{
		pnl.create(*parent->nanawdg, visible);
		boxmodel.bind(pnl);

		ctrl::init(&pnl, CTRL_PAGE, name, false);

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = CTRL_PAGE;
		properties.append("image").label("Image").category(CAT_COMMON).type(pg_type::image) = "";
		// appearance
		// ...
		// layout
		properties.remove("weight");
		properties.append("layout").label("Layout").category(CAT_LAYOUT).type(pg_type::layout) = static_cast<int>(layout_orientation::horizontal);
		properties.append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::margin) = "5,5,5,5";
		properties.append("gap").label("Gap").category(CAT_LAYOUT).type(pg_type::string_uint) = 2;
	}


	void page::update()
	{
		ctrl::update();

		boxmodel.set_type(static_cast<layout_orientation>(properties.property("layout").as_int()));
		boxmodel.set_attributes("", properties.property("margin").as_string(), properties.property("gap").as_string());
		boxmodel.update();
	}


	void page::select(bool state)
	{
		_selected = state;

		if(_selected && !nanawdg->visible())
		{
			ctrls::notebook* ntb = static_cast<ctrls::notebook*>(parent());
			ntb->show_page(this);
		}
	}


	void page::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);

		auto name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/panel.hpp>");
		cd->hpps.add("#include <nana/gui/place.hpp>");
		// declaration
		cd->decl.push_back("nana::panel<true> " + name + ";");
		if(children())
			cd->decl.push_back("nana::place " + name + "_place_;");
		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + "_panel_);");

		if(children())
		{
			cd->init.push_back(name + "_place_.bind(" + name + ");");
			cd->init.push_back(name + "_place_.div(\"" + boxmodel.get(DEFAULT_FIELD, true) + "\");");
		}

		if(!properties.property("enabled").as_bool())
			cd->init.push_back(name + ".enabled(false);");
		generatecode_colors(cd, ci, name);


		// connect to notebook
		cd->init.push_back(ci->create + "_tabbar_.push_back(\"" + properties.property("caption").as_escaped_string() + "\");");
		if(!properties.property("image").as_string().empty())
			cd->init.push_back(ci->create + "_tabbar_.tab_image(" + ci->create + "_tabbar_.length()-1, nana::paint::image(\"" + g_file_mgr.to_relative(properties.property("image").as_string()) + "\"));");
		cd->init.push_back(ci->create + "_tabbar_.tab_bgcolor(" + ci->create + "_tabbar_.length()-1, " + name + ".bgcolor());");
		cd->init.push_back(ci->create + "_tabbar_.tab_fgcolor(" + ci->create + "_tabbar_.length()-1, " + name + ".fgcolor());");
		cd->init.push_back(ci->create + "_tabbar_.attach(" + ci->create + "_tabbar_.length()-1, " + name + ");");

				
		// placement
		cd->init.push_back(ci->place + "[\"" + ci->field + "\"].fasten(" + name + ");");
		// collocate
		if(children())
			cd->init_post.push_back(name + "_place_.collocate();");
		// children
		ci->create = name;
		ci->place = name + "_place_";
		ci->field = DEFAULT_FIELD;
	}

}//end namespace ctrls
