/*
 *		ctrls::textbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/textbox.h"
#include "nana_extra/color_helper.h"

#define DEF_SELECT_BEHAVIOR	false // get from nana source
#define DEF_UNDO_LENGHT		30 // get from nana source


namespace ctrls
{

	//textbox
	textbox::textbox(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		txt.create(*parent->nanawdg);
		ctrl::init(&txt, CTRL_TEXTBOX, name);

		def_scheme = txt.scheme();

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = "";
		properties.append("tip_string").label("Tip").category(CAT_COMMON).type(pg_type::string) = "";
		properties.append("editable").label("Editable").category(CAT_COMMON).type(pg_type::check) = txt.editable();
		properties.append("line_wrapped").label("Line wrapped").category(CAT_COMMON).type(pg_type::check) = txt.line_wrapped();
		properties.append("multi_lines").label("Multiple lines").category(CAT_COMMON).type(pg_type::check) = txt.multi_lines();
		properties.append("focus_behavior").label("Focus behavior").category(CAT_COMMON).type(pg_type::focus_behavior) = static_cast<int>(nana::textbox::text_focus_behavior::none); // get from nana source
		properties.append("select_behavior").label("Select behavior").category(CAT_COMMON).type(pg_type::check) = DEF_SELECT_BEHAVIOR;
		properties.append("undo_length").label("Undo length").category(CAT_COMMON).type(pg_type::string_uint) = DEF_UNDO_LENGHT;
		// appearance
		properties.append("halign").label("Text alignment").category(CAT_APPEARANCE).type(pg_type::halign) = static_cast<int>(nana::align::left);
		// layout
		// ...
		//scheme
		properties.append("selection").label("Selection").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.selection);
		properties.append("selection_unfocused").label("Selection unfocused").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.selection_unfocused);
		properties.append("selection_text").label("Selection text").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.selection_text);
		//parameters::mouse_wheel mouse_wheel;	///< The number of lines/characters to scroll when the vertical/horizontal mouse wheel is moved.
	}


	void textbox::update()
	{
		ctrl::update();

		txt.caption(properties.property("caption").as_string());
		txt.tip_string(properties.property("tip_string").as_string());
		txt.editable(properties.property("editable").as_bool());
		txt.line_wrapped(properties.property("line_wrapped").as_bool());
		txt.multi_lines(properties.property("multi_lines").as_bool());
		txt.focus_behavior(static_cast<nana::textbox::text_focus_behavior>(properties.property("focus_behavior").as_int()));
		txt.select_behavior(properties.property("select_behavior").as_bool());
		txt.set_undo_queue_length(properties.property("undo_length").as_uint());
		txt.text_align(static_cast<nana::align>(properties.property("halign").as_int()));

		//scheme
		txt.scheme().selection = nana::to_color(properties.property("selection").as_string());
		txt.scheme().selection_unfocused = nana::to_color(properties.property("selection_unfocused").as_string());
		txt.scheme().selection_text = nana::to_color(properties.property("selection_text").as_string());
		// mouse_wheel
		nana::API::refresh_window(txt);
	}


	void textbox::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/textbox.hpp>");
		// declaration
		cd->decl.push_back("nana::textbox " + name + ";");
		// init
		if(!properties.property("caption").as_string().empty())
			cd->init.push_back(name + ".caption(\"" + properties.property("caption").as_string() + "\");");
		if(!properties.property("tip_string").as_string().empty())
			cd->init.push_back(name + ".tip_string(\"" + properties.property("tip_string").as_string() + "\");");
		if(!properties.property("editable").as_bool())
			cd->init.push_back(name + ".editable(false);");
		if(properties.property("line_wrapped").as_bool())
			cd->init.push_back(name + ".line_wrapped(true);");
		if(!properties.property("multi_lines").as_bool())
			cd->init.push_back(name + ".multi_lines(false);");
		if(properties.property("focus_behavior").as_int() != static_cast<int>(nana::textbox::text_focus_behavior::none))
			cd->init.push_back(name + ".focus_behavior(static_cast<nana::textbox::text_focus_behavior>(" + properties.property("focus_behavior").as_string() + "));");
		if(properties.property("select_behavior").as_bool() != DEF_SELECT_BEHAVIOR)
			cd->init.push_back(name + ".select_behavior(\"" + properties.property("select_behavior").as_string() + "\");");
		if(properties.property("undo_length").as_uint() != DEF_UNDO_LENGHT)
			cd->init.push_back(name + ".set_undo_queue_length(" + properties.property("undo_length").as_string() + ");");
		if(properties.property("halign").as_int() != static_cast<int>(nana::align::left))
			cd->init.push_back(name + ".text_align(static_cast<nana::align>(" + properties.property("halign").as_string() + "));");

		//scheme
		if(properties.property("selection").as_string() != nana::to_string(def_scheme.selection))
			cd->init.push_back(name + ".scheme().selection = nana::color(" + properties.property("selection").as_string() + ");");
		if(properties.property("selection_unfocused").as_string() != nana::to_string(def_scheme.selection_unfocused))
			cd->init.push_back(name + ".scheme().selection_unfocused = nana::color(" + properties.property("selection_unfocused").as_string() + ");");
		if(properties.property("selection_text").as_string() != nana::to_string(def_scheme.selection_text))
			cd->init.push_back(name + ".scheme().selection_text = nana::color(" + properties.property("selection_text").as_string() + ");");
		// scheme::mouse_wheel
	}

}//end namespace ctrls
