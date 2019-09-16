/**
 *	@file nana/gui/adi_place.cpp
 */

#include <cfloat>
#include <cmath>
#include <map>
#include <set>
#include <algorithm>
#include <nana/push_ignore_diagnostic>
#include <nana/deploy.hpp>
#include "adi_place.hpp"
#include <nana/gui/programming_interface.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/dragger.hpp>
#include <nana/gui/drawing.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/element.hpp>
#include <nana/paint/text_renderer.hpp>


#define DOCK_CAPTION_H		20
#define DOCK_PANE_MIN_W		30 //TODO: aggiungere in paneinfo



namespace nana
{
	//class paneinfo
	paneinfo& paneinfo::caption(const std::string& name)
	{
		data_.caption = name;
		return *this;
	}
	std::string paneinfo::caption() const
	{
		return data_.caption;
	}

	paneinfo& paneinfo::show_caption(bool show)
	{
		data_.show_caption = show;
		return *this;
	}
	bool paneinfo::show_caption() const
	{
		return data_.show_caption;
	}

	paneinfo& paneinfo::show_close(bool show)
	{
		data_.close_button = show;
		return *this;
	}
	bool paneinfo::show_close() const
	{
		return data_.close_button;
	}

	paneinfo& paneinfo::weight(double value)
	{
		data_.weight = value;
		return *this;
	}
	double paneinfo::weight() const
	{
		return data_.weight;
	}

	paneinfo& paneinfo::center(bool value)
	{
		data_.center = value;
		return *this;
	}
	bool paneinfo::center() const
	{
		return data_.center;
	}
	//end class paneinfo


	namespace adi_place_parts
	{
		class drawer_splitter
			: public drawer_trigger
		{
		public:
			void set_renderer(const std::function<void(window, paint::graphics&, mouse_action)>& rd)
			{
				renderer_ = rd;
			}
		private:
			void attached(widget_reference wdg, graph_reference) override
			{
				window_handle_ = wdg;
			}

			void refresh(graph_reference graph) override
			{
				API::dev::copy_transparent_background(window_handle_, graph);
				if(renderer_)
					renderer_(window_handle_, graph, API::mouse_action(window_handle_));
			}

			void mouse_enter(graph_reference graph, const arg_mouse&) override
			{
				refresh(graph);
				API::dev::lazy_refresh();
			}

			void mouse_move(graph_reference graph, const arg_mouse&) override
			{
				refresh(graph);
				API::dev::lazy_refresh();
			}

			void mouse_leave(graph_reference graph, const arg_mouse&) override
			{
				refresh(graph);
				API::dev::lazy_refresh();
			}

			void mouse_down(graph_reference graph, const arg_mouse&) override
			{
				refresh(graph);
				API::dev::lazy_refresh();
			}

			void mouse_up(graph_reference graph, const arg_mouse&) override
			{
				refresh(graph);
				API::dev::lazy_refresh();
			}
		private:
			window window_handle_{ nullptr };
			std::function<void(window, paint::graphics&, mouse_action)> renderer_;
		};

		class splitter
			: public widget_object<category::widget_tag, drawer_splitter>
		{
		public:
			void set_renderer(const std::function<void(window, paint::graphics&, mouse_action)>& rd)
			{
				get_drawer_trigger().set_renderer(rd);
			}
		private:
			void _m_complete_creation() override
			{
				this->caption("place-splitter");
				widget_object<category::widget_tag, drawer_splitter>::_m_complete_creation();

				API::effects_bground(*this, effects::bground_transparent(0), 0);
			}
		};

		class dock_notifier_interface
		{
		public:
			virtual ~dock_notifier_interface() = default;

			virtual void notify_float() = 0;
			virtual void notify_dock() = 0;
			virtual void notify_move() = 0;
			virtual void notify_move_stopped() = 0;

			//a dockarea requests to close the dockpane
			virtual void request_close() = 0;
		};

		class dockcaption_dtrigger
			: public drawer_trigger
		{
		public:
			void set_info(paneinfo * info)
			{
				info_ = info;
			}

			void set_scheme(adi_place::scheme_t* scheme)
			{
				scheme_ = scheme;
			}

			void on_close(std::function<void()>&& fn)
			{
				close_fn_ = std::move(fn);
			}

			bool hit_close() const
			{
				return x_pointed_;
			}

		private:
			virtual void attached(widget_reference wdg, graph_reference graph) override
			{
				window_handle_ = wdg;
				text_rd_.reset(new paint::text_renderer(graph));
			}

			void refresh(graph_reference& graph) override
			{
				if(!info_->show_caption())
					return;

				graph.palette(true, scheme_->caption_fg);
				graph.rectangle(true, scheme_->caption_bg);

				//draw caption
				auto text = to_wstring(API::window_caption(window_handle_));
				if((graph.size().width > DOCK_CAPTION_H) && (graph.size().width - DOCK_CAPTION_H > 10))
					text_rd_->render({ 3, 1 }, text.data(), text.size(), graph.size().width - DOCK_CAPTION_H, paint::text_renderer::mode::truncate_with_ellipsis);

				//draw x button
				if(info_->show_close())
				{
					auto r = _m_button_area();
					if(x_pointed_)
					{
						color xclr = scheme_->caption_highlight;

						if(x_state_ == ::nana::mouse_action::pressed)
							xclr = xclr.blend(colors::white, 0.2);

						graph.rectangle(r, true, xclr);
					}

					r.x += (r.width - 16) / 2;
					r.y = (r.height - 16) / 2;

					x_icon_.draw(graph, scheme_->caption_bg, scheme_->caption_fg, r, element_state::normal);
				}
			}

			void mouse_move(graph_reference graph, const arg_mouse& arg) override
			{
				if(info_->show_close())
					x_pointed_ = _m_button_area().is_hit(arg.pos);

				refresh(graph);
				API::dev::lazy_refresh();
			}

			void mouse_leave(graph_reference graph, const arg_mouse&) override
			{
				x_pointed_ = false;
				refresh(graph);
				API::dev::lazy_refresh();
			}

			void mouse_down(graph_reference graph, const arg_mouse&) override
			{
				if(!x_pointed_)
					return;

				x_state_ = ::nana::mouse_action::pressed;

				refresh(graph);
				API::dev::lazy_refresh();
			}

			void mouse_up(graph_reference graph, const arg_mouse&) override
			{
				if(!x_pointed_)
					return;

				x_state_ = ::nana::mouse_action::hovered;
				refresh(graph);
				API::dev::lazy_refresh();

				close_fn_();
			}
		private:
			::nana::rectangle _m_button_area() const
			{
				auto sz = API::window_size(window_handle_);
				return{ static_cast<int>(sz.width) - DOCK_CAPTION_H, 0, DOCK_CAPTION_H, sz.height };
			}
		public:
			window window_handle_{ nullptr };
			std::unique_ptr<paint::text_renderer> text_rd_;
			bool x_pointed_{ false };
			::nana::mouse_action x_state_{ ::nana::mouse_action::normal };
			facade<element::x_icon>	x_icon_;

			std::function<void()>	close_fn_;

			paneinfo* info_{ nullptr };
			adi_place::scheme_t* scheme_{ nullptr };
		};

		class dockarea_caption
			: public widget_object<category::widget_tag, dockcaption_dtrigger>
		{
		public:
			using widget_object<category::widget_tag, dockcaption_dtrigger>::get_drawer_trigger;
		};

