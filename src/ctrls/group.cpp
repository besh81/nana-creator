/*
 *		ctrls::group Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/group.h"


namespace ctrls
{

	//group
	group::group(ctrl* parent, const std::string& name, bool visible)
		: ctrl(parent)
	{
		grp.create(*parent->nanawdg, visible);
		grp.caption(CTRL_GROUP); //BUG: if removed the group caption is not displayed at the beginning !!!
		boxmodel.bind(&grp);

		ctrl::init(&grp, CTRL_GROUP, name);

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = CTRL_GROUP;
		properties.append("format").label("Format").category(CAT_COMMON).type(pg_type::check) = false;
		// appearance
		properties.append("halign").label("Caption alignment").category(CAT_APPEARANCE).type(pg_type::halign) = static_cast<int>(nana::align::left);
		// layout
		properties.remove("weight");
		properties.append("layout").label("Layout").category(CAT_LAYOUT).type(pg_type::layout) = static_cast<int>(layout_orientation::horizontal);
		properties.append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_weight) = -1;
		properties.append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::margin) = "5,5,5,5";
		properties.append("gap").label("Gap").category(CAT_LAYOUT).type(pg_type::string_uint) = 2;
	}


	void group::update()
	{
		ctrl::update();

		// format and caption
		if(properties.property("format").as_bool())
		{
			try
			{
				grp.caption(properties.property("caption").as_string());
				grp.enable_format_caption(true);
			}
			catch(...)
			{
				nana::msgbox m(0, CREATOR_NAME, nana::msgbox::ok);
				m.icon(m.icon_warning);
				m << "Errore formato!"; //TODO
				m();

				grp.enable_format_caption(false);
				grp.caption(properties.property("caption").as_string());
			}
		}
		else
		{
			grp.enable_format_caption(false);
			grp.caption(properties.property("caption").as_string());
		}

		grp.caption_align(static_cast<nana::align>(properties.property("halign").as_int()));

		boxmodel.set_type(static_cast<layout_orientation>(properties.property("layout").as_int()));
		boxmodel.set_attributes("", properties.property("margin").as_string(), properties.property("gap").as_string());
		boxmodel.update();

		// needed to correctly redraw the group control when the caption change
		nana::API::refresh_window(grp);
	}


	void group::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/group.hpp>");
		// declaration
		cd->decl.push_back("nana::group " + name + ";");
		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");
		
		if(properties.property("format").as_bool())
			cd->init.push_back(name + ".enable_format_caption(" + properties.property("format").as_string() + ");");
		if(properties.property("halign").as_int() != static_cast<int>(nana::align::left))
			cd->init.push_back(name + ".caption_align(static_cast<nana::align>(" + properties.property("halign").as_string() + "));");
		if(children())
			cd->init.push_back(name + ".div(\"" + boxmodel.get(DEFAULT_FIELD, true) + "\");");

		cd->init.push_back(name + ".caption(\"" + properties.property("caption").as_string() + "\");");

		if(!properties.property("enabled").as_bool())
			cd->init.push_back(name + ".enabled(false);");
		generatecode_colors(cd, ci, name);
		// placement
		cd->init.push_back(ci->place + "[\"" + ci->field + "\"] << " + name + ";");
		// collocate
		if(children())
			cd->init_post.push_back(name + ".collocate();");
		// children
		ci->create = name;
		ci->place = name;
		ci->field = DEFAULT_FIELD;
	}

}//end namespace ctrls
