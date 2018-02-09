/*
 *		resizablecanvas Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include <algorithm>
#include "resizablecanvas.h"
#include <nana/gui/widgets/scroll.hpp>
#include "style.h"


namespace drawerbase
{
	namespace resizablecanvas
	{
		struct essence_t
		{
			::nana::paint::graphics*	graph{ 0 };
			::resizablecanvas*			widget{ 0 };

			struct scroll_part
			{
				static const unsigned scale = 16;

				::nana::point pos;

				::nana::scroll<true> v;
				::nana::scroll<false> h;
			} scroll;


			::nana::size available_area() const
			{
				return { graph->size().width - 2, graph->size().height - 2 };
			}

			::nana::size child_area() const
			{
				if(widget->haschild())
					return{ widget->child()->size().width + 10, widget->child()->size().height + 10 };

				return {};
			}


			void adjust_scroll_value()
			{
				//The area of the widget
				const auto wd_sz = child_area();
				//The area to show the widget
				auto graph_sz = available_area();

				if(scroll.h.empty() == false)
				{
					const auto ext_px = scroll.v.empty() ? 0 : scroll.scale;
					if(ext_px >= graph_sz.width)
						return;

					const unsigned width = graph_sz.width - (scroll.v.empty() ? 0 : scroll.scale);

					if(scroll.pos.x + wd_sz.width < width)
					{
						scroll.pos.x = static_cast<int>(width) - static_cast<int>(wd_sz.width);
						if(scroll.pos.x > 0)
							scroll.pos.x = 0;
					}

					scroll.h.amount(wd_sz.width);
					scroll.h.range(width);
					scroll.h.value(-scroll.pos.x);
					scroll.h.step(graph->text_extent_size(L"W").width); //todo ?
				}
				else
					scroll.pos.x = 0;

				if(scroll.v.empty() == false)
				{
					const auto ext_px = scroll.h.empty() ? 0 : scroll.scale;
					if(ext_px >= graph_sz.height)
						return;

					const unsigned height = graph_sz.height - (scroll.h.empty() ? 0 : scroll.scale);

					if(scroll.pos.y + wd_sz.height < height)
					{
						scroll.pos.y = static_cast<int>(height) - static_cast<int>(wd_sz.height);
						if(scroll.pos.y > 0)
							scroll.pos.y = 0;
					}

					scroll.v.amount(wd_sz.height);
					scroll.v.range(height);
					scroll.v.value(-scroll.pos.y);
					scroll.v.step(graph->text_extent_size(L"W").height); //todo ?
				}
				else
					scroll.pos.y = 0;
			}

			void adjust_scroll_life()
			{
				::nana::internal_scope_guard lock;

				//The area of the widget
				const auto wd_sz = child_area();
				//The area to show the widget
				auto graph_sz = available_area();

				bool h = false, v = false;

				if(graph_sz.height < wd_sz.height)
				{
					v = true;
					graph_sz.width -= scroll.scale;
					if(graph_sz.width < wd_sz.width)
					{
						h = true;
						graph_sz.height -= scroll.scale;
					}
				}
				else if(graph_sz.width < wd_sz.width)
				{
					h = true;
					graph_sz.height -= scroll.scale;
					if(graph_sz.height < wd_sz.height)
					{
						v = true;
						graph_sz.width -= scroll.scale;
					}
				}

				if(h)
				{
					::nana::rectangle r(1, graph_sz.height + 1, graph_sz.width, scroll.scale);
					if(scroll.h.empty())
					{
						scroll.h.create(*widget, r);
						::nana::API::take_active(scroll.h.handle(), false, *widget);

						scroll.h.events().value_changed.connect_unignorable([this](const ::nana::arg_scroll& arg)
						{
							scroll.pos.x = -static_cast<int>(scroll.h.value());
							::nana::API::refresh_window(*widget);
						});
					}
					else
						scroll.h.move(r);
				}
				else if(!scroll.h.empty())
					scroll.h.close();

				if(v)
				{
					::nana::rectangle r(graph_sz.width + 1, 1, scroll.scale, graph_sz.height);
					if(scroll.v.empty())
					{
						scroll.v.create(*widget, r);
						::nana::API::take_active(scroll.v.handle(), false, *widget);

						scroll.v.events().value_changed.connect_unignorable([this](const ::nana::arg_scroll& arg)
						{
							scroll.pos.y = -static_cast<int>(scroll.v.value());
							::nana::API::refresh_window(*widget);
						});
					}
					else
						scroll.v.move(r);

				}
				else if(!scroll.v.empty())
					scroll.v.close();

				adjust_scroll_value();
			}
		};

		//class trigger
		trigger::trigger()
			: essence_(new essence_t)
		{}
		trigger::~trigger()
		{
			delete essence_;
		}

		essence_t& trigger::essence() const
		{
			return *essence_;
		}

		void trigger::attached(widget_reference wdg, graph_reference graph)
		{
			wdg.caption(CTRL_LAYOUT);
			window_ = wdg.handle();

			::nana::API::ignore_mouse_focus(wdg, true);

			::nana::window wd = wdg;
			::nana::API::effects_edge_nimbus(wdg, nana::effects::edge_nimbus::none);

			essence_->graph = &graph;
			essence_->widget = dynamic_cast<::resizablecanvas*>(&wdg);
		}

		void trigger::detached()
		{
			essence_->graph = nullptr;
		}

		void trigger::refresh(graph_reference graph)
		{
			graph.rectangle(true, nana::API::bgcolor(window_));

			if(essence_->widget->haschild())
				essence_->widget->child()->move(essence_->scroll.pos.x + 5, essence_->scroll.pos.y + 5);

			_m_draw_border();
		}

		void trigger::resized(graph_reference graph, const ::nana::arg_resized&)
		{
			essence_->adjust_scroll_life();
			refresh(graph);
			::nana::API::dev::lazy_refresh();
		}

		void trigger::_m_draw_border()
		{
			if(::nana::API::widget_borderless(window_))
				return;

			auto & graph = *essence_->graph;
			auto size = graph.size();
			//Draw Border
			graph.rectangle(false, static_cast<::nana::color_rgb>(0x9cb6c5));
				
			// Area betwen the scroll bars
			if((essence_->scroll.h.empty() == false) && (essence_->scroll.v.empty() == false))
				graph.rectangle({ static_cast<int>(size.width - 1 - essence_->scroll.scale),
					static_cast<int>(size.height - 1 - essence_->scroll.scale),
					essence_->scroll.scale,
					essence_->scroll.scale },
					true, ::nana::colors::button_face);
		}

	}//end namespace resizablecanvas
}//end namespace drawerbase


//resizablecanvas
resizablecanvas::resizablecanvas(nana::window wd)
{
	create(wd, nana::rectangle());

	bgcolor(CREATOR_MAIN_BG);
}

bool resizablecanvas::add(nana::window child)
{
	if(haschild())
		return false; // already has a child

	_child = nana::API::get_widget(child);
	_child->move(nana::rectangle(5, 5, MAIN_WDG_W, MAIN_WDG_H));

	return true;
}

bool resizablecanvas::remove(nana::window child)
{
	if(!haschild())
		return false; // no child to remove

	_child->move(nana::rectangle(0, 0, 5, 5));
	_child = 0;
	return true;
}

::nana::widget* resizablecanvas::child()
{
	return _child;
}

drawerbase::resizablecanvas::essence_t& resizablecanvas::_m_ess() const
{
	return get_drawer_trigger().essence();
}
