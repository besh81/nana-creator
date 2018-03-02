/*
 *		nana::items_dialog Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <nana/gui.hpp>
#include "items_dialog.h"
#include "config.h"
#include "tokenizer/Tokenizer.h"


void items_dialog::init()
{
	ret_val = nana::msgbox::pick_cancel;

	// populate toolbar
	nana::paint::image _img_edit;
	_img_edit.open("icons/item_edit_dark.png");
	_toolbar.append("Edit item", _img_edit);
	_toolbar.separate();
	nana::paint::image _img_new;
	_img_new.open("icons/item_add_dark.png");
	_toolbar.append("Add item", _img_new);
	nana::paint::image _img_del;
	_img_del.open("icons/item_delete_dark.png");
	_toolbar.append("Delete item", _img_del);
	_toolbar.separate();
	nana::paint::image _img_up;
	_img_up.open("icons/up_dark.png");
	_toolbar.append("Move up", _img_up);
	nana::paint::image _img_down;
	_img_down.open("icons/down_dark.png");
	_toolbar.append("Move down", _img_down);

	// events
	_toolbar.events().selected([this](const nana::arg_toolbar & arg)
	{
		auto cat = _list.at(0); //access the default category

		if(arg.button == 2)
		{
			// add item
			nana::inputbox ib(*this, "Insert new item", CREATOR_NAME);
			nana::inputbox::text item("Value:");
			if(ib.show_modal(item))
			{
				auto ip = cat.append(item.value());
				ip.select(true, true);
			}

			return;
		}


		// other actions need selected item
		auto idx = _list.selected();
		if(idx.size() != 1)
			return;

		auto sel = idx[0].item;

		if(arg.button == 0)
		{
			//edit item
			nana::inputbox ib(*this, "Edit item", CREATOR_NAME);
			nana::inputbox::text item("Value:", cat.at(sel).text(0));
			if(ib.show_modal(item))
			{
				auto ip = cat.at(sel).text(0, item.value());
				ip.select(true, true);
			}
		}
		else if(arg.button == 3)
		{
			// delete item
			nana::msgbox m(*this, CREATOR_NAME, nana::msgbox::yes_no);
			m.icon(m.icon_question);
			m << "Delete selected item?";
			if(m() == nana::msgbox::pick_yes)
			{
				_list.erase(cat.at(sel));
				if(cat.size())
					cat.at(0).select(true, true);
			}
		}
		else if(arg.button == 5)
		{
			// move up
			if(sel > 0)
			{
				auto v = cat.at(sel).text(0);
				cat.at(sel).text(0, cat.at(sel - 1).text(0));
				cat.at(sel - 1).text(0, v).select(true, true);
			}
		}
		else if(arg.button == 6)
		{
			// move down
			if(sel < cat.size() - 1)
			{
				auto v = cat.at(sel).text(0);
				cat.at(sel).text(0, cat.at(sel + 1).text(0));
				cat.at(sel + 1).text(0, v).select(true, true);
			}
		}
	});


	// ok button
	ok_btn.events().click([this]()
	{
		ret_val = nana::msgbox::pick_ok;
		nana::API::close_window(handle());
	});

	// cancel button
	canc_btn.events().click([this]()
	{
		ret_val = nana::msgbox::pick_cancel;
		nana::API::close_window(handle());
	});
}


//----------------------------------------------------------------------------------
// items: string with this format  ->  "item1" "item2" "item3" ...
//----------------------------------------------------------------------------------
void items_dialog::value(const std::string& items)
{
	auto cat = _list.at(0);

	Tokenizer strtkn(items);
	strtkn.setDelimiter("\"");
	std::string token;
	while((token = strtkn.next()) != "")
	{
		if(token != " ")
			cat.append(token);
	}
}

std::string items_dialog::value()
{
	auto cat = _list.at(0);
	std::string items;

	for(auto i : cat)
	{
		items.append("\"" + i.text(0) + "\" ");
	}
	return items;
}