		class dockarea
			: public widget_object <category::lite_widget_tag, drawer_trigger>
		{
			using base_type = widget_object<category::lite_widget_tag, drawer_trigger>;

			using factory = std::function<std::unique_ptr<widget>(window)>;

		public:
			void set_notifier(adi_place_parts::dock_notifier_interface* notifier)
			{
				notifier_ = notifier;
			}

			void create(window parent, paneinfo* info, adi_place::scheme_t* scheme)
			{
				host_window_ = parent;
				info_ = info;
				scheme_ = scheme;

				base_type::create(parent, true);
				this->caption("dockarea");
				caption_.create(*this, true);
				caption_.caption(info_->caption());
				caption_.get_drawer_trigger().set_info(info);
				caption_.get_drawer_trigger().set_scheme(scheme_);
				caption_.get_drawer_trigger().on_close([this]
					{
						notifier_->request_close();
					});

				this->events().resized.connect_unignorable([this](const arg_resized& arg)
					{
						adjust_caption_area_({ arg.width, arg.height });
					});

				auto grab_fn = [this](const arg_mouse& arg)
				{
					if(event_code::mouse_down == arg.evt_code)
					{
						if(::nana::mouse::left_button == arg.button)
						{
							moves_.started = true;
							moves_.start_pos = API::cursor_position();
							moves_.start_container_pos = (floating() ? container_->pos() : this->pos());
							caption_.set_capture(true);
						}
					}
					else if(event_code::mouse_move == arg.evt_code)
					{
						if(arg.left_button && moves_.started)
						{
							auto move_pos = API::cursor_position() - moves_.start_pos;
							if(!floating())
							{
								if(std::abs(move_pos.x) > 4 || std::abs(move_pos.y) > 4)
									float_away(move_pos);
							}
							else
							{
								move_pos += moves_.start_container_pos;
								API::move_window(container_->handle(), move_pos);

								if(!caption_.get_drawer_trigger().hit_close())
									notifier_->notify_move();
							}
						}
					}
					else if(event_code::mouse_up == arg.evt_code)
					{
						if((::nana::mouse::left_button == arg.button) && moves_.started)
						{
							moves_.started = false;
							caption_.release_capture();
							notifier_->notify_move_stopped();
						}
					}
				};

				caption_.events().mouse_down.connect(grab_fn);
				caption_.events().mouse_move.connect(grab_fn);
				caption_.events().mouse_up.connect(grab_fn);
			}

			void add_pane(widget* w)
			{
				if(w)
				{
					API::set_parent_window(w->handle(), handle());
					widget_ptr.reset(w);
				}
			}

			void float_away(const ::nana::point& move_pos)
			{
				if(container_)
					return;

				caption_.release_capture();

				rectangle r{ pos() + move_pos, size() };
				container_.reset(new form(host_window_, r.pare_off(-1), form::appear::bald<form::appear::sizable>()));
				drawing dw(container_->handle());
				dw.draw([](paint::graphics& graph)
					{
						graph.rectangle(false, colors::coral);
					});

				API::set_parent_window(handle(), container_->handle());
				this->move({ 1, 1 });

				container_->events().resized.connect_unignorable([this](const arg_resized& arg)
					{
						this->size({ arg.width - 2, arg.height - 2 });
					});

				container_->show();
				caption_.set_capture(true);

				notifier_->notify_float();
			}

			void dock()
			{
				caption_.release_capture();

				API::set_parent_window(handle(), host_window_);
				container_.reset();
				notifier_->notify_dock();
			}

			bool floating() const
			{
				return (nullptr != container_);
			}

			void update()
			{
				caption_.caption(info_->caption());
				adjust_caption_area_(size());
				API::refresh_window(*this);
			}

		private:
			window host_window_{ nullptr };
			paneinfo* info_{ nullptr };
			adi_place::scheme_t* scheme_{ nullptr };
			dock_notifier_interface* notifier_{ nullptr };
			std::unique_ptr<form>	container_;
			dockarea_caption	caption_;
			std::unique_ptr<widget> widget_ptr;

			void adjust_caption_area_(const nana::size& sz)
			{
				rectangle r{ 0, 0, sz.width, DOCK_CAPTION_H };

				if(info_->show_caption())
				{
					caption_.move(r);

					r.y = DOCK_CAPTION_H;
					r.height = sz.height - DOCK_CAPTION_H;
				}
				else
					r.height = sz.height;

				if(widget_ptr)
					widget_ptr->move(r);
			}

			struct moves
			{
				bool started{ false };
				::nana::point start_pos;
				::nana::point start_container_pos;
			} moves_;
		};//class dockarea


		void _indicator_left(paint::graphics& graph)
		{
			graph.rectangle(false, colors::midnight_blue);
			graph.rectangle({ 1, 1, 30, 30 }, true, colors::light_sky_blue);

			facade<element::arrow> arrow("solid_triangle");
			arrow.direction(::nana::direction::west);
			arrow.draw(graph, colors::light_sky_blue, colors::midnight_blue, { 16, 8, 16, 16 }, element_state::normal);

			graph.rectangle({ 5, 4, 11, 24 }, true, colors::midnight_blue);
			graph.rectangle({ 6, 7, 9, 20 }, true, colors::button_face);
		}

		void _indicator_right(paint::graphics& graph)
		{
			graph.rectangle(false, colors::midnight_blue);
			graph.rectangle({ 1, 1, 30, 30 }, true, colors::light_sky_blue);

			facade<element::arrow> arrow("solid_triangle");
			arrow.direction(::nana::direction::east);
			arrow.draw(graph, colors::light_sky_blue, colors::midnight_blue, { 2, 8, 16, 16 }, element_state::normal);

			graph.rectangle({ 17, 4, 11, 24 }, true, colors::midnight_blue);
			graph.rectangle({ 18, 7, 9, 20 }, true, colors::button_face);
		}

		void _indicator_up(paint::graphics& graph)
		{
			graph.rectangle(false, colors::midnight_blue);
			graph.rectangle({ 1, 1, 30, 30 }, true, colors::light_sky_blue);

			facade<element::arrow> arrow("solid_triangle");
			arrow.direction(::nana::direction::north);
			arrow.draw(graph, colors::light_sky_blue, colors::midnight_blue, { 8, 14, 16, 16 }, element_state::normal);

			graph.rectangle({ 4, 5, 24, 11 }, true, colors::midnight_blue);
			graph.rectangle({ 5, 8, 22, 7 }, true, colors::button_face);
		}

		void _indicator_down(paint::graphics& graph)
		{
			graph.rectangle(false, colors::midnight_blue);
			graph.rectangle({ 1, 1, 30, 30 }, true, colors::light_sky_blue);

			facade<element::arrow> arrow("solid_triangle");
			arrow.direction(::nana::direction::south);
			arrow.draw(graph, colors::light_sky_blue, colors::midnight_blue, { 8, 1, 16, 16 }, element_state::normal);

			graph.rectangle({ 4, 16, 24, 11 }, true, colors::midnight_blue);
			graph.rectangle({ 5, 19, 22, 7 }, true, colors::button_face);
		}


		//number_t is used for storing a number type variable
		//such as integer, real and percent. Essentially, percent is a typo of real.
		class number_t
		{
		public:
			enum class kind { none, integer, real, percent };

			number_t()
				: kind_(kind::none)
			{
				value_.integer = 0;
			}

			void reset() noexcept
			{
				kind_ = kind::none;
				value_.integer = 0;
			}

			bool is_negative() const
			{
				return (((kind::integer == kind_) && (value_.integer < 0)) ||
					((kind::real == kind_ || kind::percent == kind_) && (value_.real < 0)));
			}

			bool empty() const noexcept
			{
				return (kind::none == kind_);
			}

