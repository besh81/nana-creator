/*
 *		nana::items_dialog Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "items_dialog.h"
#include <nana/gui/filebox.hpp>
#include "filemanager.h"
#include "tokenizer/Tokenizer.h"


#define NODE_NAME		"n"
#define NEW_ITEM_TXT	"New item"
#define SEPARATOR_TXT	"-----"


extern filemanager		g_file_mgr;


//items_dialog
void items_dialog::init()
{
	_retval = nana::msgbox::pick_cancel;

	disable_editing();

	// events
	toolbar.events().selected([this](const nana::arg_toolbar & arg)
	{
		if(arg.button == 0)
		{
			// add item

			// combox
			auto ip = items_tree.insert(_node_mgr.add_numbered(NODE_NAME), NEW_ITEM_TXT);
			if(_root.empty())
				_root = ip.owner();

			_data_struct ds;
			ds.key = ip.key();
			ds.text = NEW_ITEM_TXT;

			_data.push_back(ds);
			ip.select(true);
			return;
		}

		// other actions need selected item
		auto sel = items_tree.selected();
		if(sel.empty())
			return;

		if(arg.button == 1)
		{
			// delete item
			nana::msgbox m(*this, CREATOR_NAME, nana::msgbox::yes_no);
			m.icon(m.icon_question);
			m << "Delete selected item?";
			if(m() == nana::msgbox::pick_yes)
			{
				// combox
				if(sel == _root.child())
				{
					// this is the 1st sibling
					erase_item(sel.key());
					_root = items_tree.erase(sel);
					if(!_root.empty())
					{
						_root.select(true);
						_root = _root.owner();
					}
				}
				else
				{
					erase_item(sel.key());
					sel = items_tree.erase(sel);
					if(!sel.empty())
						sel.select(true);
					else
					{
						// this was the last sibling
						sel = _root.child();
						while(!sel.sibling().empty())
							sel = sel.sibling();
						sel.select(true);
					}
				}
			}
		}
		else if(arg.button == 3)
		{
			// move up

			// combox
			auto sn = sel._m_node();
			auto pn = sel.owner().child()._m_node(); // previous node

			if(pn == sn)
				return; // this is the 1st sibling: nothing to do

			if(pn->next == sn)
			{
				// this is the 2nd sibling
				pn->next = sn->next;
				sn->next = pn;
				pn->owner->child = sn;
			}
			else
			{
				while(pn->next->next != sn)
					pn = pn->next;

				pn->next->next = sn->next;
				sn->next = pn->next;
				pn->next = sn;
			}

			nana::API::refresh_window(items_tree);
		}
		else if(arg.button == 4)
		{
			// move down

			// combox
			auto sn = sel._m_node();
			if(!sn->next)
				return; // this is the last sibling: nothing to do

			auto pn = sel.owner().child()._m_node(); // previous node
			if(pn == sn)
			{
				// this is the 1st sibling
				pn->owner->child = sn->next;
				sn->next = pn->owner->child->next;
				pn->owner->child->next = sn;
			}
			else
			{
				while(pn->next != sn)
					pn = pn->next;

				pn->next = sn->next;
				sn->next = pn->next->next;
				pn->next->next = sn;
			}

			nana::API::refresh_window(items_tree);
		}
	});

	// select item
	items_tree.events().selected([this](const nana::arg_treebox& arg)
	{
		if(arg.operated)
		{
			type(_type);
			select_item(arg.item.key());
		}
		else
		{
			disable_editing();
		}
	});

	// ok button
	ok_btn.events().click([this]()
	{
		_retval = nana::msgbox::pick_ok;
		nana::API::close_window(handle());
	});

	// cancel button
	canc_btn.events().click([this]()
	{
		_retval = nana::msgbox::pick_cancel;
		nana::API::close_window(handle());
	});


	// type radioboxes
	normal_chk.events().click([this]()
	{
		if(!_selected)
			return;

		_selected->separator = false;
		_update_selected();

		// update treebox
		auto sel = items_tree.selected();
		if(sel.empty())
			return;
		sel.text(_selected->text);

		nana::API::refresh_window(items_tree);
	});

	separator_chk.events().click([this]()
	{
		if(!_selected)
			return;

		_selected->separator = true;
		_update_selected();

		// update treebox
		auto sel = items_tree.selected();
		if(sel.empty())
			return;
		sel.text(SEPARATOR_TXT);

		nana::API::refresh_window(items_tree);
	});


	// text
	item_txt.events().key_press([this](const nana::arg_keyboard& arg)
	{
		if(arg.key == nana::keyboard::enter)
		{
			if(!_selected)
				return;

			// text cannot be empty
			if(item_txt.caption().empty())
			{
				item_txt.caption(_selected->text);
				return;
			}
			_selected->text = item_txt.caption();

			// update treebox
			auto sel = items_tree.selected();
			if(sel.empty())
				return;
			sel.text(_selected->text);

			nana::API::refresh_window(items_tree);
		}
	});

	// image
	img_txt.events().key_press([this](const nana::arg_keyboard& arg)
	{
		if(arg.key == nana::keyboard::enter)
		{
			// user can only erase content -> to select image must use btn
			if(img_txt.caption().empty())
			{
				if(!_selected)
					return;

				_selected->img = "";

				// update treebox
				//TODO aggiunta immagine

				nana::API::refresh_window(items_tree);
			}
		}
	});

	img_btn.events().click.connect_front([this](const nana::arg_click& arg)
	{
		if(!_selected)
			return;

		nana::filebox fb(arg.window_handle, true);

		fb.add_filter("Image Files (" CREATOR_SUPPORTED_IMG ")", CREATOR_SUPPORTED_IMG);
		fb.add_filter("All Files", "*.*");

		fb.init_file(img_txt.caption());

		if(fb())
		{
			img_txt.caption(g_file_mgr.to_relative(fb.file()));
			_selected->img = fb.file();
		}
	});
}


void items_dialog::disable_editing()
{
	_selected = 0;

	normal_chk.enabled(false);
	separator_chk.enabled(false);
	//
	item_txt.caption("");
	item_txt.enabled(false);
	//
	img_txt.caption("");
	img_txt.enabled(false);
	img_btn.enabled(false);
}


void items_dialog::select_item(const std::string& key)
{
	_selected = 0;

	for(auto i = _data.begin(); i < _data.end(); ++i)
	{
		if(i->key == key)
		{
			_selected = i._Ptr;
			_update_selected();
			break;
		}
	}
}


void items_dialog::erase_item(const std::string& key)
{
	for(auto i = _data.begin(); i < _data.end(); ++i)
	{
		if(i->key == key)
		{
			_data.erase(i);
			break;
		}
	}
}


void items_dialog::_update_selected()
{
	if(_type == ctrls::pg_type::collection_combox)
	{
		normal_chk.enabled(true);
		normal_chk.check(true);
		//
		item_txt.enabled(true);
		item_txt.caption(_selected->text);
		//
		img_txt.enabled(true);
		img_txt.caption(g_file_mgr.to_relative(_selected->img));
		img_btn.enabled(true);
	}
	else if(_type == ctrls::pg_type::collection_toolbar)
	{
		normal_chk.enabled(true);
		separator_chk.enabled(true);

		if(_selected->separator)
		{
			separator_chk.check(true);
			item_txt.enabled(false);
			img_txt.enabled(false);
			img_btn.enabled(false);
		}
		else
		{
			normal_chk.check(true);
			item_txt.enabled(true);
			img_txt.enabled(true);
			img_btn.enabled(true);
		}

		item_txt.caption(_selected->text);
		img_txt.caption(g_file_mgr.to_relative(_selected->img));
	}
}


void items_dialog::value(const std::string& items)
{
	_data.clear();

	// split items into item (delimiter = CITEM_TKN)
	Tokenizer items_tkn(items);
	items_tkn.setDelimiter(CITEM_TKN);

	std::string item;
	while((item = items_tkn.next()) != "")
	{
		_data_struct d;
		d.key = _node_mgr.add_numbered(NODE_NAME);
		std::string items_tree_text;

		// split item into properties (delimiter = CITEM_INNER_TKN)
		Tokenizer item_tkn(item);
		item_tkn.setDelimiter(CITEM_INNER_TKN);

		if(_type == ctrls::pg_type::collection_combox)
		{
			d.text = item_tkn.next();
			d.img = item_tkn.next();
		}
		else if(_type == ctrls::pg_type::collection_toolbar)
		{
			if(item == CITEM_TLB_SEPARATOR)
			{
				d.separator = true;
				d.text = NEW_ITEM_TXT;
				items_tree_text = SEPARATOR_TXT;
			}
			else
			{
				d.text = items_tree_text = item_tkn.next();
				d.img = item_tkn.next();
			}
		}
		else
			return;
		

		auto ip = items_tree.insert(d.key, items_tree_text);
		if(_root.empty())
			_root = ip.owner();

		_data.push_back(d);
	}

	// select first item if any
	if(!_root.empty())
		_root.child().select(true);
}


std::string items_dialog::value()
{
	std::vector<std::string> vitems;

	if(_root.empty())
		return "";

	// create a vector of items
	auto p = _root.child();
	while(!p.empty())
	{
		if(_type == ctrls::pg_type::collection_combox)
		{
			p.visit_recursively([this, &vitems](nana::treebox::item_proxy item) -> bool
			{
				for(auto i : _data)
				{
					if(i.key == item.key())
					{
						vitems.push_back(i.text + CITEM_INNER_TKN + i.img);
					}
				}

				return true;
			});
		}
		else if(_type == ctrls::pg_type::collection_toolbar)
		{
			p.visit_recursively([this, &vitems](nana::treebox::item_proxy item) -> bool
			{
				for(auto i : _data)
				{
					if(i.key == item.key())
					{
						if(i.separator)
							vitems.push_back(CITEM_TLB_SEPARATOR);
						else
							vitems.push_back(i.text + CITEM_INNER_TKN + i.img);
					}
				}

				return true;
			});
		}
		else
			return "";

		p = p.sibling();
	}

	// create a string of items (delimiter = CITEM_TKN)
	std::string items;
	for(auto i = vitems.begin(); i < vitems.end(); ++i)
		if(i + 1 < vitems.end())
			items.append(*i + CITEM_TKN);
		else
			items.append(*i);

	return items;
}
