/*
 *		ctrls::layout Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include <algorithm>
#include "ctrls/layout.h"



namespace nana
{
	namespace drawerbase
	{
		namespace layout
		{
			//class drawer
			void drawer::attached(widget_reference wdg, graph_reference)
			{
				wdg.caption(CTRL_LAYOUT);
				window_ = wdg.handle();

				nana::API::ignore_mouse_focus(wdg, false);

				nana::window wd = wdg;
				nana::API::effects_edge_nimbus(wd, nana::effects::edge_nimbus::active);
			}

			void drawer::refresh(graph_reference graph)
			{
				graph.rectangle(true, nana::API::bgcolor(window_));
				graph.rectangle(false, nana::colors::red);
			}

		}//end namespace layout
	}//end namespace drawerbase


	//layout
	layout::layout(nana::window wd, const nana::rectangle& r, bool visible)
	{
		create(wd, r, visible);
	}


	bool layout::create(nana::window wd, const nana::rectangle& r, bool visible)
	{
		if(!nana::widget_object<typename nana::category::widget_tag, drawerbase::layout::drawer>::create(wd, r, visible))
			return false;
		bgcolor(nana::API::bgcolor(wd));

		return true;
	}

}//end namespace nana


namespace ctrls
{

	//layout
	layout::layout(nana::window wd, const std::string& name)
		: ctrl()
	{
		lyt.create(wd);
		boxmodel.bind(lyt);

		ctrl::init(&lyt, CTRL_LAYOUT, name);

		properties.clear();

		// properties - main
		properties.append("type") = CTRL_LAYOUT;
		properties.append("name") = name;
		// common
		// ...
		// appearance
		// ...
		// layout
		properties.append("layout").label("Layout").category(CAT_LAYOUT).type(pg_type::layout) = static_cast<int>(layout_orientation::horizontal);
		properties.append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties.append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
		properties.append("padding").label("Padding").category(CAT_LAYOUT).type(pg_type::string_uint) = 5;
	}


	void layout::update()
	{
		//ctrl::update();

		boxmodel.orientation(static_cast<layout_orientation>(properties.property("layout").as_int()));
		boxmodel.padding(properties.property("padding").as_int());
		boxmodel.update();
	}


	void layout::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);

		auto name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/place.hpp>");
		if (!ci->place.empty())
			cd->hpps.add("#include <nana/gui/widgets/panel.hpp>");
		// declaration
		if(!ci->place.empty())
		{
			//if parent is not a panel panel (only panel has empty place) add a transparent panel
			cd->decl.push_back("nana::panel<false> " + name + "_panel;");
		}
		cd->decl.push_back("nana::place " + name + "_place;");
		// init
		cd->init.push_back("// " + name);
		if(!ci->place.empty())
		{
			cd->init.push_back(name + "_panel.create(" + ci->create + ");");
			cd->init.push_back(name + "_place.bind(" + name + "_panel);");
		}
		else
		{
			cd->init.push_back(name + "_place.bind(" + ci->create + ");");
		}
		cd->init.push_back(name + "_place.div(\"" + boxmodel.getdiv() + "\");");
		// placement
		if (!ci->place.empty())
			cd->init.push_back(ci->place + "[\"field" + std::to_string(ci->field) + "\"] << " + name + "_panel;");
		// collocate
		if (ci->place.empty())
			cd->init_post.push_back(name + "_place.collocate();");
		// children
		if (!ci->place.empty())
			ci->create = name + "_panel";
		ci->place = name + "_place";
	}


	void layout::updatefield(nana::window ctrl, const std::string& weight, const std::string& margin)
	{
		boxmodel.updatefield(ctrl, weight, margin);
	}


	bool layout::children()
	{
		return boxmodel.children();
	}


	bool layout::append(nana::window ctrl)
	{
		return boxmodel.append(ctrl);
	}
	

	bool layout::insert(nana::window pos, nana::window ctrl, bool after)
	{
		return boxmodel.insert(pos, ctrl, after);
	}


	bool layout::remove(nana::window ctrl)
	{
		return boxmodel.remove(ctrl);
	}


	bool layout::moveup(nana::window ctrl)
	{
		return boxmodel.moveup(ctrl);
	}


	bool layout::movedown(nana::window ctrl)
	{
		return boxmodel.movedown(ctrl);
	}

}//end namespace ctrls