			kind kind_of() const noexcept
			{
				return kind_;
			}

			double get_value(int ref_percent) const noexcept
			{
				switch(kind_)
				{
				case kind::integer:
					return value_.integer;
				case kind::real:
					return value_.real;
				case kind::percent:
					return value_.real * ref_percent;
				default:
					break;
				}
				return 0;
			}

			int integer() const noexcept
			{
				if(kind::integer == kind_)
					return value_.integer;
				return static_cast<int>(value_.real);
			}

			double real() const noexcept
			{
				if(kind::integer == kind_)
					return value_.integer;
				return value_.real;
			}

			void assign(int i) noexcept
			{
				kind_ = kind::integer;
				value_.integer = i;
			}

			void assign(double d) noexcept
			{
				kind_ = kind::real;
				value_.real = d;
			}

			void assign_percent(double d) noexcept
			{
				kind_ = kind::percent;
				value_.real = d / 100;
			}
		private:
			kind kind_;
			union valueset
			{
				int integer;
				double real;
			}value_;
		};//end class number_t
	}//end namespace place_parts


	typedef adi_place_parts::number_t number_t;


	static int horz_point(bool vert, const point& pos)
	{
		return (vert ? pos.y : pos.x);
	}


	//struct implement
	struct adi_place::implement
	{
		class field_dock;

		class division;
		class div_arrange;
		class div_splitter;
		class div_dockpane;

		window window_handle{nullptr};
		event_handle event_size_handle{nullptr};

		std::unique_ptr<division> root_division;
		std::map<std::string, field_dock*> docks;

		std::function<void(window, paint::graphics&, nana::mouse_action)> split_renderer;
		std::set<div_splitter*> splitters;

		scheme_t scheme;

		//The following functions are defined behind the definition of class division.
		//because the class division here is an incomplete type.
		implement();
		~implement();


		void purge();
		void collocate();

		struct dock_indicator
		{
			dockposition pos;
			point delta;
			paint::graphics graph;
			std::unique_ptr<panel<true>> dock_area;
			std::unique_ptr<form> docker;
		};

		bool dipanes_hide_forced{ false };
		bool dipanes_locked{ false };

		std::vector<std::unique_ptr<dock_indicator>> abs_indicators;
		std::vector<std::unique_ptr<dock_indicator>> pane_indicators;
		std::vector<std::unique_ptr<division>> floating_divs;

		bool is_vertical(dockposition pos);
		bool is_before(dockposition pos);

		void show_indicators();
		void show_pane_indicators(division* div);
		void hide_indicators(bool pane_only = false);
		void force_hide_pane_indicators(bool force);
		void lock_pane_indicators(bool lock);

		bool to_float(division* div);
		bool to_dock(division* div, dockposition pos);
		bool to_dock(division* div, division* where, dockposition pos);
		
		bool remove(division* div); //XXX review

		bool hit_window() const;
		bool hit_indicators(dockposition* position = nullptr, bool* absolute = nullptr) const;
		division* hit_panes();

		void print_debug();
	};	//end struct implement


	class adi_place::implement::field_dock
	{
	public:
		div_dockpane* attached{ nullptr };						//attached div object
		std::unique_ptr<adi_place_parts::dockarea> dockarea;	//the dockable widget
		paneinfo info;											//dock properties
	};//end class field_dock


	class adi_place::implement::division
	{
	public:
		enum class kind{ arrange, splitter, dockpane};

		division(kind k, const std::string& n) noexcept
			: kind_of_division(k),
			name(n)
		{
			min_px.assign(DOCK_PANE_MIN_W);
		}

		virtual ~division()
		{
#ifdef __DEBUG
			printf("*********** dtor %s - ", name.size() ? name.c_str() : "empty");
			if(division::kind::splitter == kind_of_division)
				printf("splitter\n");
			else if(division::kind::dockpane == kind_of_division)
				printf("dockpane\n");
			else if(division::kind::arrange == kind_of_division)
				printf("arrange\n");
			else
				printf("unknown\n");
#endif
		}

		void set_visible(bool vsb)
		{
			//ADI_TODO: if not visible move to detached and hide
			visible = vsb;
		}
		
		virtual bool center() const
		{
			return false;
		}

		virtual bool is_vertical() const
		{
			return false;
		}

		bool is_back(const division* div) const noexcept
		{
			if(!div || children.empty())
				return false;

			return (div == children.back().get());
		}

		static double limit_px(const division* div, double px, unsigned area_px) noexcept
		{
			if (!div->min_px.empty())
			{
				auto v = div->min_px.get_value(static_cast<int>(area_px));

				if (px < v)
					return v;
			}

			if (!div->max_px.empty())
			{
				auto v = div->max_px.get_value(static_cast<int>(area_px));
				if (px > v)
					return v;
			}
			return px;
		}

		bool is_fixed() const
		{
			return (weight_.kind_of() == number_t::kind::integer);
		}

		bool is_percent() const
		{
			return (weight_.kind_of() == number_t::kind::percent);
		}

		nana::rectangle margin_area() const
		{
			return field_area;
		}

		size_t index() const noexcept
		{
			if(div_owner)
			{
				for(size_t i = 0; i < div_owner->children.size(); ++i)
					if(div_owner->children[i].get() == this)
						return i;
			}
			return std::string::npos;
		}

		division* previous() const noexcept
		{
			auto i = index();
			return (i != std::string::npos && i > 0) ? div_owner->children[i - 1].get() : nullptr;
		}

		division* next() const noexcept
		{
			auto i = index();
			return (i < div_owner->children.size() - 1) ? div_owner->children[i + 1].get() : nullptr;
		}

	public:
		//Collocate the division and its children divisions,
		//The window parameter is specified for the window which the adi_place object binded.
		virtual void collocate(window) = 0;

	public:
		kind kind_of_division;
		bool visible{ true };
		std::string name;
		std::vector<std::unique_ptr<division>> children;

		::nana::rectangle field_area;
		number_t weight_;
		number_t min_px, max_px;

		division* div_owner{ nullptr };
	};//end class division

