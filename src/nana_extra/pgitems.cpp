/*
 *		nana::propgrid_items Implementation
 *
 *      part of Nana Creator (https://github.com/besh81/nana-creator)
 *      Author: besh81
 */

#include <iostream>
#include <algorithm>
#include "pgitems.h"
#include "color_helper.h"
#include "lock_guard.h"



namespace nana
{
	/// class pg_string
	void pg_string::value(const std::string& value)
	{
		lock_guard evt_lock(&evt_emit_, false);
		txt_.caption(value);
		
		pgitem::value(value);
	}

	void pg_string::enabled(bool state)
	{
		pgitem::enabled(state);
		txt_.enabled(en_);
	}

	void pg_string::set_accept(std::function<bool(wchar_t)> f)
	{
		txt_.set_accept(f);
	}

	void pg_string::editable(bool editable)
	{
		txt_.editable(editable);
	}
	bool pg_string::editable()
	{
		return txt_.editable();
	}

	void pg_string::create(window wd)
	{
		txt_.create(wd);
		txt_.multi_lines(false);
		txt_.caption(value_);

		txt_.events().click.connect_front([this](const nana::arg_click& arg)
		{
			scroll();
		});
		txt_.events().dbl_click([this](const nana::arg_mouse& arg)
		{
			txt_.select(true);
		});
		txt_.events().key_press([this](const nana::arg_keyboard& arg)
		{
			if(arg.key == nana::keyboard::enter)
			{
				pgitem::value(txt_.caption());
				emit_event();
			}
		});
	}

	void pg_string::draw_value(paint::graphics* graph, rectangle rect, const int txtoff, color bgcolor, color fgcolor) const
	{
		txt_.move(rect.x + PG_BORDER_X, PG_BORDER_Y);
		txt_.size(nana::size(rect.width - 2* PG_BORDER_X, size_ - 2* PG_BORDER_Y));
	}
	/// class pg_string end


	/// class pg_string_int
	void pg_string_int::value(int value)
	{
		pg_string::value(std::to_string(value));
	}
	int pg_string_int::to_int() const
	{
		int int_val = -1;
		try
		{
			int_val = std::stoi(pgitem::value());
		}
		catch(...)
		{
		}
		return int_val;
	}

	void pg_string_int::range(int min, int max)
	{
		if(min >= max)
		{
			range_ = false;
			return;
		}

		range_ = true;
		min_ = min;
		max_ = max;

		value(std::clamp(to_int(), min_, max_));
	}

	void pg_string_int::create(window wd)
	{
		pg_string::create(wd);

		txt_.events().key_press.connect_front([this](const nana::arg_keyboard& arg)
		{
			if(arg.key == nana::keyboard::enter)
			{
				int int_val = -1;
				try
				{
					int_val = std::stoi(txt_.caption());
					if(range_)
						int_val = std::clamp(int_val, min_, max_);
				}
				catch(...)
				{
					txt_.caption(value_);
					arg.stop_propagation();
					return;
				}

				pg_string::value(std::to_string(int_val));

				emit_event();
				arg.stop_propagation();
			}
		});

		txt_.set_accept([](wchar_t c) -> bool
		{
			return (isdigit(c) || c == '-' || c == nana::keyboard::cancel || c == nana::keyboard::backspace) ? true : false;
		});
	}
	/// class pg_string_int end


	/// class pg_string_uint
	void pg_string_uint::value(unsigned value)
	{
		pg_string::value(std::to_string(value));
	}
	unsigned pg_string_uint::to_uint() const
	{
		unsigned u_val = -1;
		try
		{
			u_val = std::stoul(pgitem::value());
		}
		catch(...)
		{
		}
		return u_val;
	}

	void pg_string_uint::range(unsigned min, unsigned max)
	{
		if(min >= max)
		{
			range_ = false;
			return;
		}

		range_ = true;
		min_ = min;
		max_ = max;

		value(std::clamp(to_uint(), min_, max_));
	}

