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


extern filemanager		g_file_mgr;
extern inifile			g_inifile;



/// class pg_filename
void pg_filename::value(const std::string& value)
{
	lock_guard evt_lock(&evt_emit_, false);
	txt_.caption(g_file_mgr.to_relative(value));

	pgitem::value(value);
}

void pg_filename::add_filter(const std::string& description, const std::string& filetype)
{
	filters_.push_back({ description, filetype });
}

void pg_filename::create(nana::window wd)
{
	pg_string_button::create(wd);

	//textbox
	value(value_);

	//button
	set_button_click([this](const nana::arg_click& arg)
	{
		nana::filebox fb(arg.window_handle, true);
		
		for(auto i : filters_)
			fb.add_filter(i.first, i.second);
		fb.add_filter("All Files", "*.*");

		fb.init_file(value_);

		if(fb())
		{
			value(fb.file());
			emit_event();
		}
	});
}
/// class pg_filename end



/// class pg_image
void pg_image::value(const std::string& value)
{
	auto ep = equalize_path(value);

	lock_guard evt_lock(&evt_emit_, false);
	txt_.caption(g_file_mgr.to_relative(ep));

	pgitem::value(ep);
}

void pg_image::create(nana::window wd)
{
	pg_string_button::create(wd);

	//textbox
	value(value_);

	//button
	set_button_click([this](const nana::arg_click& arg)
	{
		nana::filebox fb(arg.window_handle, true);

		fb.add_filter("Image Files (" CREATOR_SUPPORTED_IMG ")", CREATOR_SUPPORTED_IMG);

		if(value_.empty())
			fb.init_path(equalize_path(g_inifile.image_dir()));
		else
			fb.init_file(value_);

		if(fb())
		{
			value(fb.file());

			// save image folder
			auto path = get_dir_path(equalize_path(fb.file()));
			if(path != g_inifile.image_dir())
				g_inifile.image_dir(path, true);

			emit_event();
		}
	});
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

		auto path = folder_picker.show();
		if(path)
		{
			value(path.value().string());
			emit_event();
		}
	});
}
/// class pg_folder end



/// class pg_collection
void pg_collection::create(nana::window wd)
{
	pg_string_button::create(wd);
	{
		lock_guard evt_lock(&evt_emit_, false);
		txt_.caption("(Collection)");
	}
	editable(false);

	//button
	set_button_click([this](const nana::arg_click& arg)
	{
		//items dialog
		if(items_)
		{
			items_dialog dlg(arg.window_handle, type_);
			dlg.set_items(*items_);
			dlg.modality();

			if(dlg.return_val() == nana::msgbox::pick_ok)
			{
				*items_ = dlg.get_items();
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