	class adi_place::implement::div_arrange
		: public division
	{
	public:
		div_arrange(bool vert, const std::string& name) noexcept
			: division(kind::arrange, name), vertical(vert)
		{}

		bool center() const override
		{
			for(auto& c : children)
				if(c->center())
					return true;

			return false;
		}

		bool is_vertical() const override
		{
			return vertical;
		}

		void collocate(window wd) override
		{
			rectangle_rotator area(vertical, margin_area());
			auto area_px = area.w();

			auto fa = _m_fixed_and_adjustable(area_px);
			double adjustable_px = _m_revise_adjustable(fa, area_px);

			auto has_center = center();
			
			double position = area.x();
			std::vector<division*> delay_collocates; // splitters are collocated after
			double precise_px = 0;
			for(auto& child_ptr : children)		/// First collocate child div's !!!
			{
				auto child = child_ptr.get();

				rectangle_rotator child_area(vertical, child->field_area);
				child_area.x_ref() = static_cast<int>(position);
				child_area.y_ref() = area.y();
				child_area.h_ref() = area.h();

				double child_px;				// and calculate this div.
				if(!child->is_fixed())			// with is fixed for fixed div
				{
					if(child->is_percent())		// and calculated for others: if the child div is percent - simple take it full
						child_px = area_px * child->weight_.real() + precise_px;
					else
						child_px = adjustable_px;

					child_px = limit_px(child, child_px, area_px);

					auto npx = static_cast<unsigned>(child_px);
					precise_px = child_px - npx;
					child_px = npx;
				}
				else
				{
					child_px = static_cast<unsigned>(child->weight_.integer());
				}

				//Use 'endpos' to calc width is to avoid deviation
				int endpos = static_cast<int>(position + child_px);
				if ((!child->is_fixed()) && child->max_px.empty() && is_back(child) && (endpos != area.right()))
					endpos = area.right();

				child_area.w_ref() = static_cast<unsigned>((std::max)(endpos - child_area.x(), 0));

				child->field_area = child_area.result();
				position += child_px;

				if(child->kind_of_division == kind::splitter)
					delay_collocates.emplace_back(child);
				else
				{
					if(kind::dockpane == child->kind_of_division)
					{
						if(has_center)
						{
							if(child->center())
								child->weight_.reset();
							else if(child->weight_.empty() || child->is_percent())
								child->weight_.assign(static_cast<int>(child_px));
						}
						else
						{
							if(child->weight_.empty() || child->is_fixed())
								child->weight_.assign_percent((child_px / area_px) * 100);
						}
					}

					child->collocate(wd);	/// The child div have full position. Now we can collocate inside it the child fields and child-div.
				}
			}

			for(auto child : delay_collocates)
				child->collocate(wd);

			if(has_center)
				weight_.reset();
			else if(weight_.empty() || is_percent())
				weight_.assign(static_cast<int>(area.h()));
		}

	private:
		//Returns the fixed pixels and the number of adjustable items.
		std::pair<unsigned, std::size_t> _m_fixed_and_adjustable(unsigned area_px) const noexcept
		{
			std::pair<unsigned, std::size_t> result;

			double children_fixed_px = 0;
			for(auto& child : children)
			{
				if(!child->weight_.empty())
					children_fixed_px += child->weight_.get_value(area_px);
				else
					++result.second;
			}
			result.first = static_cast<unsigned>(children_fixed_px);
			return result;
		}

		double _m_revise_adjustable(std::pair<unsigned, std::size_t>& fa, unsigned area_px)
		{
			if(fa.first >= area_px || 0 == fa.second)
				return 0;

			double var_px = area_px - fa.first;

			std::size_t min_count = 0;
			double sum_min_px = 0;
			std::vector<revised_division> revises;

			for(auto& child : children)
			{
				if(!child->weight_.empty())
					continue;

				double min_px = std::numeric_limits<double>::lowest(), max_px = std::numeric_limits<double>::lowest();

				if(!child->min_px.empty())
				{
					min_px = child->min_px.get_value(static_cast<int>(area_px));
					sum_min_px += min_px;
					++min_count;
				}

				if(!child->max_px.empty())
					max_px = child->max_px.get_value(static_cast<int>(area_px));

				if(min_px >= 0 && max_px >= 0 && min_px > max_px)
				{
					if(child->min_px.kind_of() == number_t::kind::percent)
						min_px = std::numeric_limits<double>::lowest();
					else if(child->max_px.kind_of() == number_t::kind::percent)
						max_px = std::numeric_limits<double>::lowest();
				}

				if(min_px >= 0 || max_px >= 0)
					revises.push_back({ child.get(), min_px, max_px });
			}

			if(revises.empty())
				return var_px / fa.second;

			double block_px = 0;
			double level_px = 0;
			auto rest_px = var_px - sum_min_px;
			std::size_t blocks = fa.second;

			while((rest_px > 0) && blocks)
			{
				auto lowest = _m_find_lowest_revised_division(revises, level_px);

				double fill_px = 0;
				//blocks may be equal to min_count. E.g, all child divisions have min/max attribute.
				if(blocks > min_count)
				{
					fill_px = rest_px / (blocks - min_count);
					if(fill_px + level_px <= lowest)
					{
						block_px += fill_px;
						break;
					}
				}

				block_px = lowest;
				if(blocks > min_count)
					rest_px -= (lowest - level_px) * (blocks - min_count);

				std::size_t full_count;
				min_count -= _m_remove_revised(revises, lowest, full_count);
				blocks -= full_count;
				level_px = lowest;
			}

			return block_px;
		}

		struct revised_division
		{
			division* div;
			double min_px;
			double max_px;
		};

		static double _m_find_lowest_revised_division(const std::vector<revised_division>& revises, double level_px) noexcept
		{
			double v = (std::numeric_limits<double>::max)();

			for(auto & rev : revises)
			{
				if (rev.min_px >= 0 && rev.min_px < v && rev.min_px > level_px)
					v = rev.min_px;
				else if (rev.max_px >= 0 && rev.max_px < v)
					v = rev.max_px;
			}
			return v;
		}

		static std::size_t _m_remove_revised(std::vector<revised_division>& revises, double value, std::size_t& full_count) noexcept
		{
			full_count = 0;
			std::size_t reached_mins = 0;
			auto i = revises.begin();
			while (i != revises.end())
			{
				if (i->max_px == value)
				{
					++full_count;
					i = revises.erase(i);
				}
				else
				{
					if (i->min_px == value)
						++reached_mins;
					++i;
				}
			}
			return reached_mins;
		}

	private:
		bool vertical;
	};