	void pg_string_uint::create(window wd)
	{
		pg_string::create(wd);

		txt_.events().key_press.connect_front([this](const nana::arg_keyboard& arg)
		{
			if(arg.key == nana::keyboard::enter)
			{
				unsigned u_val = -1;
				try
				{
					u_val = std::stoul(txt_.caption());
					if(range_)
						u_val = std::clamp(u_val, min_, max_);
				}
				catch(...)
				{
					txt_.caption(value_);
					arg.stop_propagation();
					return;
				}

				pg_string::value(std::to_string(u_val));

				emit_event();
				arg.stop_propagation();
			}
		});

		txt_.set_accept([](wchar_t c) -> bool
		{
			return (isdigit(c) || c == nana::keyboard::cancel || c == nana::keyboard::backspace) ? true : false;
		});
	}
	/// class pg_string_uint end


	/// class pg_string_button
#define STRING_BUTTON_SIZE		30

	void pg_string_button::value(const std::string& value)
	{
		value_ = value;

		lock_guard evt_lock(&evt_emit_, false);
		txt_.caption(value);
	}

	void pg_string_button::enabled(bool state)
	{
		pgitem::enabled(state);
		txt_.enabled(en_);
		btn_.enabled(en_);
	}

	void pg_string_button::set_accept(std::function<bool(wchar_t)> f)
	{
		txt_.set_accept(f);
	}

	void pg_string_button::set_button_click(std::function<void(const nana::arg_click&)> f)
	{
		btn_.events().click.connect_front(f);
	}

	void pg_string_button::editable(bool editable)
	{
		txt_.editable(editable);
	}
	bool pg_string_button::editable() const
	{
		return txt_.editable();
	}

	void pg_string_button::create(window wd)
	{
		txt_.create(wd);
		txt_.multi_lines(false);
		txt_.caption(value_);


		txt_.set_accept([this](wchar_t c) -> bool
		{
			return (((c == nana::keyboard::cancel || c == nana::keyboard::backspace) && txt_.selected()) || c == nana::keyboard::enter) ? true : false;
		});
		txt_.events().click.connect_front([this](const nana::arg_click& arg)
		{
			txt_.select(true);
		});
		txt_.events().dbl_click([this](const nana::arg_mouse& arg)
		{
			txt_.select(true);
		});
		txt_.events().key_press([this](const nana::arg_keyboard& arg)
		{
			if(arg.key == nana::keyboard::enter && txt_.editable())
			{
				pgitem::value(txt_.caption());
				emit_event();
			}
		});


		//button
		btn_.create(wd);
		btn_.caption(btn_label_);
	}

	void pg_string_button::draw_value(paint::graphics* graph, rectangle rect, const int txtoff, color bgcolor, color fgcolor) const
	{
		txt_.move(rect.x + PG_BORDER_X, PG_BORDER_Y);
		txt_.size(nana::size(rect.width - 2 * PG_BORDER_X - STRING_BUTTON_SIZE, size_ - 2 * PG_BORDER_Y));

		btn_.move(rect.x + PG_BORDER_X + txt_.size().width, PG_BORDER_Y);
		btn_.size(nana::size(STRING_BUTTON_SIZE, size_ - 2 * PG_BORDER_Y));
	}
	/// class pg_string_button end


	/// class pg_choice
	void pg_choice::value(const std::string& value_)
	{
		try
		{
			option(std::stoul(pgitem::value()));
		}
		catch(...)
		{
		}
	}

	void pg_choice::enabled(bool state)
	{
		pgitem::enabled(state);
		cmb_.enabled(en_);
	}

	void pg_choice::option(std::size_t value)
	{
		lock_guard evt_lock(&evt_emit_, false);
		if(value < cmb_.the_number_of_options())
		{
			cmb_.option(value);
			pgitem::value(std::to_string(value));
		}
	}
	std::size_t pg_choice::option() const
	{
		std::size_t val = nana::npos;
		try
		{
			val = std::stoul(pgitem::value());
		}
		catch(...)
		{
		}
		return val;
	}

