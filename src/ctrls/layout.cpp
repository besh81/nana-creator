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
				//graph.frame_rectangle(rectangle(graph.size()), colors::blue, colors::red, colors::red, colors::red);
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

		_place.bind(*this);
		_place.collocate();
		return true;
	}


	void layout::update()
	{
		auto pw = nana::API::get_widget(parent());
		bgcolor(pw->bgcolor());
		fgcolor(pw->fgcolor());

		_place.div(getdiv().c_str());
		_place.collocate();
	}


	void layout::orientation(layout_orientation orientation)
	{
		_orientation_str = (orientation == layout_orientation::vertical) ? "vert " : "";
	}


	void layout::padding(int pixels)
	{
		// [css boxmodel]padding -> [nana]margin
		_padding_str = "margin=" + std::to_string(pixels) + " ";
	}


	void layout::updatefield(nana::window child, const std::string& weight, const std::string& margin)
	{
		for(auto& f : _children)
		{
			if(f.second == child)
			{
				f.first.weight = weight;
				f.first.margin = margin;

				_place.div(getdiv().c_str());
				_place.collocate();
				return;
			}
		}
	}


	bool layout::append(nana::window child)
	{
		field f;
		f.name = _name_mgr.add_numbered("field");

		_children.push_back(std::pair<field, nana::window>(f, child));

		_place.div(getdiv().c_str());
		_place.field(f.name.c_str()) << child;
		return true;
	}


	bool layout::remove(nana::window child)
	{
		_place.erase(child);

		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(i->second == child)
			{
				_name_mgr.remove(i->first.name);
				_children.erase(i);

				_place.div(getdiv().c_str());
				_place.collocate();
				return false;
			}
		}

		_place.collocate();
		return false;
	}

	bool layout::moveup(nana::window child)
	{
		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(i->second == child)
			{
				if(i == _children.begin())
					return false;

				std::iter_swap(i, i-1);

				_place.div(getdiv().c_str());
				_place.collocate();
				return false;
			}
		}

		return false;
	}

	bool layout::movedown(nana::window child)
	{
		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(i->second == child)
			{
				if(i + 1 == _children.end())
					return false;

				std::iter_swap(i, i + 1);

				_place.div(getdiv().c_str());
				_place.collocate();
				return false;
			}
		}

		return false;
	}


	std::string layout::getdiv()
	{
		std::string div;
		div.append(_orientation_str);
		div.append(_padding_str);
		//
		for(auto f : _children)
		{
			div.append("<");
			if(!f.first.weight.empty())
				div.append("weight=" + f.first.weight + " ");
			if(!f.first.margin.empty())
				div.append("margin=" + f.first.margin + " ");
			div.append(f.first.name);
			div.append(">");
		}

		return div;
	}

}//end namespace nana


namespace ctrls
{

	//layout
	layout::layout(nana::window wd, const std::string& name)
		: ctrl()
	{
		lyt.create(wd);
		ctrl::init(&lyt, CTRL_LAYOUT, name);

		properties.clear();

		// properties - main
		properties.append("type") = CTRL_LAYOUT;
		properties.append("name") = name;
		// common
		properties.append("layout").label("Layout").category(CAT_COMMON).type(pg_type::layout) = static_cast<int>(nana::layout_orientation::horizontal);
		// appearance
		// ...
		// layout
		properties.append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties.append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
		properties.append("padding").label("Padding").category(CAT_LAYOUT).type(pg_type::string_uint) = 5;
	}


	void layout::update()
	{
		//ctrl::update();

		lyt.orientation(static_cast<nana::layout_orientation>(properties.property("layout").as_int()));
		lyt.padding(properties.property("padding").as_int());
		lyt.update();
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
		cd->init.push_back(name + "_place.div(\"" + lyt.getdiv() + "\");");
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


	void layout::updatefield(nana::window child, const std::string& weight, const std::string& margin)
	{
		lyt.updatefield(child, weight, margin);
	}


	bool layout::append(nana::window child)
	{
		return lyt.append(child);
	}


	bool layout::remove(nana::window child)
	{
		return lyt.remove(child);
	}


	bool layout::moveup(nana::window child)
	{
		return lyt.moveup(child);
	}


	bool layout::movedown(nana::window child)
	{
		return lyt.movedown(child);
	}

}//end namespace ctrls
