/*
 *		nana::pg_items Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "pg_items.h"
#include <nana/gui/filebox.hpp>
#include "items_dialog.h"
#include "filemanager.h"
#include "lock_guard.h"


extern filemanager		g_file_mgr;



/// class pg_filename
void pg_filename::value(const std::string& value)
{
	value_ = value;

	lock_guard evt_lock(&evt_emit_, false);
	txt_.caption(g_file_mgr.to_relative(value));
}

void pg_filename::add_filter(const std::string& description, const std::string& filetype)
{
	filters_.push_back({ description, filetype });
}

void pg_filename::create(nana::window wd)
{
	pg_string_button::create(wd);

	//textbox
	txt_.caption(g_file_mgr.to_relative(value_));

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
			value_ = fb.file();
			txt_.caption(g_file_mgr.to_relative(fb.file()));
			emit_event();
		}
	});
}
/// class pg_filename end



/// class pg_folder
void pg_folder::value(const std::string& value)
{
	value_ = value;

	lock_guard evt_lock(&evt_emit_, false);
	txt_.caption(equalize_path(value));
}

void pg_folder::create(nana::window wd)
{
	pg_string_button::create(wd);

	//textbox
	txt_.caption(equalize_path(value_));

	//button
	set_button_click([this](const nana::arg_click& arg)
	{
		nana::folderbox folder_picker(arg.window_handle, equalize_path(value_, '/', '\\'));

		auto path = folder_picker.show();
		if(path)
		{
			value_ = path.value().string();
			txt_.caption(equalize_path(value_));
			emit_event();
		}
	});
}
/// class pg_folder end



/// class pg_collection
void pg_collection::value(const std::string& value)
{
	value_ = value;

	lock_guard evt_lock(&evt_emit_, false);
	txt_.caption("(Collection)");
}

void pg_collection::create(nana::window wd)
{
	pg_string_button::create(wd);
	txt_.caption("(Collection)");

	//button
	set_button_click([this](const nana::arg_click& arg)
	{
		//items dialog
		items_dialog dlg(arg.window_handle, type_);
		dlg.value(value_);
		dlg.modality();

		if(dlg.return_val() == nana::msgbox::pick_ok)
		{
			value_ = dlg.value();
			emit_event();
		}
	});
}
/// class pg_collection end



/// class pg_layout_weight
#define STRING_CHOICE_SIZE		50

void pg_layout_weight::value(const std::string& value)
{
	cmb_.option((value.find('%') == std::string::npos) ? 0 : 1);

	value_ = value;
	pg_string_int::value(to_int());
}
std::string pg_layout_weight::value() const
{
	if(cmb_.option() == 1)
		return value_ + "%";
	return value_;
}

void pg_layout_weight::create(nana::window wd)
{
	pg_string_int::create(wd);

	//combox
	cmb_.create(wd);

	cmb_.events().click.connect_front([this](const nana::arg_click& arg)
	{
		scroll();
	});
	cmb_.events().selected([this](const nana::arg_combox& arg)
	{
		emit_event();
	});

	cmb_.push_back(CITEM_PIXELS);
	cmb_.push_back(CITEM_PERCENT);
	//cmb_.option(0);
	pg_layout_weight::value(value_);
}

bool pg_layout_weight::draw_value(nana::paint::graphics* graph, nana::rectangle rect, nana::color bgcolor, nana::color fgcolor) const
{
	txt_.move(PG_BORDER_X, PG_BORDER_Y);
	txt_.size(nana::size(rect.width - 2 * PG_BORDER_X - STRING_CHOICE_SIZE, size_ - 2 * PG_BORDER_Y));

	cmb_.move(PG_BORDER_X + txt_.size().width, PG_BORDER_Y);
	cmb_.size(nana::size(STRING_CHOICE_SIZE, size_ - 2 * PG_BORDER_Y));
	return false;
}
/// class pg_layout_weight end