	void pg_choice::push_back(const std::string& option)
	{
		cmb_.push_back(option);
	}

	void pg_choice::create(window wd)
	{
		cmb_.create(wd);

		cmb_.events().click.connect_front([this](const nana::arg_click& arg)
		{
			scroll();
		});
		cmb_.events().selected([this](const nana::arg_combox& arg)
		{
			if(!evt_emit_)
				return;

			option(arg.widget.option());
			emit_event();
		});
	}

	void pg_choice::draw_value(paint::graphics* graph, rectangle rect, const int txtoff, color bgcolor, color fgcolor) const
	{
		cmb_.move(rect.x + PG_BORDER_X, PG_BORDER_Y);
		cmb_.size(nana::size(rect.width - 2 * PG_BORDER_X, size_ - 2 * PG_BORDER_Y));
	}
	/// class pg_choice end


	/// class pg_check
	void pg_check::value(const std::string& value)
	{
		lock_guard evt_lock(&evt_emit_, false);
		if(value == "true" || value == "T" || value == "t" || value == "1")
		{
			chk_.check(true);
			pgitem::value("true");
		}
		else if(value == "false" || value == "F" || value == "f" || value == "0")
		{
			chk_.check(false);
			pgitem::value("false");
		}
	}

	void pg_check::enabled(bool state)
	{
		pgitem::enabled(state);
		chk_.enabled(en_);
	}

	void pg_check::create(window wd)
	{
		chk_.create(wd);
		value(value_); // set the initial value passed in ctr

		chk_.events().click.connect_front([this](const nana::arg_click& arg)
		{
			scroll();
		});
		chk_.events().checked([this](const nana::arg_checkbox& arg)
		{
			if(!evt_emit_)
				return;

			check(arg.widget->checked());
			emit_event();
		});
	}

	void pg_check::draw_value(paint::graphics* graph, rectangle rect, const int txtoff, color bgcolor, color fgcolor) const
	{
		chk_.bgcolor(bgcolor);

		chk_.move(rect.x + PG_BORDER_X, PG_BORDER_Y);
		auto chksize = size_ - 2 * PG_BORDER_Y;
		chk_.size(nana::size(chksize, chksize));
	}
	/// class pg_check end


	/// class pg_color
	void pg_color::value(const std::string& value)
	{
		lock_guard evt_lock(&evt_emit_, false);

		color_ = nana::to_color(value);
		rgb_[0].caption(std::to_string(static_cast<int>(color_.r())));
		rgb_[1].caption(std::to_string(static_cast<int>(color_.g())));
		rgb_[2].caption(std::to_string(static_cast<int>(color_.b())));

		if(show_inherited_)
		{
			inherited_ = is_color_inherited(value);
			menu_.checked(2, inherited_);

			for(auto & i : rgb_)
				i.enabled(en_ && !inherited_);
		}

		pgitem::value(nana::to_string(color_, show_inherited_ ? inherited_ : false));
	}

	void pg_color::enabled(bool state)
	{
		pgitem::enabled(state);

		for(auto & i : rgb_)
			i.enabled(en_ && !inherited_);
	}

	void pg_color::value(::nana::color value)
	{
		pg_color::value(nana::to_string(value, show_inherited_ ? inherited_ : false));
	}

	void pg_color::inherited(bool value)
	{
		if(show_inherited_)
			pg_color::value(nana::to_string(color_, value));
	}

	unsigned pg_color::size() const
	{
		return expand_ ? 2 * size_ : size_;
	}