	class adi_place::implement::div_splitter
		: public division
	{
		struct div_block
		{
			division * div;
			int	pixels;

			div_block(division* d, int px) noexcept
				: div(d), pixels(px)
			{}
		};

	public:
		div_splitter(implement* impl, bool vert) noexcept :
			division(kind::splitter, std::string()), impl_(impl)
		{
			impl->splitters.insert(static_cast<div_splitter*>(this));
			this->splitter_.set_renderer(impl_->split_renderer);

			splitter_cursor_ = (vert ? cursor::size_ns : cursor::size_we);
			weight_.assign(static_cast<int>(impl_->scheme.splitter_width));
		}

		~div_splitter()
		{
			impl_->splitters.erase(this);
		}

		void set_renderer(const std::function<void(window, paint::graphics&, mouse_action)> & fn, bool update)
		{
			this->splitter_.set_renderer(fn);
			if(update && this->splitter_.handle())
				API::refresh_window(this->splitter_);
		}

		bool is_vertical() const override
		{
			return splitter_cursor_ == cursor::size_ns;
		}

	private:
		void collocate(window wd) override
		{
			if(API::is_destroying(wd))
				return;

			if(splitter_.empty())
			{
				splitter_.create(wd);
				splitter_.cursor(splitter_cursor_);

				dragger_.trigger(splitter_);

				auto grab_fn = [this](const arg_mouse& arg)
				{
					if ((false == arg.left_button) && (mouse::left_button != arg.button))
						return;

					auto const leaf_left = previous();
					auto const leaf_right = next();

					if (event_code::mouse_down == arg.evt_code)
					{
						begin_point_ = splitter_.pos();

						auto px_ptr = &nana::rectangle::width;

						//Use field_area of leaf, not margin_area. Otherwise splitter would be at wrong position
						auto const area_left = leaf_left->field_area;
						auto const area_right = leaf_right->field_area;

						if(nana::cursor::size_we != splitter_cursor_)
						{
							left_pos_ = area_left.y;
							right_pos_ = area_right.bottom();
							px_ptr = &nana::rectangle::height;
						}
						else
						{
							left_pos_ = area_left.x;
							right_pos_ = area_right.right();
						}

						left_pixels_ = area_left.*px_ptr;
						right_pixels_ = area_right.*px_ptr;

						grabbed_ = true;
					}
					else if(event_code::mouse_up == arg.evt_code)
					{
						grabbed_ = false;
						impl_->print_debug();
					}
					else if (event_code::mouse_move == arg.evt_code)
					{
						if(!grabbed_)
							return;
							
						auto const vert = (::nana::cursor::size_we != splitter_cursor_);
						auto const delta = horz_point(vert, splitter_.pos() - begin_point_);

						const auto total_pixels = static_cast<int>(left_pixels_ + right_pixels_);

						auto area_px = rectangle_rotator(vert, div_owner->margin_area()).w();
						//double imd_rate = 100.0 / area_px;

						auto left_px = std::clamp(static_cast<int>(left_pixels_) + delta, 0, total_pixels);
						left_px = static_cast<int>(limit_px(leaf_left, left_px, area_px));
						//leaf_left->weight.assign_percent(imd_rate * left_px);
						leaf_left->weight_.assign(left_px);

						auto right_px = std::clamp(static_cast<int>(right_pixels_) - delta, 0, total_pixels);
						right_px = static_cast<int>(limit_px(leaf_right, right_px, area_px));
						//leaf_right->weight.assign_percent(imd_rate * right_px);
						leaf_right->weight_.assign(right_px);

						pause_move_collocate_ = true;
						div_owner->collocate(splitter_.parent());

						//After the collocating, the splitter keeps the calculated weight of left division,
						//and clear the weight of right division.
						//leaf_right->weight.reset();
						//XXX il reset del weight si trova in collocate !!!

						pause_move_collocate_ = false;
					}
				};

				auto & events = splitter_.events();
				events.mouse_down.connect_unignorable(grab_fn);
				events.mouse_up.connect_unignorable(grab_fn);
				events.mouse_move.connect_unignorable(grab_fn);
			}

			_m_update_splitter_range();

			if (false == pause_move_collocate_)
				splitter_.move(this->field_area);
		}

		rectangle_rotator _m_update_splitter_range()
		{
			const bool vert = (cursor::size_ns == splitter_cursor_);

			rectangle_rotator area(vert, div_owner->margin_area());

			auto leaf_left = previous();
			auto leaf_right = next();

			rectangle_rotator left(vert, leaf_left->field_area);
			rectangle_rotator right(vert, leaf_right->field_area);

			const int left_base = left.x(), right_base = right.right();
			int pos = left_base;
			int endpos = right_base;

			if (!leaf_left->min_px.empty())
				pos += static_cast<int>(leaf_left->min_px.get_value(area.w()));
	
			if (!leaf_left->max_px.empty())
				endpos = left_base + static_cast<int>(leaf_left->max_px.get_value(area.w()));

			if (!leaf_right->min_px.empty())
				endpos = (std::min)(right_base - static_cast<int>(leaf_right->min_px.get_value(area.w())), endpos);

			if (!leaf_right->max_px.empty())
				pos = (std::max)(right_base - static_cast<int>(leaf_right->max_px.get_value(area.w())), pos);

			area.x_ref() = pos;
			area.w_ref() = unsigned(endpos - pos);

			dragger_.target(splitter_, area.result(), (vert ? nana::arrange::vertical : nana::arrange::horizontal));

			return area;
		}

	private:
		implement* const impl_{ nullptr };
		nana::cursor	splitter_cursor_{nana::cursor::arrow};
		adi_place_parts::splitter	splitter_;
		nana::point	begin_point_;
		int			left_pos_{ 0 }, right_pos_{ 0 };
		unsigned	left_pixels_{ 0 }, right_pixels_{ 0 };
		dragger	dragger_;
		bool	grabbed_{ false };
		bool	pause_move_collocate_{ false };	//A flag represents whether do move when collocating.
	};


	class adi_place::implement::div_dockpane
		: public division, public adi_place_parts::dock_notifier_interface
	{
	public:
		div_dockpane(const std::string& name, paneinfo* info, implement* impl) noexcept
			:	division(kind::dockpane, name), impl_ptr_{impl}
		{
			info_ = info;

			//ADI_TODO : migliorare peso iniziale
			if(info_->weight() != 0.f)
				weight_.assign_percent(info_->weight());
		}

		~div_dockpane() noexcept
		{
			if (dockable_field)
			{
				dockable_field->dockarea.reset();
				dockable_field->attached = nullptr;
			}
		}

		bool center() const override
		{
			return info_->center();
		}

		void collocate(window) override
		{
			auto & dockarea = dockable_field->dockarea;
			if(dockarea && !dockarea->floating())
				dockarea->move(field_area);

			if(!div_owner)
				weight_.reset();
		}

		void update()
		{
			//XXX ???
		}

	private:
		//Implement dock_notifier_interface
		void notify_float() override
		{
			impl_ptr_->to_float(this);

			impl_ptr_->collocate();
			//XXX
			impl_ptr_->print_debug();
		}

		void notify_dock() override
		{
			impl_ptr_->hide_indicators();

			impl_ptr_->collocate();
			//XXX
			impl_ptr_->print_debug();
		}

		void notify_move() override
		{
			if(!impl_ptr_->hit_window())
			{
				impl_ptr_->hide_indicators();
				return;
			}

			impl_ptr_->show_indicators();

			auto pane = impl_ptr_->hit_panes();
			if(pane)
				impl_ptr_->show_pane_indicators(pane);
			else
				impl_ptr_->hide_indicators(true);

			dockposition dockpos;
			bool abspos;
			if(impl_ptr_->hit_indicators(&dockpos, &abspos))
			{
				field_area.dimension(dockable_field->dockarea->size());

				if(abspos)
				{
					impl_ptr_->lock_pane_indicators(false);
					impl_ptr_->force_hide_pane_indicators(true);

					if(impl_ptr_->to_dock(this, dockpos))
					{
						impl_ptr_->collocate();
						impl_ptr_->print_debug();
					}
				}
				else
				{
					impl_ptr_->lock_pane_indicators(true);

					if(impl_ptr_->to_dock(this, pane, dockpos))
					{
						impl_ptr_->collocate();
						impl_ptr_->print_debug();
					}
				}
			}
			else
			{
				impl_ptr_->lock_pane_indicators(false);
				impl_ptr_->force_hide_pane_indicators(false);

				if(impl_ptr_->to_float(this))
				{
					impl_ptr_->collocate();
					impl_ptr_->print_debug();
				}
			}
		}

		void notify_move_stopped() override
		{
			if(impl_ptr_->hit_indicators() && dockable_field && dockable_field->dockarea)
				dockable_field->dockarea->dock();

			impl_ptr_->hide_indicators();
		}

		void request_close() override
		{
			auto window_handle = dockable_field->dockarea->handle();

			//a workaround for capture
			auto ptr = dockable_field->dockarea.release();
			API::at_safe_place(window_handle, [ptr]
			{
				std::unique_ptr<typename std::remove_pointer<decltype(ptr)>::type> del(ptr);
			});

			impl_ptr_->collocate();

			API::close_window(window_handle);

			//ADI_TODO //XXX : not sure if this is the right place where to destroy the object
			impl_ptr_->remove(this);
		}

	public:
		field_dock* dockable_field{ nullptr };

	private:
		implement* impl_ptr_;
		paneinfo* info_;
	};


	adi_place::implement::implement()
	{
		abs_indicators.emplace_back(new dock_indicator{ dockposition::up });
		abs_indicators.emplace_back(new dock_indicator{ dockposition::down });
		abs_indicators.emplace_back(new dock_indicator{ dockposition::left });
		abs_indicators.emplace_back(new dock_indicator{ dockposition::right });
		//
		pane_indicators.emplace_back(new dock_indicator{ dockposition::up });
		pane_indicators.emplace_back(new dock_indicator{ dockposition::down });
		pane_indicators.emplace_back(new dock_indicator{ dockposition::left });
		pane_indicators.emplace_back(new dock_indicator{ dockposition::right });
	}

