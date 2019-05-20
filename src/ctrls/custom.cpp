/*
 *		ctrls::custom Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/custom.h"
#include "filemanager.h"
#include "style.h"



namespace ctrls
{

	//custom
	custom::custom(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		cst.create(*parent->nanawdg);

		nana::drawing dw{ cst };
		dw.draw([this](nana::paint::graphics & graph)
			{
				std::string text = properties.property("class_name").as_string().empty() ? "Custom ctrl" : properties.property("class_name").as_string();
				nana::size ts = graph.text_extent_size(text);
				int x = graph.size().width, y = graph.size().height;
				graph.string(nana::point{ (x - static_cast<int>(ts.width)) / 2, (y - static_cast<int>(ts.height)) / 2 }, text, cst.fgcolor());

				graph.rectangle(false, CUSTOM_COL);
			});

		// c++ code
		properties.append("include").label("Include file").category(CAT_CPPCODE).type(pg_type::string);
		properties.append("include_style").label("Use for include").category(CAT_CPPCODE).type(pg_type::include_style) = 0; // CITEM_INCLUDE_1
		properties.append("class_name").label("Class name").category(CAT_CPPCODE).type(pg_type::string);
		properties.append("create").label("Creating code").category(CAT_CPPCODE).type(pg_type::string) = "$(THIS).create($(PARENT));";
		properties.append("extra_code").label("Extra code").category(CAT_CPPCODE).type(pg_type::string);

		ctrl::init(&cst, CTRL_CUSTOM, name);

		// common
		// ...
		// appearance
		// ...
		// layout
		// ...
	}


	void custom::update()
	{
		ctrl::update();
	}


	void custom::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		///ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// create
		cd->init.push_back("// " + name);
		cd->init.push_back(parse_code(properties.property("create").as_string(), name, ci->create));
		// placement
		cd->init.push_back(ci->place + "[\"" + ci->field + "\"] << " + name + ";");
		// headers
		std::string hpp = properties.property("include").as_string();
		if(properties.property("include_style").as_int() == 0) // CITEM_INCLUDE_1
			hpp = "#include <" + hpp + ">";
		else
			hpp = "#include \"" + hpp + "\"";
		cd->hpps.add(hpp);
		// declaration
		cd->decl.push_back(properties.property("class_name").as_string() + " " + name + ";");
		// init
		if(!properties.property("enabled").as_bool())
			cd->init.push_back(name + ".enabled(false);");
		generatecode_colors(cd, ci, name);
		generatecode_fonts(cd, name);

		if(!properties.property("extra_code").as_string().empty())
			cd->init.push_back(parse_code(properties.property("extra_code").as_string(), name, ci->create));
	}


	void findAndReplaceAll(std::string& data, const std::string& toSearch, const std::string& replaceStr)
	{
		// Get the first occurrence
		size_t pos = data.find(toSearch);

		// Repeat till end is reached
		while(pos != std::string::npos)
		{
			// Replace this occurrence of Sub String
			data.replace(pos, toSearch.size(), replaceStr);
			// Get the next occurrence from the current position
			pos = data.find(toSearch, pos + replaceStr.size());
		}
	}


	std::string custom::parse_code(const std::string& code, const std::string& name, const std::string& parent)
	{
		auto cc = code;
		findAndReplaceAll(cc, "$(THIS)", name);
		findAndReplaceAll(cc, "$(PARENT)", parent);
		return cc;
	}

}//end namespace ctrls
