/*
 *		ctrls::label Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/label.h"


namespace ctrls
{

	//label
	label::label(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		lbl.create(*parent->nanawdg);
		ctrl::init(&lbl, CTRL_LABEL, name);

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = CTRL_LABEL;
		properties.append("format").label("Format").category(CAT_COMMON).type(pg_type::check) = false;
		// appearance
		properties.append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check) = lbl.transparent();
		properties.append("halign").label("Horizontal alignment").category(CAT_APPEARANCE).type(pg_type::halign) = static_cast<int>(nana::align::left);
		properties.append("valign").label("Vertical alignment").category(CAT_APPEARANCE).type(pg_type::valign) = static_cast<int>(nana::align_v::top);
		// layout
		// ...
	}


	void label::update()
	{
		ctrl::update();

		// format and caption
		if(properties.property("format").as_bool())
		{
			try
			{
				lbl.caption(properties.property("caption").as_string());
				lbl.format(true);
			}
			catch(...)
			{
				nana::msgbox m(0, CREATOR_NAME, nana::msgbox::ok);
				m.icon(m.icon_warning);
				m << "Errore formato!"; //TODO
				m();

				lbl.format(false);
				lbl.caption(properties.property("caption").as_string());
			}
		}
		else
		{
			lbl.format(false);
			lbl.caption(properties.property("caption").as_string());
		}

		lbl.transparent(properties.property("transparent").as_bool());
		lbl.text_align(static_cast<nana::align>(properties.property("halign").as_int()),
			static_cast<nana::align_v>(properties.property("valign").as_int()));
	}


	void label::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/label.hpp>");
		// declaration
		cd->decl.push_back("nana::label " + name + ";");
		// init
		cd->init.push_back(name + ".caption(\"" + properties.property("caption").as_string() + "\");");
		if(properties.property("format").as_bool())
			cd->init.push_back(name + ".format(true);");
		if(properties.property("transparent").as_bool())
			cd->init.push_back(name + ".transparent(true);");
		if(properties.property("halign").as_int() != static_cast<int>(nana::align::left) || properties.property("valign").as_int() != static_cast<int>(nana::align_v::top))
			cd->init.push_back(name + ".text_align(static_cast<nana::align>(" + properties.property("halign").as_string() + "), static_cast<nana::align_v>(" + properties.property("valign").as_string() + "));");
	}

}//end namespace ctrls