	adi_place::implement::~implement()
	{
		API::umake_event(event_size_handle);
		root_division.reset();

		floating_divs.clear();

		for (auto & dock : docks)
			delete dock.second;
	}

	bool adi_place::implement::is_vertical(dockposition pos)
	{
		return (dockposition::up == pos || dockposition::down == pos) ? true : false;
	}

	bool adi_place::implement::is_before(dockposition pos)
	{
		return (dockposition::up == pos || dockposition::left == pos) ? true : false;
	}

	void adi_place::implement::show_indicators()
	{
		for(auto& i : abs_indicators)
		{
			if(!i->docker)
			{
				auto host_size = API::window_size(window_handle);

				if(dockposition::up == i->pos)
				{
					i->docker.reset(new form(window_handle, { static_cast<int>(host_size.width) / 2 - 16, 5, 32, 32 }, form::appear::bald<>()));
					drawing dw(i->docker->handle());
					dw.draw([](paint::graphics& graph)
						{
							adi_place_parts::_indicator_up(graph);
						});
				}
				else if(dockposition::down == i->pos)
				{
					i->docker.reset(new form(window_handle, { static_cast<int>(host_size.width) / 2 - 16, static_cast<int>(host_size.height) - 32 - 5, 32, 32 }, form::appear::bald<>()));
					drawing dw(i->docker->handle());
					dw.draw([](paint::graphics& graph)
						{
							adi_place_parts::_indicator_down(graph);
						});
				}
				else if(dockposition::left == i->pos)
				{
					i->docker.reset(new form(window_handle, { 5, static_cast<int>(host_size.height) / 2 - 16, 32, 32 }, form::appear::bald<>()));
					drawing dw(i->docker->handle());
					dw.draw([](paint::graphics& graph)
						{
							adi_place_parts::_indicator_left(graph);
						});
				}
				else if(dockposition::right == i->pos)
				{
					i->docker.reset(new form(window_handle, { static_cast<int>(host_size.width) - 32 - 5, static_cast<int>(host_size.height) / 2 - 16, 32, 32 }, form::appear::bald<>()));
					drawing dw(i->docker->handle());
					dw.draw([](paint::graphics& graph)
						{
							adi_place_parts::_indicator_right(graph);
						});
				}
				else
					continue;

				i->docker->z_order(nullptr, ::nana::z_order_action::topmost);
				i->docker->show();

				i->docker->events().destroy.connect([this, &i](const arg_destroy&)
					{
						if(i->dock_area)
						{
							i->dock_area.reset();
							i->graph.release();
						}
					});
			}
		}
	}

	void adi_place::implement::show_pane_indicators(division* div)
	{
		if(!div || dipanes_hide_forced || dipanes_locked)
			return;

		print_debug();

		for(auto& i : pane_indicators)
		{
			auto x = div->field_area.x + static_cast<int>(div->field_area.width) / 2;
			auto y = div->field_area.y + static_cast<int>(div->field_area.height) / 2;

			if(!i->docker)
			{
				if(dockposition::up == i->pos)
				{
					auto pos = point(x - 16, y - 16 - 32);
					i->docker.reset(new form(window_handle, { pos.x, pos.y, 32, 32 }, form::appear::bald<>()));
					i->delta = i->docker->pos() - pos;
					drawing dw(i->docker->handle());
					dw.draw([](paint::graphics& graph)
						{
							adi_place_parts::_indicator_up(graph);
						});
				}
				else if(dockposition::down == i->pos)
				{
					auto pos = point(x - 16, y + 16);
					i->docker.reset(new form(window_handle, { pos.x, pos.y, 32, 32 }, form::appear::bald<>()));
					i->delta = i->docker->pos() - pos;
					drawing dw(i->docker->handle());
					dw.draw([](paint::graphics& graph)
						{
							adi_place_parts::_indicator_down(graph);
						});
				}
				else if(dockposition::left == i->pos)
				{
					auto pos = point(x - 16 - 32, y - 16);
					i->docker.reset(new form(window_handle, { pos.x, pos.y, 32, 32 }, form::appear::bald<>()));
					i->delta = i->docker->pos() - pos;
					drawing dw(i->docker->handle());
					dw.draw([](paint::graphics& graph)
						{
							adi_place_parts::_indicator_left(graph);
						});
				}
				else if(dockposition::right == i->pos)
				{
					auto pos = point(x + 16, y - 16);
					i->docker.reset(new form(window_handle, { pos.x, pos.y, 32, 32 }, form::appear::bald<>()));
					i->delta = i->docker->pos() - pos;
					drawing dw(i->docker->handle());
					dw.draw([](paint::graphics& graph)
						{
							adi_place_parts::_indicator_right(graph);
						});
				}
				else
					continue;

				i->docker->z_order(nullptr, ::nana::z_order_action::topmost);
				i->docker->show();

				i->docker->events().destroy.connect([this, &i](const arg_destroy&)
					{
						if(i->dock_area)
						{
							i->dock_area.reset();
							i->graph.release();
						}
					});
			}
			else
			{
				if(dockposition::up == i->pos)
				{
					auto pos = point(x - 16, y - 16 - 32);
					i->docker->move(pos + i->delta);
				}
				else if(dockposition::down == i->pos)
				{
					auto pos = point(x - 16, y + 16);
					i->docker->move(pos + i->delta);
				}
				else if(dockposition::left == i->pos)
				{
					auto pos = point(x - 16 - 32, y - 16);
					i->docker->move(pos + i->delta);
				}
				else if(dockposition::right == i->pos)
				{
					auto pos = point(x + 16, y - 16);
					i->docker->move(pos + i->delta);
				}
				else
					continue;
			}
		}
	}

	void adi_place::implement::hide_indicators(bool pane_only)
	{
		if(pane_only)
		{
			if(!dipanes_locked)
				for(auto& i : pane_indicators)
					i->docker.reset();
		}
		else
		{
			for(auto& i : abs_indicators)
				i->docker.reset();

			for(auto& i : pane_indicators)
				i->docker.reset();

			lock_pane_indicators(false);
		}
	}

	void adi_place::implement::force_hide_pane_indicators(bool force)
	{
		dipanes_hide_forced = force;
		if(dipanes_hide_forced)
			hide_indicators(true);
	}

	void adi_place::implement::lock_pane_indicators(bool lock)
	{
		dipanes_locked = lock;
	}

	bool adi_place::implement::to_float(division* div)
	{
		auto owner = div->div_owner;
		if(!owner)
		{
			if(root_division && root_division.get() == div)
			{
				// move the div to the support vector "floating_divs"
				floating_divs.emplace_back(root_division.release());
				return true;
			}
			return false;
		}

		for(size_t i = 0; i < owner->children.size(); ++i)
		{
			if(owner->children[i].get() == div)
			{
				// move the div to the support vector "floating_divs"
				floating_divs.emplace_back(owner->children[i].release());
				floating_divs.back()->div_owner = nullptr;
				floating_divs.back()->weight_.reset();
				
				// remove the children from the divs tree
				if(i == owner->children.size() - 1)
				{
					owner->children.pop_back();

					if(owner->children.size())
						// remove the splitter bar
						owner->children.pop_back();
				}
				else
				{
					owner->children.erase(owner->children.begin() + i);
					if(owner->children.size())
						// remove the splitter bar
						owner->children.erase(owner->children.begin() + i);
				}
				break;
			}
		}

		return true;
	}

	bool adi_place::implement::to_dock(division* div, dockposition pos)
	{
		return to_dock(div, nullptr, pos);
	}

