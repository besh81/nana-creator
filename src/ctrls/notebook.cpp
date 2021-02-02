/*
 *		ctrls::notebook Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/notebook.h"
#include "nana_extra/color_helper.h"


#define DEF_TABS_WEIGHT		"30"


namespace ctrls
{

	//notebook
	notebook::notebook(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		pnl.create(*parent->nanawdg);
		plc.bind(pnl);
		plc.div("vert margin=1 <vert weight=" DEF_TABS_WEIGHT " tabs><pages>");

		tbb.create(pnl);
		plc["tabs"] << tbb;


		ctrl::init(&pnl, CTRL_NOTEBOOK, name);

		// tabbar events
		//---------------
		// mouse move
		tbb.events().mouse_move.connect_front([this](const nana::arg_mouse& arg)
		{
			if(_highlighted == highlight_mode::before_after)
			{
				auto pos = mouse_pos(arg.pos, nanawdg->size());
				if(pos != _cursor_pos)
				{
					_cursor_pos = pos;
					refresh();
				}
			}
		});

		// drawing
		nana::drawing dw{ tbb };
		dw.draw([this](nana::paint::graphics& graph)
		{
			if(highlighted())
				draw_highlight(graph, _highlighted, _cursor_pos);
		});


		// common
		// ...
		// appearance
		properties.append("addbtn").label("Add button").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("closebtn").label("Close button").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("closefly").label("Close fly").category(CAT_APPEARANCE).type(pg_type::check).enabled("closebtn", true) = false;
		properties.append("listbtn").label("List button").category(CAT_APPEARANCE).type(pg_type::check) = false;
		properties.append("scrollbtn").label("Scroll button").category(CAT_APPEARANCE).type(pg_type::check) = true;
		properties.append("tabweight").label("Tabs weight").category(CAT_APPEARANCE).type(pg_type::string_weight) = DEF_TABS_WEIGHT;
		// layout
		// ...


		// events
		//----------
		// avoid use of add button
		tbb.events().adding([this](const nana::arg_tabbar_adding<size_t> & arg)
		{
			if(!internal_use)
			{
				arg.add = false;
				arg.stop_propagation();
			}
		});
		// avoid use of close button
		tbb.events().removed([this](const nana::arg_tabbar_removed<size_t> & arg)
		{
			if(!internal_use)
			{
				arg.remove = false;
				arg.stop_propagation();
			}
		});
		// release the mutex
		tbb.events().mouse_up.connect_front([this](const nana::arg_mouse& arg)
		{
			mouse_mtx.try_lock();
			mouse_mtx.unlock();
		});
	}


	bool notebook::append(ctrl* child)
	{
		if(!child)
			return false;

		pages.push_back(child);

		tbb.push_back(child->properties.property("caption").as_string());
		tbb.attach(tbb.length() - 1, *child->nanawdg);

		plc["pages"].fasten(*child->nanawdg);
		plc.collocate();
		return true;
	}


	bool notebook::insert(ctrl* child, ctrl* pos, bool after)
	{
		if(!child)
			return false;

		int i = 0;
		for(auto p = pages.begin(); p < pages.end(); ++p, ++i)
		{
			if(*p == pos)
			{
				if(after)
				{
					++p;
					++i;
				}

				pages.insert(p, child);

				tbb.insert(i, child->properties.property("caption").as_string());
				tbb.attach(i, *child->nanawdg);

				plc["pages"].fasten(*child->nanawdg);
				plc.collocate();
				return true;
			}
		}

		return false;
	}


	bool notebook::remove(ctrl* child)
	{
		if(!child)
			return false;

		plc.erase(*child->nanawdg);

		int i = 0;
		for(auto p = pages.begin(); p < pages.end(); ++p, ++i)
		{
			if(*p == child)
			{
				pages.erase(p);

				internal_use = true;
				tbb.erase(i);
				internal_use = false;

				plc.collocate();
				return true;
			}
		}

		plc.collocate();
		return false;
	}


	bool notebook::moveup(ctrl* child)
	{
		if(!child)
			return false;

		int i = 0;
		for(auto p = pages.begin(); p < pages.end(); ++p, ++i)
		{
			if(*p == child)
			{
				if(p == pages.begin())
					return false;

				std::iter_swap(p, p - 1);

				tbb.attach(i, *(*p)->nanawdg);
				tbb.attach(i - 1, *(*(p - 1))->nanawdg);
				tbb.activated(i - 1);
				update();

				plc.collocate();
				return true;
			}
		}

		return false;
	}


	bool notebook::movedown(ctrl* child)
	{
		if(!child)
			return false;

		int i = 0;
		for(auto p = pages.begin(); p < pages.end(); ++p, ++i)
		{
			if(*p == child)
			{
				if(p + 1 == pages.end())
					return false;

				std::iter_swap(p, p + 1);

				tbb.attach(i, *(*p)->nanawdg);
				tbb.attach(i + 1, *(*(p + 1))->nanawdg);
				tbb.activated(i + 1);
				update();

				plc.collocate();
				return true;
			}
		}

		return false;
	}


	ctrl* notebook::get_page(size_t pos)
	{
		if(pos >= pages.size())
			return 0;

		return pages[pos];
	}


	void notebook::show_page(ctrl* page)
	{
		for(std::size_t i = 0; i < pages.size(); i++)
		{
			if(page == pages[i])
			{
				if(!pages[i]->nanawdg->visible())
					tbb.activated(i);
				return;
			}
		}
	}


	void notebook::update()
	{
		ctrl::update();

		// tbb
		tbb.bgcolor(pnl.bgcolor());
		tbb.fgcolor(pnl.fgcolor());

		int i = 0;
		for(auto p : pages)
		{
			tbb.text(i, p->properties.property("caption").as_string());

			tbb.tab_image(i, nana::paint::image(p->properties.property("image").as_string()));

			bool inherited;
			auto col = nana::to_color(p->properties.property("bgcolor").as_string(), inherited);
			tbb.tab_bgcolor(i, inherited ? tbb.bgcolor() : col);

			col = nana::to_color(p->properties.property("fgcolor").as_string(), inherited);
			tbb.tab_fgcolor(i, inherited ? tbb.fgcolor() : col);

			i++;
		}

		tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::add, properties.property("addbtn").as_bool());
		tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::close, properties.property("closebtn").as_bool());
		tbb.close_fly(properties.property("closefly").as_bool());
		tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::list, properties.property("listbtn").as_bool());
		tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::scroll, properties.property("scrollbtn").as_bool());

		// tabs weight
		auto tabw = properties.property("tabweight").as_string();
		if(tabw.empty())
			tabw = DEF_TABS_WEIGHT;
		else if(tabw[0] == '-')
			tabw = DEF_TABS_WEIGHT;
		plc.div("vert margin=1 <vert weight=" + tabw + " tabs><pages>");
		plc.collocate();
	}


	void notebook::refresh()
	{
		nana::API::refresh_window(pnl);
		nana::API::refresh_window(tbb);
	}


	void notebook::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();


		// tabs weight
		auto tabw = properties.property("tabweight").as_string();
		if(tabw.empty())
			tabw = DEF_TABS_WEIGHT;
		else if(tabw[0] == '-')
			tabw = DEF_TABS_WEIGHT;


		// headers
		cd->hpps.add("#include <nana/gui/widgets/panel.hpp>");
		cd->hpps.add("#include <nana/gui/widgets/tabbar.hpp>");
		// declaration
		cd->decl.push_back("nana::panel<true> " + name + "_pnl;");
		cd->decl.push_back("nana::tabbar<size_t> " + name + "_tbb;");
		cd->decl.push_back("nana::place " + name + "_plc;");
		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + "_pnl.create(" + ci->create + ");");
		cd->init.push_back(name + "_tbb.create(" + name + "_pnl);");

		cd->init.push_back(name + "_plc.bind(" + name + "_pnl);");
		cd->init.push_back(name + "_plc.div(\"vert margin=1 <vert weight=" + tabw + " tabs><pages>\");");
		cd->init.push_back(name + "_plc[\"tabs\"] << " + name + "_tbb;");
		
		if(properties.property("addbtn").as_bool())
			cd->init.push_back(name + "_tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::add, true);");
		if(properties.property("closebtn").as_bool())
		{
			cd->init.push_back(name + "_tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::close, true);");
			if(properties.property("closefly").as_bool())
				cd->init.push_back(name + "_tbb.close_fly(true);");
		}
		if(properties.property("listbtn").as_bool())
			cd->init.push_back(name + "_tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::list, true);");
		if(!properties.property("scrollbtn").as_bool()) // Only scrolling button is enabled by default
			cd->init.push_back(name + "_tbb.toolbox(nana::drawerbase::tabbar::trigger::kits::scroll, false);");

		// placement
		cd->init.push_back(ci->place + "[\"" + ci->field + "\"] << " + name + "_pnl;");
		// collocate
		cd->init_post.push_back(name + "_plc.collocate();");
		// children
		ci->create = name;
		ci->place = name + "_plc";
		ci->field = "pages";
	}


	void notebook::connect_mouse_enter(std::function<void(const nana::arg_mouse&)> f)
	{
		pnl.events().mouse_enter.connect_front(f);
		tbb.events().mouse_enter.connect_front(f);
	}


	void notebook::connect_mouse_leave(std::function<void(const nana::arg_mouse&)> f)
	{
		pnl.events().mouse_leave.connect_front(f);
		tbb.events().mouse_leave.connect_front(f);
	}


	void notebook::connect_mouse_down(std::function<void(const nana::arg_mouse&)> f)
	{
		pnl.events().mouse_down.connect_front(f);

		tbb.events().mouse_down.connect_front([this, f](const nana::arg_mouse& arg)
		{
			if(mouse_mtx.try_lock())
				f(arg);

			mouse_mtx.unlock();
		});
	}


	void notebook::connect_tab_click(std::function<void(const nana::arg_tabbar_mouse<size_t>&)> f)
	{
		tbb.events().tab_click([this, f](const nana::arg_tabbar_mouse<size_t> & arg)
		{
			if(mouse_mtx.try_lock())
				f(arg);
		});
	}

}//end namespace ctrls
