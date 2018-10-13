/*
 *		nana::propertygrid Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include "propgrid.h"
#include <nana/gui/widgets/scroll.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/layout_utility.hpp>
#include <nana/gui/element.hpp>
#include <list>
#include <stdexcept>
#include <algorithm>
#include <cassert>


//#define		ENABLE_H_SCROLL


namespace nana
{
	namespace drawerbase
	{
		namespace propertygrid
		{
			struct essence_t;

			/// Essence of the columns
			class es_columns
			{
			public:
				void set_min_width(unsigned col, unsigned width)
				{
					min_width_[col] = width;
				}

				unsigned get_min_width(unsigned col) const
				{
					return min_width_[col];
				}

				std::size_t count() const
				{
					return 2;
				}

				unsigned col_width_adj(unsigned col, unsigned width) const
				{
					unsigned adj = 0;
					if(width > width_())
						adj = (width - width_()) / 2;

					// labels
					if(col == 0)
						return min_width_[0] + adj;
					// values
					if(width > width_())
						return width - (min_width_[0] + adj);
					return min_width_[1];
				}

			private:
				unsigned width_() const
				{
					return min_width_[0] + min_width_[1];
				}

				unsigned min_width_[2]{ 100, 100 };
			};


			class inline_pane
				: public nana::panel<false>
			{
			public:
				inline_pane(nana::window wd)
					: nana::panel<false>(wd)
				{
					inner_ = std::unique_ptr<nana::panel<false>>(new nana::panel<false>);
					inner_->create(*this);
				}

				void adjust_position(const nana::rectangle& r, const nana::rectangle& inner)
				{
					move(r);
					inner_->move(inner);
				}

				nana::window wd_ptr() const
				{
					return inner_->handle();
				}


			protected:
				std::unique_ptr<nana::panel<false>> inner_;
			};


			struct category_t
			{
				std::string text;

				std::vector<pgitem_ptr> items;
				std::vector<std::unique_ptr<inline_pane>>	inline_panes;

				bool expand{ true };

				category_t() = default;

				category_t(std::string str)
					:text(std::move(str))
				{}
			};

			class es_lister
			{
			public:
				es_lister()
				{
					//#0 is a default category
					categories_.emplace_back();
				}

				void bind(essence_t* ess, widget& wd)
				{
					ess_ = ess;
					widget_ = dynamic_cast<nana::propertygrid*>(&wd);
				}

				nana::propertygrid* wd_ptr() const
				{
					return widget_;
				}


				void scroll(const index_pair& pos, bool as_first, bool expand_category = true);

				///Append a new category with a specified name and return a pointer to it.
				category_t* create_cat(std::string&& text)
				{
					categories_.emplace_back(std::move(text));
					return &categories_.back();
				}
				/// add a new cat created at "pos" and return a ref to it
				category_t* create_cat(std::size_t pos, std::string&& text)
				{
					return &(*categories_.emplace(this->get(pos), std::move(text)));
				}

				std::vector<pgitem_ptr>::value_type& at(const index_pair& pos)
				{
					return get(pos.cat)->items.at(pos.item);
				}

				const std::vector<pgitem_ptr>::value_type& at(const index_pair& pos) const
				{
					return get(pos.cat)->items.at(pos.item);
				}

				void clear(std::size_t cat)
				{
					auto& catobj = *get(cat);

					catobj.inline_panes.clear();
					catobj.items.clear();
				}
				/// clear all items in all cat, but not the container of cat self.
				void clear()
				{
					for(auto & m : categories_)
					{
						m.inline_panes.clear();
						m.items.clear();
					}
				}

				pgitem& get_property(category_t* cat, std::size_t pos) const
				{
					if(!cat)
						throw std::out_of_range("nana::propertygrid: category is null");

					return *cat->items.at(pos);
				}

				void erase(const index_pair& pos);

				void erase(std::size_t cat)
				{
					auto i = get(cat);

					//If the category is the first one, it just clears the items instead of removing whole category.
					if(0 == cat)
					{
						i->inline_panes.clear();
						i->items.clear();
					}
					else
						categories_.erase(i);
				}

				void erase()
				{
					//Do not remove the first category.
					auto i = categories_.begin();
					i->inline_panes.clear();
					i->items.clear();
					if(categories_.size() > 1)
						categories_.erase(++i, categories_.end());
				}

				bool expand(std::size_t cat, bool exp)
				{
					if(good(cat))
					{
						auto& expanded = get(cat)->expand;
						if(expanded != exp)
						{
							expanded = exp;
							return true;
						}
					}
					return false;
				}

				bool expand(std::size_t cat) const
				{
					auto ex = get(cat)->expand;
					return (good(cat) ? get(cat)->expand : false);
				}

				std::vector<category_t>& cat_container()
				{
					return categories_;
				}

				const std::vector<category_t>& cat_container() const
				{
					return categories_;
				}

				std::size_t the_height_of_expanded() const;

				std::size_t size_categ() const
				{
					return categories_.size();
				}

				std::size_t size_item(std::size_t cat) const
				{
					return get(cat)->items.size();
				}

				bool good(std::size_t cat) const
				{
					return (cat < categories_.size());
				}

				void enabled(bool state)
				{
					for(auto & c : categories_)
					{
						for(auto & i : c.items)
						{
							i->enabled(state);
						}
					}
				}


				/// categories iterator
				std::vector<category_t>::iterator get(std::size_t pos)
				{
					if(pos >= categories_.size())
						throw std::out_of_range("nana::propertygrid: invalid category index");

					auto i = categories_.begin();
					std::advance(i, pos);
					return i;
				}

				std::vector<category_t>::const_iterator get(std::size_t pos) const
				{
					if(pos >= categories_.size())
						throw std::out_of_range("nana::propertygrid: invalid category index");

					auto i = categories_.cbegin();
					std::advance(i, pos);
					return i;
				}

			private:
				void bind_();

				essence_t * ess_{ nullptr };
				nana::propertygrid * widget_{ nullptr };

				std::vector<category_t> categories_;
			};//end class es_lister


			//struct essence_t
			//@brief:	this struct gives many data for propertygrid,
			//			the state of the struct does not effect on member funcions, therefore all data members are public.
			struct essence_t
			{
				::nana::propertygrid::scheme_type* scheme_ptr{ nullptr };
				::nana::paint::graphics* graph{ nullptr };
				bool auto_draw{ true };
				unsigned cat_size{ 24 };
				unsigned text_height{ 0 };

				es_columns	columns;
				es_lister	lister;

				index_pair	pointer_where;	//indicates the item where the mouse is placed
				

				struct scroll_part
				{
					static const unsigned scale = 16;// need to be abs??? to see the same item after sort() ??
					::nana::point pos;

					::nana::scroll<true> v;
					#ifdef ENABLE_H_SCROLL
					::nana::scroll<false> h;
					#endif
				} scroll;


				essence_t()
				{
				}


				void property_changed(const drawerbase::propertygrid::item_proxy& ip)
				{
					arg_propertygrid arg{ ip };
					lister.wd_ptr()->events().property_changed.emit(arg, lister.wd_ptr()->handle());
				}


				void v_offset(int val, bool is_delta = false)
				{
					if(is_delta)
						scroll.pos.y += val;
					else
						scroll.pos.y = val;

					scroll.pos.y = std::max(std::min(0, scroll.pos.y), static_cast<int>(scroll.v.range()) - static_cast<int>(scroll.v.amount()));
				}
				int v_offset() const
				{
					return scroll.pos.y;
				}
				#ifdef ENABLE_H_SCROLL
				void h_offset(int val, bool is_delta = false)
				{
					if(is_delta)
						scroll.pos.x += val;
					else
						scroll.pos.x = val;

					scroll.pos.x = std::max(std::min(0, scroll.pos.x), static_cast<int>(scroll.h.range()) - static_cast<int>(scroll.h.amount()));
				}
				int h_offset() const
				{
					return scroll.pos.x;
				}
				#endif


				void update()
				{
					if(auto_draw && lister.wd_ptr())
					{
						adjust_scroll_life();
						API::refresh_window(lister.wd_ptr()->handle());
					}
				}


				::nana::rectangle available_area() const
				{
					::nana::rectangle r(::nana::point{ 0, 0 }, graph->size());

					if(!lister.wd_ptr()->borderless())
					{
						r.x = 1;
						r.y = 1;
						r.width -= 2;
						r.height -= 2;
					}

					return r;
				}

				::nana::size items_area()
				{
					return { columns.get_min_width(0) + columns.get_min_width(1), static_cast<unsigned>(lister.the_height_of_expanded()) };
				}


				void adjust_scroll_value()
				{
					//The area of the widget
					const auto wd_sz = items_area();
					//The area to show the widget
					auto graph_r = available_area();

					#ifdef ENABLE_H_SCROLL
					if(scroll.h.empty() == false)
					{
						const auto ext_px = scroll.v.empty() ? 0 : scroll.scale;
						if(ext_px >= graph_r.width)
							return;

						const unsigned width = graph_r.width - (scroll.v.empty() ? 0 : scroll.scale);

						scroll.h.amount(wd_sz.width);
						scroll.h.range(width);
						
						if(h_offset() < static_cast<int>(width) - static_cast<int>(wd_sz.width))
							h_offset(static_cast<int>(width) - static_cast<int>(wd_sz.width));

						scroll.h.value(-h_offset());
						scroll.h.step(graph->text_extent_size(L"W").width); //todo ?
					}
					else
						h_offset(0);
					#endif

					if(scroll.v.empty() == false)
					{
						#ifdef ENABLE_H_SCROLL
						const auto ext_px = scroll.h.empty() ? 0 : scroll.scale;
						const unsigned height = graph_r.height - (scroll.h.empty() ? 0 : scroll.scale);
						#else
						const auto ext_px = 0;
						const unsigned height = graph_r.height;
						#endif

						if(ext_px >= graph_r.height)
							return;

						scroll.v.amount(wd_sz.height);
						scroll.v.range(height);

						if(v_offset() < static_cast<int>(height) - static_cast<int>(wd_sz.height))
							v_offset(static_cast<int>(height) - static_cast<int>(wd_sz.height));

						scroll.v.value(-v_offset());
						scroll.v.step(cat_size);
					}
					else
						v_offset(0);
				}

				void adjust_scroll_life()
				{
					::nana::internal_scope_guard lock;

					window wd = lister.wd_ptr()->handle();
					if(!wd)
						return;

					//The area of the widget
					const auto wd_sz = items_area();
					//The area to show the widget
					auto graph_r = available_area();

					bool h = false, v = false;

					if(graph_r.height < wd_sz.height)
					{
						v = true;
						graph_r.width -= scroll.scale;
						#ifdef ENABLE_H_SCROLL
						if(graph_r.width < wd_sz.width)
						{
							h = true;
							graph_r.height -= scroll.scale;
						}
						#endif
					}
					#ifdef ENABLE_H_SCROLL
					else if(graph_r.width < wd_sz.width)
					{
						h = true;
						graph_r.height -= scroll.scale;
						if(graph_r.height < wd_sz.height)
						{
							v = true;
							graph_r.width -= scroll.scale;
						}
					}
					#endif

					#ifdef ENABLE_H_SCROLL
					if(h)
					{
						::nana::rectangle r(graph_r.x, graph_r.y + graph_r.height, graph_r.width, scroll.scale);
						if(scroll.h.empty())
						{
							scroll.h.create(wd, r);
							::nana::API::take_active(scroll.h.handle(), false, wd);

							scroll.h.events().value_changed.connect_unignorable([this](const ::nana::arg_scroll& arg)
							{
								h_offset(-static_cast<int>(scroll.h.value()));
								::nana::API::refresh_window(lister.wd_ptr()->handle());
							});
						}
						else
							scroll.h.move(r);
					}
					else if(!scroll.h.empty())
						scroll.h.close();
					#endif

					if(v)
					{
						::nana::rectangle r(graph_r.x + graph_r.width, graph_r.y, scroll.scale, graph_r.height);
						if(scroll.v.empty())
						{
							scroll.v.create(wd, r);
							::nana::API::take_active(scroll.v.handle(), false, wd);

							scroll.v.events().value_changed.connect_unignorable([this](const ::nana::arg_scroll& arg)
							{
								v_offset(-static_cast<int>(scroll.v.value()));
								::nana::API::refresh_window(lister.wd_ptr()->handle());
							});
						}
						else
							scroll.v.move(r);

					}
					else if(!scroll.v.empty())
						scroll.v.close();

					adjust_scroll_value();
				}

				void set_auto_draw(bool ad)
				{
					if(auto_draw != ad)
					{
						auto_draw = ad;
						if(ad)
						{
							adjust_scroll_life();
							API::refresh_window(lister.wd_ptr()->handle());
						}
					}
				}

				bool mouse_moved(::nana::point pos)
				{
					index_pair new_where{ npos, npos };

					nana::rectangle rect;
					rect_lister(rect);

					if(rect.x < pos.x && pos.x < static_cast<int>(rect.width) && rect.y < pos.y && pos.y < static_cast<int>(rect.height))
					{
						auto y = v_offset();

						unsigned cat = 0;
						for(auto i_categ = lister.get(0); i_categ != lister.cat_container().end(); ++i_categ, ++cat)
						{
							if(cat != 0)
							{
								y += cat_size;

								if(y > 0 && pos.y <= rect.y + y)
								{
									new_where = { cat, npos };
									break;
								}

								if(!i_categ->expand)
									continue;
							}
							

							// items
							auto size = i_categ->items.size();
							for(decltype(size) item = 0; item < size; ++item)
							{
								y += i_categ->items[item]->size();

								if(y > 0 && pos.y <= rect.y + y)
								{
									new_where = { cat, item };
									break;
								}
							}

							if(!new_where.empty())
								break;
						}
					}

					if(new_where == pointer_where)
						return false;

					pointer_where = new_where;
					return true;
				}

				bool rect_lister(nana::rectangle& r) const
				{
					//The area to show the widget
					r = available_area();

					unsigned width = (scroll.v.empty() ? 0 : scroll.scale);
					#ifdef ENABLE_H_SCROLL
					unsigned height = (scroll.h.empty() ? 0 : scroll.scale);
					#else
					unsigned height = 0;
					#endif

					if(r.width <= width || r.height <= height)
						return false;

					r.width -= width;
					r.height -= height;
					return true;
				}

				bool wheel(bool upwards)
				{
					if(scroll.v.empty() || !scroll.v.scrollable(upwards))
						return false;

					v_offset(upwards ? static_cast<int>(scroll.v.step()) : -static_cast<int>(scroll.v.step()), true);
					return true;
				}

				unsigned auto_width(unsigned col, unsigned max = 3000) /// \todo introduce parametr max_header_width
				{
					unsigned max_w{ 0 };
					for(const auto &cat : lister.cat_container())
						for(const auto &it : cat.items)
						{
							// precalcule text geometry
							unsigned ts;
							if(col == 0)
								ts = graph->text_extent_size(it->label()).width;
							else
								ts = graph->text_extent_size(it->value()).width;
							if(max_w < ts)
								max_w = ts;
						}
					if(!max_w) return 0;

					unsigned ext_w = scheme_ptr->ext_w;
					columns.set_min_width(col, max_w + ext_w + 1 < max ? max_w + ext_w + 1 : max);
					return max_w;
				}
			};


			/// class pgitem
			void pgitem::label(const std::string& label)
			{
				label_ = label;
			}
			std::string pgitem::label() const
			{
				return label_;
			}
			void pgitem::value(const std::string& value)
			{
				value_ = value;
			}
			std::string pgitem::value() const
			{
				return value_;
			}

			unsigned pgitem::size() const
			{
				return size_;
			}

			void pgitem::draw(paint::graphics* graph, rectangle label_r, rectangle value_r, const int txtoff, color bgcolor, color fgcolor) const
			{
				// draw the label
				// background
				graph->rectangle(label_r, true, bgcolor);
				// text
				if(label_r.width > 5)
				{
					int content_pos = 20;
					nana::size ts = graph->text_extent_size(label_);        // precalcule text geometry

					graph->string(point{ label_r.x + content_pos, label_r.y + txtoff }, label_, fgcolor); // draw full text of the cell index (column)

					if(static_cast<int>(ts.width) > static_cast<int>(label_r.width) - (content_pos + label_r.x))	// it was an excess
					{
						auto suspension_width = graph->text_extent_size(L"...").width;

						//The text is painted over the next subitem                // here beging the ...
						int xpos = label_r.x + static_cast<int>(label_r.width) - static_cast<int>(suspension_width);

						// litter rect with the item bg end ...
						graph->rectangle(rectangle{ xpos, label_r.y + 2, suspension_width, label_r.height - 4 }, true, bgcolor);
						graph->string(point{ xpos, label_r.y + 2 }, L"...");
					}
				}

				// draw the value
				if(value_r.empty())
					return;

				// background
				graph->rectangle(value_r, true, bgcolor);

				//if(false == draw_value(graph, rectangle{ value_r.x+1, value_r.y+2, value_r.width-2, value_r.height-4 }, bgcolor, fgcolor))
				if(false == draw_value(graph, value_r, bgcolor, fgcolor))
					return;

				// draw the value as label
				if(value_r.width > 5)
				{
					int content_pos = 5;
					graph->string(point{ value_r.x + content_pos, value_r.y + txtoff }, value_, fgcolor); // draw full text of the cell index (column)
				}
			}

			void pgitem::update()
			{
				ess_->update();
			}

			void pgitem::scroll()
			{
				ess_->lister.scroll(idx_, false, false);
			}

			void pgitem::emit_event()
			{
				if(evt_emit_)
					ess_->property_changed(item_proxy{ ess_, idx_ });
			}
			/// class pgitem end


			void es_lister::scroll(const index_pair& pos, bool as_first, bool expand_category)
			{
				if(pos.cat == ::nana::npos)
					throw std::invalid_argument("propertygrid: invalid pos to scroll");

				auto cat_ptr = get(pos.cat);

				if(pos.item != ::nana::npos && pos.item >= cat_ptr->items.size())
					throw std::invalid_argument("propertygrid: invalid pos to scroll");

				if(!cat_ptr->expand && expand_category)
				{
					expand(pos.cat, true);
				}


				int pos_offset_end, pos_offset = 0;
				bool first_cat = true;
				for(auto i_categ = get(0); i_categ != cat_container().end(); ++i_categ)
				{
					if(!first_cat)
					{
						if(i_categ == cat_ptr && pos.item == ::nana::npos)
						{
							pos_offset_end = pos_offset + ess_->cat_size;
							break;
						}

						pos_offset += ess_->cat_size;

						if(!i_categ->expand)
							continue;
					}
					first_cat = false;

					// items
					bool found = false;
					auto size = i_categ->items.size();
					for(decltype(size) item = 0; item < size; ++item)
					{
						if(i_categ == cat_ptr && item == pos.item)
						{
							pos_offset_end = pos_offset + i_categ->items[item]->size();

							found = true;
							break;
						}

						pos_offset += i_categ->items[item]->size();
					}

					if(found)
						break;
				}


				nana::rectangle rect;
				ess_->rect_lister(rect);

				auto xx = ess_->v_offset();

				if(pos_offset < -ess_->v_offset() || as_first)
					ess_->v_offset(-pos_offset);
				if(pos_offset_end + ess_->v_offset() > static_cast<int>(rect.height))
					ess_->v_offset(-(pos_offset_end - static_cast<int>(rect.height)));


				ess_->adjust_scroll_value();
			}

			void es_lister::erase(const index_pair& pos)
			{
				auto & catobj = *get(pos.cat);
				if(pos.item < catobj.items.size())
				{
					catobj.inline_panes.erase(catobj.inline_panes.begin() + pos.item);
					catobj.items.erase(catobj.items.begin() + pos.item);
				}
			}

			std::size_t es_lister::the_height_of_expanded() const
			{
				std::size_t n = (categories_.size() - 1) * ess_->cat_size;
				for(auto & c : categories_)
				{
					if(c.expand)
						for(auto & i : c.items)
							n += i->size();
				}
				return n;
			}



			class drawer_lister_impl
			{
			public:
				drawer_lister_impl(essence_t * es)
					:essence_(es)
				{}

				void draw(const nana::rectangle& rect) const
				{
					internal_scope_guard lock;

					widget* wdptr = essence_->lister.wd_ptr();
					auto bgcolor = wdptr->bgcolor();
					auto fgcolor = wdptr->fgcolor();

					auto labels_width = essence_->columns.col_width_adj(0, rect.width);
					auto values_width = essence_->columns.col_width_adj(1, rect.width);

					es_lister & lister = essence_->lister;

					//The Tracker indicates the item where mouse is placed
					auto tracker = essence_->pointer_where;

					auto y = essence_->v_offset();
					int txtoff = (essence_->cat_size - essence_->text_height) / 2;

					unsigned cat = 0;
					for(auto i_categ = lister.get(0); i_categ != lister.cat_container().end(); ++i_categ, ++cat)
					{
						if(cat != 0)
						{
							bool cat_visible = true;

							if(y + static_cast<int>(essence_->cat_size) < 0)
								cat_visible = false;
							if(y >= static_cast<int>(rect.height))
								cat_visible = false;

							if(cat_visible)
							{
								// draw category
								bool highlighted = tracker.is_category() && (cat == tracker.cat) ? true : false;
								//_m_draw_categ(*i_categ, rectangle{ rect.x, rect.y + y, rect.width, essence_->cat_size }, txtoff, bgcolor, highlighted);
								_m_draw_categ(*i_categ, rectangle{ rect.x, rect.y + y, rect.width, essence_->cat_size }, txtoff, nana::color(240, 240, 245), highlighted);
							}

							y += essence_->cat_size;
						}

						// items
						auto size = i_categ->items.size();
						for(decltype(size) item = 0; item < size; ++item)
						{
							bool item_visible = true;
							auto& item_property = i_categ->items[item];
							auto item_size = item_property->size();

							if(i_categ->expand)
							{
								if(y + static_cast<int>(item_size) < 0)
									item_visible = false;
								if(y >= static_cast<int>(rect.height))
									item_visible = false;
							}
							else
								item_visible = false;

							auto & inline_wdg = i_categ->inline_panes[item];

							if(item_visible)
							{
								// draw item
								rectangle visible_r{ 0, 0, rect.width, rect.height };
								rectangle value_min_r{ static_cast<int>(labels_width), y, values_width, item_size };
								
								rectangle pane_r;
								if(::nana::overlap(visible_r, value_min_r, pane_r))
								{
									pane_r.x += rect.x;
									pane_r.y += rect.y;

									inline_wdg->adjust_position(pane_r, nana::rectangle{ 0, std::min(0, y), pane_r.width, item_size });
									API::show_window(*inline_wdg, true);
								}
								else
								{
									API::show_window(*inline_wdg, false);
								}

								item_property->activate(essence_, index_pair{ cat, item });
								rectangle label_r{ rect.x, rect.y + y, labels_width, item_size };
								value_min_r.x += rect.x;
								value_min_r.y += rect.y;
								item_property->draw(essence_->graph, label_r, value_min_r, txtoff, bgcolor, fgcolor);
							}
							else
							{
								// hide item's pane
								API::show_window(*inline_wdg, false);
							}

							if(i_categ->expand)
								y += item_size;
						}
					}

					if(y < static_cast<int>(rect.height))
						essence_->graph->rectangle(rectangle{ rect.x, rect.y + y, rect.width, static_cast<unsigned>(rect.height - y) }, true, bgcolor);
				}


			private:
				void _m_draw_categ(const category_t& categ, rectangle rect, int txtoff, nana::color bgcolor, bool highlighted) const
				{
					if(highlighted)
						bgcolor = bgcolor.blend(static_cast<color_rgb>(0x99defd), 0.8);

					auto graph = essence_->graph;
					graph->rectangle(rect, true, bgcolor);


					color txt_color{ colors::black };

					facade<element::arrow> arrow("solid_triangle");
					arrow.direction(direction::southeast);
					if(!categ.expand)
					{
						arrow.switch_to("hollow_triangle");
						arrow.direction(direction::east);
					}

					arrow.draw(*graph, {}, txt_color,
					{ rect.x, rect.y + static_cast<int>(rect.height - 16) / 2, 16, 16 },
						element_state::normal);

					graph->string({ rect.x + 20, rect.y + txtoff }, categ.text, txt_color);


					if(!categ.expand)
					{
						::nana::point pos{ rect.x, rect.y + static_cast<int>(rect.height) - 1 };
						graph->line(pos, { rect.x + static_cast<int>(rect.width), pos.y }, colors::light_gray);
					}
				}


				essence_t* essence_;
			};

			//class trigger: public drawer_trigger
			trigger::trigger()
				: essence_(new essence_t),
				drawer_lister_(new drawer_lister_impl(essence_))
			{}

			trigger::~trigger()
			{
				delete drawer_lister_;
				delete essence_;
			}

			essence_t& trigger::essence() const
			{
				return *essence_;
			}

			void trigger::_m_draw_border()
			{
				if(API::widget_borderless(*essence_->lister.wd_ptr()))
					return;

				auto & graph = *essence_->graph;
				auto size = graph.size();
				//Draw Border
				graph.rectangle(false, static_cast<color_rgb>(0x9cb6c5));
				graph.line({ 1, 1 }, { 1, static_cast<int>(size.height) - 2 }, colors::white);
				graph.line({ static_cast<int>(size.width) - 2, 1 }, { static_cast<int>(size.width) - 2, static_cast<int>(size.height) - 2 });
			}

			void trigger::attached(widget_reference widget, graph_reference graph)
			{
				essence_->scheme_ptr = static_cast<::nana::propertygrid::scheme_type*>(API::dev::get_scheme(widget));
				essence_->graph = &graph;
				typeface_changed(graph);

				essence_->lister.bind(essence_, widget);
				widget.bgcolor(colors::white);
			}

			void trigger::detached()
			{
				essence_->graph = nullptr;
			}

			void trigger::typeface_changed(graph_reference graph)
			{
				essence_->text_height = graph.text_extent_size(L"jHWn0123456789/<?'{[|\\_").height;
				essence_->cat_size = essence_->text_height + 10;

				// apply to all items
				for(auto & c : essence_->lister.cat_container())
				{
					for(auto & i : c.items)
						i->typeface_changed(essence_->text_height);
				}
			}

			void trigger::refresh(graph_reference)
			{
				if(API::is_destroying(essence_->lister.wd_ptr()->handle()))
					return;

				nana::rectangle r;

				if(essence_->rect_lister(r))
					drawer_lister_->draw(r);
				_m_draw_border();
			}

			void trigger::mouse_move(graph_reference graph, const arg_mouse& arg)
			{
				bool update = false;

				if(essence_->mouse_moved(arg.pos))
				{
					update = true;
				}

				if(update)
				{
					refresh(graph);
					API::dev::lazy_refresh();
				}
			}

			void trigger::mouse_leave(graph_reference graph, const arg_mouse& arg)
			{
				if(!essence_->pointer_where.empty())
				{
					essence_->pointer_where.reset();

					refresh(graph);
					API::dev::lazy_refresh();
				}
			}

			void trigger::mouse_down(graph_reference, const arg_mouse& arg)
			{
			}

			void trigger::mouse_up(graph_reference graph, const arg_mouse& arg)
			{
			}

			void trigger::mouse_wheel(graph_reference graph, const arg_wheel& arg)
			{
				if(essence_->wheel(arg.upwards))
				{
					essence_->adjust_scroll_value();
					refresh(graph);
					API::dev::lazy_refresh();
				}
			}

			void trigger::click(graph_reference graph, const arg_click& arg)
			{
				auto tracker = essence_->pointer_where;
				if(tracker.empty())
					return;

				auto& lister = essence_->lister;
				lister.scroll(tracker, false, false);

				if(!tracker.is_category())
					return;

				//arg_category ai(cat_proxy(essence_, item_pos.cat));
				//lister.wd_ptr()->events().category_click.emit(ai);

				bool do_expand = (lister.expand(tracker.cat) == false);
				lister.expand(tracker.cat, do_expand);

				essence_->adjust_scroll_life();
				refresh(graph);
				API::dev::lazy_refresh();
			}

			void trigger::resized(graph_reference graph, const arg_resized&)
			{
				essence_->adjust_scroll_life();
				refresh(graph);
				API::dev::lazy_refresh();
			}

			void trigger::key_press(graph_reference graph, const arg_keyboard& arg)
			{
				/*
				switch(arg.key)
				{
				case keyboard::os_pageup:
				case keyboard::os_pagedown:
					break;
				case keyboard::os_home:
					break;
				case keyboard::os_end:
					break;

				default:
					return;
				}
				refresh(graph);
				API::dev::lazy_refresh();
				*/
			}
			//end class trigger

			//class item_proxy
			item_proxy::item_proxy(essence_t * ess)
				: ess_(ess)
			{}

			item_proxy::item_proxy(essence_t * ess, const index_pair& pos)
				: ess_(ess),
				pos_(pos)
			{
				if(ess)
				{
					auto i = ess_->lister.get(pos.cat);
					cat_ = &(*i);
				}
			}

			bool item_proxy::empty() const
			{
				return !ess_;
			}

			//Behavior of Iterator's value_type
			bool item_proxy::operator==(const char * s) const
			{
				return this->operator==(std::string(s));
			}

			bool item_proxy::operator==(const std::string& s) const
			{
				return (ess_->lister.get_property(cat_, pos_.item).label() == s);
			}

			item_proxy& item_proxy::operator=(const item_proxy& rhs)
			{
				if(this != &rhs)
				{
					ess_ = rhs.ess_;
					cat_ = rhs.cat_;
					pos_ = rhs.pos_;
				}
				return *this;
			}

			// Behavior of Iterator
			item_proxy& item_proxy::operator++()
			{
				if(++pos_.item >= cat_->items.size())
					cat_ = nullptr;

				return *this;
			}

			// Behavior of Iterator
			item_proxy item_proxy::operator++(int)
			{
				item_proxy ip(*this);

				if(++pos_.item >= cat_->items.size())
					cat_ = nullptr;
				return ip;
			}

			// Behavior of Iterator
			item_proxy& item_proxy::operator*()
			{
				return *this;
			}

			// Behavior of Iterator
			const item_proxy& item_proxy::operator*() const
			{
				return *this;
			}

			// Behavior of Iterator
			item_proxy* item_proxy::operator->()
			{
				return this;
			}

			// Behavior of Iterator
			const item_proxy* item_proxy::operator->() const
			{
				return this;
			}

			// Behavior of Iterator
			bool item_proxy::operator==(const item_proxy& rhs) const
			{
				if((ess_ != rhs.ess_) || (cat_ != rhs.cat_))
					return false;

				//They both are end iterator when cat_ == 0
				return (!cat_ || (pos_ == rhs.pos_));
			}

			// Behavior of Iterator
			bool item_proxy::operator!=(const item_proxy& rhs) const
			{
				return !this->operator==(rhs);
			}

			//Undocumented methods
			essence_t* item_proxy::_m_ess() const
			{
				return ess_;
			}

			index_pair item_proxy::pos() const
			{
				return pos_;
			}

			item_proxy& item_proxy::label(const std::string& label)
			{
				_m_property().label(label);
				_m_ess()->update();
				return *this;
			}
			std::string item_proxy::label() const
			{
				return _m_property().label();
			}

			item_proxy& item_proxy::value(const std::string& value, bool emit)
			{
				if(emit)
					_m_ess()->property_changed(*this);

				_m_property().value(value);
				_m_ess()->update();
				return *this;
			}
			std::string item_proxy::value() const
			{
				return _m_property().value();
			}

			bool item_proxy::enabled()
			{
				return _m_property().enabled();
			}
			void item_proxy::enabled(bool state)
			{
				_m_property().enabled(state);
			}

			auto item_proxy::_m_property() const -> pgitem&
			{
				return ess_->lister.get_property(cat_, pos_.item);
			}

			auto item_proxy::_m_pgitem() const -> pgitem*
			{
				return &ess_->lister.get_property(cat_, pos_.item);
			}
			//end class item_proxy

			//class cat_proxy
			//the member cat_ is used for fast accessing to the category
			cat_proxy::cat_proxy(essence_t * ess, std::size_t pos)
				: ess_(ess),
				pos_(pos)
			{
				_m_cat_by_pos();
			}

			cat_proxy::cat_proxy(essence_t* ess, category_t* cat)
				: ess_(ess),
				cat_(cat)
			{
				for(auto & m : ess->lister.cat_container())
				{
					if(&m == cat)
						break;
					++pos_;
				}
			}

			item_proxy cat_proxy::append(pgitem_ptr p)
			{
				internal_scope_guard lock;

				std::unique_ptr<inline_pane> pane_ptr(new inline_pane(*ess_->lister.wd_ptr()));

				p->activate(ess_, index_pair{ pos_, cat_->items.size() });
				p->typeface_changed(ess_->text_height);
				p->create(pane_ptr->wd_ptr());

				cat_->items.push_back(std::move(p));
				cat_->inline_panes.push_back(std::move(pane_ptr));
				ess_->update();

				return back();
			}

			cat_proxy& cat_proxy::text(std::string s)
			{
				internal_scope_guard lock;
				if(s != cat_->text)
				{
					cat_->text.swap(s);
					ess_->update();
				}
				return *this;
			}

			std::string cat_proxy::text() const
			{
				internal_scope_guard lock;
				return cat_->text;
			}

			//Behavior of a container
			item_proxy cat_proxy::begin() const
			{
				auto i = ess_->lister.get(pos_);
				if(i->items.empty())
					return end();

				return item_proxy(ess_, index_pair(pos_, 0));
			}
			item_proxy cat_proxy::end() const
			{
				return item_proxy(ess_);
			}
			item_proxy cat_proxy::cbegin() const
			{
				return begin();
			}
			item_proxy cat_proxy::cend() const
			{
				return end();
			}

			item_proxy cat_proxy::at(std::size_t pos) const
			{
				if(pos >= size())
					throw std::out_of_range("propertygrid.cat_proxy.at() invalid position");
				return item_proxy(ess_, index_pair(pos_, pos));
			}

			item_proxy cat_proxy::back() const
			{
				if(cat_->items.empty())
					throw std::runtime_error("propertygrid.back() no element in the container.");

				return item_proxy(ess_, index_pair(pos_, cat_->items.size() - 1));
			}

			std::size_t cat_proxy::position() const
			{
				return pos_;
			}

			std::size_t cat_proxy::size() const
			{
				return cat_->items.size();
			}

			// Behavior of Iterator
			cat_proxy& cat_proxy::operator=(const cat_proxy& r)
			{
				if(this != &r)
				{
					ess_ = r.ess_;
					cat_ = r.cat_;
					pos_ = r.pos_;
				}
				return *this;
			}

			// Behavior of Iterator
			cat_proxy & cat_proxy::operator++()
			{
				++pos_;
				_m_cat_by_pos();

				return *this;
			}

			// Behavior of Iterator
			cat_proxy	cat_proxy::operator++(int)
			{
				cat_proxy ip(*this);
				++pos_;
				_m_cat_by_pos();

				return ip;
			}

			// Behavior of Iterator
			cat_proxy& cat_proxy::operator*()
			{
				return *this;
			}

			// Behavior of Iterator
			const cat_proxy& cat_proxy::operator*() const
			{
				return *this;
			}

			/// Behavior of Iterator
			cat_proxy* cat_proxy::operator->()
			{
				return this;
			}

			/// Behavior of Iterator
			const cat_proxy* cat_proxy::operator->() const
			{
				return this;
			}

			// Behavior of Iterator
			bool cat_proxy::operator==(const cat_proxy& r) const
			{
				if(ess_ != r.ess_)
					return false;

				if(ess_)	//Not empty
					return (pos_ == r.pos_);

				return true;	//Both are empty
			}

			// Behavior of Iterator
			bool cat_proxy::operator!=(const cat_proxy& r) const
			{
				return !this->operator==(r);
			}

			void cat_proxy::_m_cat_by_pos()
			{
				if(pos_ >= ess_->lister.size_categ())
				{
					ess_ = nullptr;
					cat_ = nullptr;
					return;
				}

				auto i = ess_->lister.get(pos_);
				cat_ = &(*i);
			}

			//A fix for auto_draw, to make sure the inline widget set() issued after value() and value_ptr() are actually set.
			//Fixed by leobackes(pr#86)
			void cat_proxy::_m_update() {
				ess_->update();
			}

			//class cat_proxy

			//end class cat_proxy
		}
	}//end namespace drawerbase


	//class propertygrid
	propertygrid::propertygrid(window wd, bool visible)
	{
		create(wd, rectangle(), visible);
	}

	propertygrid::propertygrid(window wd, const rectangle& r, bool visible)
	{
		create(wd, r, visible);
	}

	void propertygrid::enabled(bool state)
	{
		if(_en == state)
			return;

		_m_ess().lister.enabled(_en = state);
	}

	void propertygrid::auto_draw(bool ad)
	{
		_m_ess().set_auto_draw(ad);
	}

	void propertygrid::scroll(const index_pair& pos, bool as_first)
	{
		auto& ess = _m_ess();
		ess.lister.scroll(pos, as_first);
		ess.update();
	}

	propertygrid& propertygrid::labels_width(unsigned pixels)
	{
		auto & ess = _m_ess();
		ess.columns.set_min_width(0, pixels);
		ess.update();
		return *this;
	}
	propertygrid& propertygrid::values_width(unsigned pixels)
	{
		auto & ess = _m_ess();
		ess.columns.set_min_width(1, pixels);
		ess.update();
		return *this;
	}

	unsigned propertygrid::labels_width() const
	{
		return _m_ess().columns.get_min_width(0);
	}
	unsigned propertygrid::values_width() const
	{
		return _m_ess().columns.get_min_width(1);
	}

	unsigned propertygrid::labels_auto_width(unsigned max)
	{
		auto & ess = _m_ess();
		unsigned max_w = ess.auto_width(0, max);
		ess.update();
		return max_w;
	}
	unsigned propertygrid::values_auto_width(unsigned max)
	{
		auto & ess = _m_ess();
		unsigned max_w = ess.auto_width(1, max);
		ess.update();
		return max_w;
	}

	propertygrid::cat_proxy propertygrid::append(std::string s)
	{
		auto & ess = _m_ess();

		auto pos = find(s);
		if(pos != npos)
			return cat_proxy{ &ess, pos };

		internal_scope_guard lock;
		auto new_cat_ptr = ess.lister.create_cat(std::move(s));
		ess.update();

		return cat_proxy{ &ess, new_cat_ptr };
	}

	auto propertygrid::insert(cat_proxy cat, std::string str) -> cat_proxy
	{
		internal_scope_guard lock;
		auto & ess = _m_ess();
		auto new_cat_ptr = ess.lister.create_cat(cat.position(), std::move(str));
		return cat_proxy{ &ess, new_cat_ptr };
	}

	propertygrid::cat_proxy propertygrid::at(std::size_t pos) const
	{
		auto & ess = _m_ess();
		if(pos >= ess.lister.size_categ())
			throw std::out_of_range("Nana.propertygrid.at(): invalid position");

		return cat_proxy(&ess, pos);
	}

	std::size_t propertygrid::find(std::string category) const
	{
		auto & ess = _m_ess();

		std::size_t pos = 0;
		for(auto i_categ = ess.lister.get(0); i_categ != ess.lister.cat_container().end(); ++i_categ, ++pos)
		{
			if(i_categ->text == category)
				return pos;
		}
		return npos;
	}

	propertygrid::item_proxy propertygrid::at(const index_pair& idx) const
	{
		return at(idx.cat).at(idx.item);
	}

	void propertygrid::clear(std::size_t cat)
	{
		auto& ess = _m_ess();
		ess.lister.clear(cat);

		// from current display position
		// move to the cat self
		ess.lister.scroll({ cat, npos }, true);
		ess.update();
	}

	void propertygrid::clear()
	{
		auto& ess = _m_ess();
		ess.lister.clear();

		// from current display position
		// move to the first cat
		ess.lister.scroll({ 0, npos }, true);
		ess.update();
	}

	void propertygrid::erase(std::size_t cat)
	{
		auto& ess = _m_ess();
		ess.lister.erase(cat);

		// from current display position
		// move to the previous cat
		if(cat)
			ess.lister.scroll({ cat - 1, npos }, true);
		else
			ess.lister.scroll({ 1, npos }, true);
		ess.update();
	}

	void propertygrid::erase()
	{
		auto& ess = _m_ess();
		ess.lister.erase();
		ess.update();
	}

	void propertygrid::erase(item_proxy ip)
	{
		auto& ess = _m_ess();
		auto _where = ip.pos();

		ess.lister.erase(_where);
		ess.update();
	}

	std::size_t propertygrid::size_categ() const
	{
		return _m_ess().lister.size_categ();
	}

	std::size_t propertygrid::size_item(std::size_t categ) const
	{
		return _m_ess().lister.size_item(categ);
	}

	drawerbase::propertygrid::essence_t & propertygrid::_m_ess() const
	{
		return get_drawer_trigger().essence();
	}

	//end class propertygrid
}//end namespace nana