	bool adi_place::implement::to_dock(division* div, division* where, dockposition pos)
	{
		// look into floating_divs for div to dock
		size_t i_div = 0;
		for(; i_div < floating_divs.size(); ++i_div)
			if(floating_divs[i_div].get() == div)
				break;

		if(i_div == floating_divs.size())
			return false;

		if(!root_division)
		{
			// the divs tree is empty: append div to divs tree root
			root_division = std::move(floating_divs[i_div]);
			// remove div from detached divs
			floating_divs.erase(floating_divs.begin() + i_div);
			return true;
		}


		bool add_new_arrange = false;
		if(root_division->kind_of_division == division::kind::dockpane)
			// in the divs tree there is only one dock
			add_new_arrange = true;
		else
		{
			// check direction of the div_arrange where to dock
			auto where_owner = static_cast<div_arrange*>(where ? where->div_owner : root_division.get());
			if(where_owner->is_vertical() != is_vertical(pos))
				// owner direction is different
				add_new_arrange = true;
		}


		if(add_new_arrange)
		{
			std::unique_ptr<division> where_uptr;
			auto where_owner = where ? where->div_owner : root_division.get();
			size_t i_where = std::string::npos;

			if(where)
			{
				i_where = where->index();
				if(i_where == std::string::npos)
					// divs tree corrupted
					return false;

				where_uptr = std::move(where->div_owner->children[i_where]);
			}
			else
				where_uptr = std::move(root_division);


			auto arrange_ptr = new div_arrange(is_vertical(pos), "");
			if(where)
			{
				arrange_ptr->div_owner = where->div_owner;
				arrange_ptr->weight_.assign(static_cast<int>(arrange_ptr->is_vertical() ? where_uptr->field_area.width : where_uptr->field_area.height));
			}

			auto splitter_ptr = new div_splitter(this, is_vertical(pos));
			splitter_ptr->div_owner = arrange_ptr;

			div->div_owner = arrange_ptr;
			if(div->weight_.empty())
			{
				div->weight_.assign(static_cast<int>(is_vertical(pos) ? div->field_area.height : div->field_area.width));

				// check the available space and adjust the weight accordingly
				int avail = (is_vertical(pos) ? where_uptr->field_area.height : where_uptr->field_area.width) - where_uptr->min_px.integer() - scheme.splitter_width;
				if(div->weight_.integer() == 0 || avail < 0 || div->weight_.integer() >= avail)
					div->weight_.reset();
			}

			where_uptr->div_owner = arrange_ptr;
			if(!div->weight_.empty())
				where_uptr->weight_.reset();

			if(is_before(pos))
			{
				// DIV | WHERE

				// append div
				arrange_ptr->children.emplace_back(floating_divs[i_div].release());
				// append splitter
				arrange_ptr->children.emplace_back(splitter_ptr);
				// append where
				arrange_ptr->children.emplace_back(where_uptr.release());
			}
			else
			{
				// WHERE | DIV

				// append where
				arrange_ptr->children.emplace_back(where_uptr.release());
				// append splitter
				arrange_ptr->children.emplace_back(splitter_ptr);
				// append div
				arrange_ptr->children.emplace_back(floating_divs[i_div].release());
			}

			// remove div from detached divs
			floating_divs.erase(floating_divs.begin() + i_div);

			// append the new arrange
			if(i_where == std::string::npos)
				root_division.reset(arrange_ptr);
			else
				where_owner->children[i_where].reset(arrange_ptr);
		}
		else
		{
			auto where_owner = where ? where->div_owner : root_division.get();
			size_t i_where = where ? where->index() : std::string::npos;

			auto splitter_ptr = new div_splitter(this, is_vertical(pos));
			splitter_ptr->div_owner = where_owner;

			div->div_owner = where_owner;
			if(div->weight_.empty())
			{
				// check available space
				int children_fixed_px = 0;
				for(auto& child : where_owner->children)
					if(!child->weight_.empty())
						children_fixed_px += child->weight_.integer();
					else
						children_fixed_px += child->min_px.integer();
				children_fixed_px += scheme.splitter_width;
				int avail = (is_vertical(pos) ? where_owner->field_area.height : where_owner->field_area.width) - children_fixed_px;
				// adjust the weight according with the available space
				if(avail < div->min_px.integer())
					div->weight_.assign(static_cast<int>(is_vertical(pos) ? div->field_area.height : div->field_area.width));
				else
					div->weight_.assign(std::min(avail, static_cast<int>(is_vertical(pos) ? div->field_area.height : div->field_area.width)));
				
				if(div->weight_.integer() == 0)
					div->weight_.reset();
			}

			if(is_before(pos))
			{
				if(i_where == std::string::npos)
					i_where = 0;

				// DIV | children[i_where]

				// append div
				where_owner->children.emplace(where_owner->children.begin() + i_where, floating_divs[i_div].release());
				// append splitter
				where_owner->children.emplace(where_owner->children.begin() + i_where + 1, splitter_ptr);
			}
			else
			{
				if(i_where == std::string::npos)
					i_where = where_owner->children.size() - 1;

				// children[i_where] | DIV

				// append splitter
				where_owner->children.emplace(where_owner->children.begin() + i_where + 1, splitter_ptr);
				// append div
				where_owner->children.emplace(where_owner->children.begin() + i_where + 2, floating_divs[i_div].release());
			}

			// remove div from detached divs
			floating_divs.erase(floating_divs.begin() + i_div);
		}

		return true;
	}

	bool adi_place::implement::remove(division* div)
	{
		// move div to detached divs vector
		to_float(div);

		size_t i_div = 0;
		for(; i_div < floating_divs.size(); ++i_div)
			if(floating_divs[i_div].get() == div)
				break;

		if(i_div == floating_divs.size())
			return false;

		// remove div from detached divs
		floating_divs.erase(floating_divs.begin() + i_div);
		return true;
	}

	bool adi_place::implement::hit_window() const
	{
		point pos;
		API::calc_screen_point(window_handle, pos);
		return rectangle{ pos, API::window_size(window_handle) }.is_hit(API::cursor_position());
	}

	bool adi_place::implement::hit_indicators(dockposition* position, bool* absolute) const
	{
		for(auto& i : abs_indicators)
		{
			if(!i->docker)
				continue;

			point p;
			API::calc_screen_point(i->docker->handle(), p);
			if(rectangle{ p, API::window_size(i->docker->handle()) }.is_hit(API::cursor_position()))
			{
				if(position)
					*position = i->pos;
				if(absolute)
					*absolute = true;
				return true;
			}
		}

		for(auto& i : pane_indicators)
		{
			if(!i->docker)
				continue;

			point p;
			API::calc_screen_point(i->docker->handle(), p);
			if(rectangle{ p, API::window_size(i->docker->handle()) }.is_hit(API::cursor_position()))
			{
				if(position)
					* position = i->pos;
				if(absolute)
					* absolute = false;
				return true;
			}
		}

		return false;
	}

	adi_place::implement::division* adi_place::implement::hit_panes()
	{
		if(!root_division || (division::kind::dockpane == root_division->kind_of_division))
			return nullptr;

		std::function<implement::division* (implement::division * div)> hit_fn;
		hit_fn = [&hit_fn, this](implement::division* div) -> implement::division *
		{
			if(division::kind::dockpane == div->kind_of_division)
			{
				point pos;
				API::calc_screen_point(window_handle, pos);
				if(rectangle{ pos + div->field_area.position(), div->field_area.dimension() }.is_hit(API::cursor_position()))
					return div;
			}

			for(auto& child : div->children)
			{
				auto r = hit_fn(child.get());
				if(r)
					return r;
			}

			return nullptr;
		};

		return hit_fn(root_division.get());
	}


