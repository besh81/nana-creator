/*
 *		ctrls::treebox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/treebox.h"
#include "nana_extra/color_helper.h"


namespace ctrls
{

	//treebox
	treebox::treebox(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		trb.create(*parent->nanawdg);
		ctrl::init(&trb, CTRL_TREEBOX, name);

		def_scheme = trb.scheme();

		// common
		properties.append("checkable").label("Checkable").category(CAT_COMMON).type(pg_type::check) = trb.checkable();
		// appearance
		properties.append("use_entire_line").label("Use entire line").category(CAT_APPEARANCE).type(pg_type::check) = false;
		// layout
		// ...
		//scheme
		properties.append("item_bg_selected").label("Selected bg").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.item_bg_selected);
		properties.append("item_fg_selected").label("Selected fg").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.item_fg_selected);
		properties.append("item_bg_highlighted").label("Highlighted bg").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.item_bg_highlighted);
		properties.append("item_fg_highlighted").label("Highlighted fg").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.item_fg_highlighted);
		properties.append("item_bg_selected_and_highlighted").label("Sel & High bg").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.item_bg_selected_and_highlighted);
		properties.append("item_fg_selected_and_highlighted").label("Sel & High fg").category(CAT_SCHEME).type(pg_type::color) = nana::to_string(def_scheme.item_fg_selected_and_highlighted);
		properties.append("item_offset").label("Item offset").category(CAT_SCHEME).type(pg_type::string_uint) = def_scheme.item_offset;
		properties.append("text_offset").label("Text offset").category(CAT_SCHEME).type(pg_type::string_uint) = def_scheme.text_offset;
		properties.append("icon_size").label("Icon size").category(CAT_SCHEME).type(pg_type::string_uint) = def_scheme.icon_size;
		properties.append("crook_size").label("Crook size").category(CAT_SCHEME).type(pg_type::string_uint) = def_scheme.crook_size;
		properties.append("indent_displacement").label("Indent displacement").category(CAT_SCHEME).type(pg_type::string_uint) = def_scheme.indent_displacement;
	}


	void treebox::update()
	{
		ctrl::update();

		trb.checkable(properties.property("checkable").as_bool());
		trb.use_entire_line(properties.property("use_entire_line").as_bool());

		//scheme
		/* NOTE: at the moment they are not needed because it is not possible to add elements to the treebox
		trb.scheme().item_bg_selected = nana::to_color(properties.property("item_bg_selected").as_string());
		trb.scheme().item_fg_selected = nana::to_color(properties.property("item_fg_selected").as_string());
		trb.scheme().item_bg_highlighted = nana::to_color(properties.property("item_bg_highlighted").as_string());
		trb.scheme().item_fg_highlighted = nana::to_color(properties.property("item_fg_highlighted").as_string());
		trb.scheme().item_bg_selected_and_highlighted = nana::to_color(properties.property("item_bg_selected_and_highlighted").as_string());
		trb.scheme().item_fg_selected_and_highlighted = nana::to_color(properties.property("item_fg_selected_and_highlighted").as_string());
		trb.scheme().item_offset = properties.property("item_offset").as_uint();
		trb.scheme().text_offset = properties.property("text_offset").as_uint();
		trb.scheme().icon_size = properties.property("icon_size").as_uint();
		trb.scheme().crook_size = properties.property("crook_size").as_uint();
		trb.scheme().indent_displacement = properties.property("indent_displacement").as_uint();
		*/
	}


	void treebox::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/treebox.hpp>");
		// declaration
		cd->decl.push_back("nana::treebox " + name + ";");
		// init
		if(properties.property("checkable").as_bool())
			cd->init.push_back(name + ".checkable(true);");
		if(properties.property("use_entire_line").as_bool())
			cd->init.push_back(name + ".use_entire_line(true);");

		//scheme
		if(properties.property("item_bg_selected").as_string() != nana::to_string(def_scheme.item_bg_selected))
			cd->init.push_back(name + ".scheme().item_bg_selected = nana::color(" + properties.property("item_bg_selected").as_string() + ");");
		if(properties.property("item_fg_selected").as_string() != nana::to_string(def_scheme.item_fg_selected))
			cd->init.push_back(name + ".scheme().item_fg_selected = nana::color(" + properties.property("item_fg_selected").as_string() + ");");
		if(properties.property("item_bg_highlighted").as_string() != nana::to_string(def_scheme.item_bg_highlighted))
			cd->init.push_back(name + ".scheme().item_bg_highlighted = nana::color(" + properties.property("item_bg_highlighted").as_string() + ");");
		if(properties.property("item_fg_highlighted").as_string() != nana::to_string(def_scheme.item_fg_highlighted))
			cd->init.push_back(name + ".scheme().item_fg_highlighted = nana::color(" + properties.property("item_fg_highlighted").as_string() + ");");
		if(properties.property("item_bg_selected_and_highlighted").as_string() != nana::to_string(def_scheme.item_bg_selected_and_highlighted))
			cd->init.push_back(name + ".scheme().item_bg_selected_and_highlighted = nana::color(" + properties.property("item_bg_selected_and_highlighted").as_string() + ");");
		if(properties.property("item_fg_selected_and_highlighted").as_string() != nana::to_string(def_scheme.item_fg_selected_and_highlighted))
			cd->init.push_back(name + ".scheme().item_fg_selected_and_highlighted = nana::color(" + properties.property("item_fg_selected_and_highlighted").as_string() + ");");
		if(properties.property("item_offset").as_uint() != def_scheme.item_offset)
			cd->init.push_back(name + ".scheme().item_offset = " + properties.property("item_offset").as_string() + ";");
		if(properties.property("text_offset").as_uint() != def_scheme.text_offset)
			cd->init.push_back(name + ".scheme().text_offset = " + properties.property("text_offset").as_string() + ";");
		if(properties.property("icon_size").as_uint() != def_scheme.icon_size)
			cd->init.push_back(name + ".scheme().icon_size = " + properties.property("icon_size").as_string() + ";");
		if(properties.property("crook_size").as_uint() != def_scheme.crook_size)
			cd->init.push_back(name + ".scheme().crook_size = " + properties.property("crook_size").as_string() + ";");
		if(properties.property("indent_displacement").as_uint() != def_scheme.indent_displacement)
			cd->init.push_back(name + ".scheme().indent_displacement = " + properties.property("indent_displacement").as_string() + ";");
	}

}//end namespace ctrls
