/*
 *		ctrls::combox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/combox.h"
#include "filemanager.h"

#define DEF_IMG_SIZE	16 // get from nana source


extern filemanager		g_file_mgr;


namespace ctrls
{

	//combox
	combox::combox(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		cmb.create(*parent->nanawdg);
		ctrl::init(&cmb, CTRL_COMBOX, name);

		// common
		properties.append("options").label("Options").category(CAT_COMMON).type(pg_type::collection_combox) = "";
		properties.append("option").label("Option").category(CAT_COMMON).type(pg_type::string_uint) = 0;
		properties.append("editable").label("Editable").category(CAT_COMMON).type(pg_type::check) = cmb.editable();
		// appearance
		properties.append("image_pixels").label("Image pixels").category(CAT_APPEARANCE).type(pg_type::string_uint) = DEF_IMG_SIZE;
		// layout
		// ...
	}


	void combox::init_item(properties_collection& item)
	{
		ctrl::init_item(item);
		item.property("type") = "option";
		//
		item.append("text").label("Text").category(CAT_COMMON).type(pg_type::string) = "New Item";
		item.append("image").label("Image").category(CAT_COMMON).type(pg_type::image) = "";
	}


	void combox::update()
	{
		ctrl::update();

		//options: I don't think it's usefull to set options here

		cmb.editable(properties.property("editable").as_bool());
		cmb.image_pixels(properties.property("image_pixels").as_uint());
	}


	void combox::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/combox.hpp>");
		// declaration
		cd->decl.push_back("nana::combox " + name + ";");
		// init
		
		// options - START
		items.for_each([cd, &name](tree_node<ctrls::properties_collection>* node) -> bool
			{
				cd->init.push_back(name + ".push_back(\"" + node->value.property("text").as_escaped_string() + "\");");

				if(!node->value.property("image").as_string().empty())
					cd->init.push_back(name + ".image(" + std::to_string(node->pos()) + ", nana::paint::image(\"" + g_file_mgr.to_relative(node->value.property("image").as_string()) + "\"));");

				return true;
			});
		// options - END

		cd->init.push_back(name + ".option(" + properties.property("option").as_string() + ");");
		if(properties.property("editable").as_bool())
			cd->init.push_back(name + ".editable(true);");
		if(properties.property("image_pixels").as_uint() != DEF_IMG_SIZE)
			cd->init.push_back(name + ".image_pixels(" + properties.property("image_pixels").as_string() + ");");
	}

}//end namespace ctrls