	void adi_place::implement::print_debug()
	{
#ifdef __DEBUG
		unsigned level = 0;

		printf("\n\n - div() -\n");
		if(!root_division)
		{
			printf("\t<empty>\n\n");
			return;
		}

		std::function<void(implement::division * div)> print_fn;
		print_fn = [&print_fn, &level](implement::division* div)
		{
			for(unsigned i = 0; i < level; ++i)
				printf("\t");

			if(division::kind::splitter == div->kind_of_division)
				printf("| splitter (%s)\n", div->is_vertical() ? "V" : "H");
			else
			{
				printf("%s [%.2f", div->name.size() ? div->name.c_str() : "empty", div->weight_.get_value(100));
				if(div->is_percent())
					printf("%%] - ");
				else
					printf("] - ");
				
				if(division::kind::dockpane == div->kind_of_division)
					printf("dockpane\n");
				else if(division::kind::arrange == div->kind_of_division)
					printf("arrange (%s)\n", div->is_vertical() ? "V" : "H");
				else
					printf("unknown !!!\n");
			}

			++level;
			for(auto& child : div->children)
				print_fn(child.get());
			--level;
		};

		print_fn(root_division.get());
		printf("\n\n\n");
#endif
	}

	void adi_place::implement::purge()
	{
		if(!root_division)
			return;

		std::function<void(implement::division* div)> _purge_fn;
		_purge_fn = [&_purge_fn](implement::division* div)
		{
			if(!div)
				return;

			for(size_t i = 0; i < div->children.size(); ++i)
			{
				_purge_fn(div->children[i].get());

				bool to_remove = false;

				if(division::kind::dockpane == div->children[i]->kind_of_division)
				{
					// remove dockpane without pane
					auto const dock = dynamic_cast<implement::div_dockpane*>(div->children[i].get());
					if(!dock->dockable_field)
						to_remove = true;
				}
				else if(division::kind::arrange == div->children[i]->kind_of_division)
				{
					if(div->children[i]->children.empty())
						to_remove = true;
					else if(div->children[i]->children.size() == 1)
					{
						std::unique_ptr<implement::division> child = std::move(div->children[i]->children[0]);

						child->div_owner = div->children[i]->div_owner;
						child->weight_ = div->children[i]->weight_;

						div->children[i] = std::move(child);
					}
				}

				if(to_remove)
				{
					if(i == div->children.size() - 1)
					{
						div->children.pop_back();

						// remove the splitter bar
						if(div->children.size())
							div->children.pop_back();
					}
					else
					{
						div->children.erase(div->children.begin() + i);

						// remove the splitter bar
						if(div->children.size())
							div->children.erase(div->children.begin() + i);

						--i;
					}
				}
			}
		};

		_purge_fn(root_division.get());

		while(root_division->children.size() == 1)
		{
			if(division::kind::arrange == root_division->kind_of_division)
			{
				std::unique_ptr<implement::division> child;
				child.swap(root_division->children[0]);

				child->div_owner = root_division->div_owner;
				child->field_area = root_division->field_area;

				root_division.swap(child);
			}
			else
				break;
		}
	}

	void adi_place::implement::collocate()
	{
		if(root_division && window_handle)
		{
			purge();

			root_division->field_area.dimension(API::window_size(window_handle));

			if(root_division->field_area.empty())
				return;

			root_division->collocate(window_handle);

			API::refresh_window(window_handle);
		}
	}


	//class adi_place
	adi_place::adi_place()
		: impl_(new implement)
	{}

	adi_place::adi_place(window wd)
		: impl_(new implement)
	{
		bind(wd);
	}

	adi_place::~adi_place()
	{
		delete impl_;
	}

	void adi_place::bind(window wd)
	{
		if (impl_->window_handle)
			throw error(" bind('"+ API::window_caption(wd).substr(0, 80) + "'): it was already bound to another window.", *this);

		impl_->window_handle = wd;
		impl_->event_size_handle = API::events(wd).resized.connect_unignorable([this](const arg_resized& arg)
		{
			if (impl_->root_division)
			{
				impl_->root_division->field_area.dimension({ arg.width, arg.height });
				impl_->root_division->collocate(arg.window_handle);

				impl_->print_debug();
			}
		});
	}

	window adi_place::window_handle() const
	{
		return impl_->window_handle;
	}

	void adi_place::splitter_renderer(std::function<void(window, paint::graphics&, mouse_action)> fn)
	{
		impl_->split_renderer.swap(fn);

		for(auto sp : impl_->splitters)
			sp->set_renderer(impl_->split_renderer, true);
	}

	void adi_place::collocate()
	{
		impl_->collocate();
	}

	paneinfo adi_place::add_pane(const std::string& id, widget* w, const std::string& caption)
	{
		paneinfo i(id);
		i.caption(caption);
		return add_pane(i, w);
	}

	paneinfo adi_place::add_pane(const std::string& id, widget* w, dockposition position, const std::string& caption)
	{
		paneinfo i(id);
		i.caption(caption);
		return add_pane(i, w, position);
	}

	paneinfo adi_place::add_pane(const paneinfo& info, widget* w)
	{
		return add_pane(info, w, dockposition::right);
	}

	paneinfo adi_place::add_pane(const paneinfo& info, widget* w, dockposition position)
	{
		if(info.empty())
		{
			throw error("add_pane() - the id cannot be empty", *this);
			return paneinfo();
		}

		auto& dock_ptr = impl_->docks[info.id()];
		if(dock_ptr)
		{
			throw error("add_pane() - the specified id (" + info.id() + ") already exists", *this);
			return paneinfo();
		}

		dock_ptr = new implement::field_dock;
		dock_ptr->info = info;

		auto* div = new implement::div_dockpane(info.id(), &dock_ptr->info, impl_);
		div->dockable_field = dock_ptr;
		dock_ptr->attached = div;

		impl_->floating_divs.emplace_back(div);

		dock_ptr->dockarea.reset(new ::nana::adi_place_parts::dockarea);
		dock_ptr->dockarea->create(impl_->window_handle, &dock_ptr->info, &impl_->scheme);
		dock_ptr->dockarea->set_notifier(dock_ptr->attached);
		dock_ptr->dockarea->move(dock_ptr->attached->field_area);
		dock_ptr->dockarea->add_pane(w);

		if(impl_->to_dock(div, position))
			return dock_ptr->info;
		
		throw error("add_pane() -> to_dock() - possible divs tree corruption", *this);
		return paneinfo();
	}

	paneinfo adi_place::get_pane(const std::string& id)
	{
		auto it = impl_->docks.find(id);
		if(it != impl_->docks.end())
			return it->second->info;

		return paneinfo();
	}

	void adi_place::update_pane(const paneinfo& info)
	{
		auto it = impl_->docks.find(info.id());
		if(it != impl_->docks.end())
		{
			it->second->info = info;
			it->second->attached->update();
			it->second->dockarea->update();
		}
	}
	
	adi_place::scheme_t& adi_place::scheme()
	{
		return impl_->scheme;
	}


	adi_place::error::error(const std::string& what, const adi_place& plc)

		: std::invalid_argument(  "from widget '" 
		                        + API::window_caption(plc.window_handle()).substr(0,80)
								+ "'; nana::adi_place error "
		                        + what),
		base_what( what ),
		owner_caption( API::window_caption(plc.window_handle()).substr(0,80) )
	{}
	//end class adi_place

}//end namespace nana

#include <nana/pop_ignore_diagnostic>