	void pg_color::create(window wd)
	{
		// ibox context menu
		menu_.append_splitter();
		// 2. Inherited
		menu_.append("Inherited", [this](const nana::menu::item_proxy& ip)
		{
			pg_color::value(nana::to_string(color_, ip.checked()));
			printf(ip.checked() ? "[X]\n" : "[ ]\n");
			emit_event();
		});
		menu_.enabled(2, show_inherited_);
		menu_.check_style(2, nana::menu::checks::highlight);


		// colorbox
		colorbox_.create(wd);
		colorbox_.editable(false);

		colorbox_.events().mouse_down.connect_front([this](const nana::arg_mouse& arg)
		{
			if(!en_) // only if enabled
				return;

			if(arg.left_button)
			{
				expand_ = !expand_;
				scroll();
				update();
			}
		});


		// textboxes
		for(auto & i : rgb_)
		{
			i.create(wd);
			i.multi_lines(false);

			i.events().click.connect_front([this](const nana::arg_click& arg)
			{
				scroll();
			});
			i.events().dbl_click([this, &i](const nana::arg_mouse& arg)
			{
				i.select(true);
			});
			i.events().key_press([this](const nana::arg_keyboard& arg)
			{
				if(arg.key == nana::keyboard::enter)
				{
					pg_color::value(nana::to_string(nana::to_color(rgb_[0].caption(), rgb_[1].caption(), rgb_[2].caption()), show_inherited_ ? inherited_ : false));
					emit_event();
				}
			});
			i.set_accept([](wchar_t c) -> bool
			{
				return (isdigit(c) || c == nana::keyboard::cancel || c == nana::keyboard::backspace) ? true : false;
			});
		}

		pg_color::value(value_);
	}

	void pg_color::draw(paint::graphics* graph, rectangle area, unsigned labelw, unsigned  valuew, unsigned  iboxw, const int txtoff, color bgcolor, color fgcolor) const
	{
		// background
		graph->rectangle(area, true, bgcolor);

		// draw label
		if(labelw)
		{
			auto rect = area;
			rect.width = labelw;
			draw_label(graph, rect, txtoff, bgcolor, fgcolor);
		}

		// draw value - colorbox
		if(valuew)
		{
			auto rect = area;
			rect.x += labelw;
			rect.width = valuew;
			colorbox_.move(rect.x + PG_BORDER_X, PG_BORDER_Y);
			colorbox_.size(nana::size(rect.width - 2 * PG_BORDER_X, size_ - 2 * PG_BORDER_Y));
			colorbox_.bgcolor(color_);
		}

		// draw interaction-box
		if(iboxw)
		{
			auto rect = area;
			rect.x += labelw + valuew;
			rect.width = iboxw;
			draw_ibox(graph, rect, bgcolor, fgcolor);
		}

		// draw expanded
		if(!expand_)
		{
			for(auto & i : rgb_)
				API::show_window(i, false);
			return;
		}

		if(!en_)
			fgcolor = colors::gray; //TODO: scheme


		for(auto & i : rgb_)
			API::show_window(i, true);

		
		// colorbox R G B
		int availw = labelw + valuew;
		if(availw)
		{
			auto txtsize = graph->text_extent_size("R");
			int x = 75;
			int y = size_ + static_cast<int>(size_ - txtsize.height) / 2;
			int ctrly = size_;
			const int offset = 8;

			availw -= x;
			const unsigned txtw = txtsize.width + 5;
			const unsigned ctrlw = (availw - (3 * txtw) - (2 * offset)) / 3;

			// R
			graph->string(point{ area.x + x, area.y + y }, "R", fgcolor);
			x += txtw;
			rgb_[0].move(x, ctrly);
			rgb_[0].size(nana::size(ctrlw, size_ - 2 * PG_BORDER_Y));
			x += ctrlw + offset;
			// G
			graph->string(point{ area.x + x, area.y + y }, "G", fgcolor);
			x += txtw;
			rgb_[1].move(x, ctrly);
			rgb_[1].size(nana::size(ctrlw, size_ - 2 * PG_BORDER_Y));
			x += ctrlw + offset;
			// B
			graph->string(point{ area.x + x, area.y + y }, "B", fgcolor);
			x += txtw;
			rgb_[2].move(x, ctrly);
			rgb_[2].size(nana::size(ctrlw, size_ - 2 * PG_BORDER_Y));
		}
	}
	/// class pg_color end

}//end namespace nana
