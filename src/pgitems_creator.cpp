/*
 *		nana::pgitems_creator Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "pgitems_creator.h"
#include <nana/gui/filebox.hpp>
#include "items_dialog.h"
#include "filemanager.h"
#include "inifile.h"
#include "lock_guard.h"
#include "nana_extra/color_helper.h"


extern filemanager		g_file_mgr;
extern inifile			g_inifile;



/// class pg_filename
void pg_filename::value(const std::string& value)
{
	auto ep = equalize_path(value);

	lock_guard evt_lock(&evt_emit_, false);
	txt_.caption(g_file_mgr.to_relative(value));

	pgitem::value(value);
}

void pg_filename::create(nana::window wd)
{
	pg_string_button::create(wd);

	//textbox
	value(value_);

	//button
	set_button_click([this](const nana::arg_click& arg)
	{
		if(!on_open_dlg())
			return;

		nana::filebox fb(arg.window_handle, true);
		
		for(auto i : filters_)
			fb.add_filter(i.first, i.second);
		fb.add_filter("All Files", "*.*");

		if(value_.empty())
		#if defined(NANA_WINDOWS)
		{
			// solve the problem with lpstrinitialdir
			auto p = equalize_path(init_dir_, '/', '\\');
			fb.init_file(p.empty() ? "./" : p + "\\.");
		}
		#else
			fb.init_path(init_dir_);
		#endif
		else
			fb.init_file(value_);

		auto paths = fb.show();
		if(!paths.empty())
		{
			auto filename = paths[0];
			if(!on_close_dlg(filename.string()))
				return;

			value(filename.string());
			emit_event();
		}
	});
}
/// class pg_filename end



/// class pg_image
void pg_image::create(nana::window wd)
{
	pg_filename::create(wd);

	add_filter("Image Files (" CREATOR_SUPPORTED_IMG ")", CREATOR_SUPPORTED_IMG);
}

bool pg_image::on_open_dlg()
{
	init_path(equalize_path(g_inifile.image_dir()));
	return true;
}

bool pg_image::on_close_dlg(const std::string& file)
{
	nana::filebox::path_type path(equalize_path(file));

	// save image folder
	if(path.parent_path().string() != g_inifile.image_dir())
		g_inifile.image_dir(path.parent_path().string(), true);
	return true;
}
/// class pg_image end



/// class pg_folder
void pg_folder::value(const std::string& value)
{
	auto ep = equalize_path(value);

	lock_guard evt_lock(&evt_emit_, false);
	txt_.caption(ep);

	pgitem::value(ep);
}

void pg_folder::create(nana::window wd)
{
	pg_string_button::create(wd);

	//textbox
	value(value_);

	//button
	set_button_click([this](const nana::arg_click& arg)
	{
		nana::folderbox folder_picker(arg.window_handle, value_);

		auto paths = folder_picker.show();
		if(!paths.empty())
		{
			value(paths[0].string());
			emit_event();
		}
	});
}
/// class pg_folder end



/// class pg_collection
void pg_collection::defvalue(const std::string& value)
{
	if(items_)
		if((*items_).size())
		{
			pgitem::defvalue("items");
			return;
		}

	pgitem::defvalue(pgitem::value());
}

void pg_collection::items(std::vector<ctrls::properties_collection>* items)
{
	items_ = items;
	defvalue("");
}

void pg_collection::reset()
{
	if(items_)
		(*items_).clear();
	defvalue("");
}

void pg_collection::create(nana::window wd)
{
	pg_string_button::create(wd);
	pg_string_button::value("(Collection)");
	defvalue("");
	editable(false);

	//button
	set_button_click([this](const nana::arg_click& arg)
	{
		//items dialog
		if(items_)
		{
			items_dialog dlg(arg.window_handle, type_);
			if(items_)
				dlg.set_items(*items_);
			dlg.modality();

			if(dlg.return_val() == nana::msgbox::pick_ok)
			{
				if(items_)
					*items_ = dlg.get_items();
				defvalue("");
				emit_event();
			}
		}
	});
}
/// class pg_collection end



/// class pg_layout_weight
#define STRING_CHOICE_SIZE		50

void pg_layout_weight::value(const std::string& value)
{
	lock_guard evt_lock(&evt_emit_, false);

	cmb_.option((value.find('%') == std::string::npos) ? 0 : 1);

	int int_val = -1;
	try
	{
		int_val = std::stoi(value);
	}
	catch(...)
	{
	}
	txt_.caption(std::to_string(int_val));

	pgitem::value(value);
}

void pg_layout_weight::create(nana::window wd)
{
	pg_string::create(wd);

	// textbox
	txt_.events().key_press.connect_front([this](const nana::arg_keyboard& arg)
	{
		if(arg.key == nana::keyboard::enter)
		{
			int int_val = -1;
			try
			{
				int_val = std::stoi(txt_.caption());
			}
			catch(...)
			{
				value(value_);
				arg.stop_propagation();
				return;
			}

			value(std::to_string(int_val) + (cmb_.option() == 1 ? "%" : ""));

			emit_event();
			arg.stop_propagation();
		}
	});

	txt_.set_accept([](wchar_t c) -> bool
	{
		return (isdigit(c) || c == '-' || c == nana::keyboard::cancel || c == nana::keyboard::backspace) ? true : false;
	});


	//combox
	cmb_.create(wd);

	cmb_.events().click.connect_front([this](const nana::arg_click& arg)
	{
		scroll();
	});
	cmb_.events().selected([this](const nana::arg_combox& arg)
	{
		if(!evt_emit_)
			return;

		value(txt_.caption() + (cmb_.option() == 1 ? "%" : ""));
		emit_event();
		arg.stop_propagation();
	});

	cmb_.push_back(CITEM_PIXELS);
	cmb_.push_back(CITEM_PERCENT);

	value(value_);
}

void pg_layout_weight::draw_value(nana::paint::graphics* graph, nana::rectangle rect, const int txtoff, nana::color bgcolor, nana::color fgcolor) const
{
	txt_.move(rect.x + PG_BORDER_X, PG_BORDER_Y);
	txt_.size(nana::size(rect.width - 2 * PG_BORDER_X - STRING_CHOICE_SIZE, size_ - 2 * PG_BORDER_Y));

	cmb_.move(rect.x + PG_BORDER_X + txt_.size().width, PG_BORDER_Y);
	cmb_.size(nana::size(STRING_CHOICE_SIZE, size_ - 2 * PG_BORDER_Y));
}
/// class pg_layout_weight end



/// class pg_margin
#define ARROW_BG_COLOR		nana::colors::dark_gray
#define ARROW_FG_COLOR		nana::colors::white

void pg_margin::value(const std::string& value)
{
	std::stringstream ss(value);
	std::string item;
	std::vector<int> items;

	try
	{
		while(getline(ss, item, ','))
			items.push_back(item.empty() ? 0 : std::max(0, std::stoi(item)));
	}
	catch(...)
	{
		// reset
		items.clear();
	}

	if(items.size() == 1)
	{
		expand_ = false;
		menu_.checked(2, true);
	}
	else
	{
		expand_ = true;
		menu_.checked(3, true);

		for(size_t i = items.size(); i < 4; ++i)
			items.push_back(0);
	}

	evt_emit_ = false;
	values_[0].caption(std::to_string(items[0])); //top or all
	if(expand_)
	{
		values_[1].caption(std::to_string(items[1])); //right
		values_[2].caption(std::to_string(items[2])); //bottom
		values_[3].caption(std::to_string(items[3])); //left
	}
	else
	{
		values_[1].caption("0"); //right
		values_[2].caption("0"); //bottom
		values_[3].caption("0"); //left
	}
	evt_emit_ = true;

	if(expand_)
		pgitem::value(std::to_string(items[0]) + "," + std::to_string(items[1]) + "," + std::to_string(items[2]) + "," + std::to_string(items[3]));
	else
		pgitem::value(std::to_string(items[0]));
}

void pg_margin::enabled(bool state)
{
	pgitem::enabled(state);

	for(auto& i : values_)
		i.enabled(en_);
}

unsigned pg_margin::size() const
{
	return expand_ ? 2 * size_ : size_;
}

void pg_margin::value(int value)
{
	pg_margin::value(std::to_string(value));
}

void pg_margin::value(int top, int right, int bottom, int left)
{
	pg_margin::value(std::to_string(top) + "," + std::to_string(right) + "," + std::to_string(bottom) + "," + std::to_string(left));
}

void pg_margin::create(nana::window wd)
{
	// ibox context menu
	menu_.append_splitter();
	// 2. 1 value
	menu_.append("1 value", [this](const nana::menu::item_proxy& ip)
		{
			pg_margin::value(values_[0].caption());
			scroll();
			update();
			emit_event();
		});
	menu_.check_style(2, nana::menu::checks::option);
	// 3. 4 values
	menu_.append("4 values", [this](const nana::menu::item_proxy& ip)
		{
			pg_margin::value(values_[0].caption() + "," + values_[0].caption() + "," + values_[0].caption() + "," + values_[0].caption());
			scroll();
			update();
			emit_event();
		});
	menu_.check_style(3, nana::menu::checks::option);
	menu_.checked(3, true);


	// textboxes
	for(auto& i : values_)
	{
		i.create(wd);
		i.multi_lines(false);
		i.focus_behavior(nana::textbox::text_focus_behavior::select_if_tabstop_or_click);

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
					if(expand_)
						pg_margin::value(values_[0].caption() + "," + values_[1].caption() + "," + values_[2].caption() + "," + values_[3].caption());
					else
						pg_margin::value(values_[0].caption());
					emit_event();
				}
			});
		i.events().focus([this, &i](const nana::arg_focus& arg)
			{
				if(!arg.getting)
				{
					// just lost focus, so capture the value left by the user
					if(expand_)
						pg_margin::value(values_[0].caption() + "," + values_[1].caption() + "," + values_[2].caption() + "," + values_[3].caption());
					else
						pg_margin::value(values_[0].caption());
					emit_event();
				}
			});
		i.set_accept([](wchar_t c) -> bool
			{
				return (isdigit(c) || c == nana::keyboard::cancel || c == nana::keyboard::backspace || c == nana::keyboard::tab) ? true : false;
			});
	}

	pg_margin::value(value_);
}

void pg_margin::draw_value(nana::paint::graphics* graph, nana::rectangle rect, const int txtoff, nana::color bgcolor, nana::color fgcolor) const
{
	if(expand_)
	{
		nana::API::show_window(values_[1], true);
		nana::API::show_window(values_[2], true);
		nana::API::show_window(values_[3], true);

		auto txtsize = graph->text_extent_size("R");
		int x = rect.x + PG_BORDER_X;
		int y = rect.y + static_cast<int>(size_ - 16) / 2;
		int ry = rect.y + PG_BORDER_Y;
		const int offset = 8;

		const unsigned txtw = 16 + 3;
		const unsigned ctrlw = (rect.width - (2 * txtw) - offset - 2 * PG_BORDER_X) / 2;

		nana::facade<nana::element::arrow> arrow("solid_triangle");

		// TOP
		graph->rectangle(nana::rectangle{ x + 1, ry, txtw, size_ - 2 * PG_BORDER_Y }, true, ARROW_BG_COLOR);
		arrow.direction(nana::direction::north);
		arrow.draw(*graph, {}, ARROW_FG_COLOR, { x + 3, y, 16, 16 }, nana::element_state::normal);
		x += txtw;
		values_[0].move(x, PG_BORDER_Y);
		values_[0].size(nana::size(ctrlw, size_ - 2 * PG_BORDER_Y));
		x += ctrlw + offset;
		// BOTTOM
		graph->rectangle(nana::rectangle{ x + 1, ry, txtw, size_ - 2 * PG_BORDER_Y }, true, ARROW_BG_COLOR);
		arrow.direction(nana::direction::south);
		arrow.draw(*graph, {}, ARROW_FG_COLOR, { x + 3, y, 16, 16 }, nana::element_state::normal);
		x += txtw;
		values_[2].move(x, PG_BORDER_Y);
		values_[2].size(nana::size(ctrlw, size_ - 2 * PG_BORDER_Y));

		// second line
		x = rect.x + PG_BORDER_X;
		y += size_;
		ry += size_;

		// LEFT
		graph->rectangle(nana::rectangle{ x + 1, ry, txtw, size_ - 2 * PG_BORDER_Y }, true, ARROW_BG_COLOR);
		arrow.direction(nana::direction::west);
		arrow.draw(*graph, {}, ARROW_FG_COLOR, { x + 4, y, 16, 16 }, nana::element_state::normal);
		x += txtw;
		values_[3].move(x, size_ + PG_BORDER_Y);
		values_[3].size(nana::size(ctrlw, size_ - 2 * PG_BORDER_Y));
		x += ctrlw + offset;
		// RIGHT
		graph->rectangle(nana::rectangle{ x + 1, ry, txtw, size_ - 2 * PG_BORDER_Y }, true, ARROW_BG_COLOR);
		arrow.direction(nana::direction::east);
		arrow.draw(*graph, {}, ARROW_FG_COLOR, { x + 4, y, 16, 16 }, nana::element_state::normal);
		x += txtw;
		values_[1].move(x, size_ + PG_BORDER_Y);
		values_[1].size(nana::size(ctrlw, size_ - 2 * PG_BORDER_Y));
	}
	else
	{
		nana::API::show_window(values_[1], false);
		nana::API::show_window(values_[2], false);
		nana::API::show_window(values_[3], false);

		values_[0].move(rect.x + PG_BORDER_X, PG_BORDER_Y);
		values_[0].size(nana::size(rect.width - 2 * PG_BORDER_X, size_ - 2 * PG_BORDER_Y));
	}
}
/// class pg_margin end



/// class pg_color_inherited
void pg_color_inherited::value(const std::string& value)
{
	evt_emit_ = false;

	color_ = nana::to_color(value);
	rgb_[0].caption(std::to_string(static_cast<int>(color_.r())));
	rgb_[1].caption(std::to_string(static_cast<int>(color_.g())));
	rgb_[2].caption(std::to_string(static_cast<int>(color_.b())));

	inherited_ = nana::is_color_inherited(value);
	menu_.checked(2, inherited_);

	for(auto& i : rgb_)
		i.enabled(en_ && !inherited_);

	evt_emit_ = true;

	pgitem::value(nana::to_string(color_, inherited_));
}

void pg_color_inherited::enabled(bool state)
{
	pgitem::enabled(state);

	for(auto& i : rgb_)
		i.enabled(en_ && !inherited_);
}

void pg_color_inherited::value(::nana::color value)
{
	pg_color_inherited::value(nana::to_string(value, inherited_));
}

void pg_color_inherited::inherited(bool value)
{
	pg_color_inherited::value(nana::to_string(color_, value));
}

void pg_color_inherited::create(nana::window wd)
{
	pg_color::create(wd);

	// ibox context menu
	menu_.append_splitter();
	// 2. Inherited
	menu_.append("Inherited", [this](const nana::menu::item_proxy& ip)
		{
			pg_color_inherited::value(nana::to_string(color_, ip.checked()));
			emit_event();
		});
	menu_.check_style(2, nana::menu::checks::highlight);

	pg_color_inherited::value(value_);
}

/// class pg_color_inherited end
