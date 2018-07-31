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
	//ctrls - START
	if(_type == ctrls::pg_type::collection_combox)
	{
		// toolbar
		toolbar.enable(6, false);
		toolbar.enable(7, false);

		prop_grp.div("vert margin=5 arrange=[25,repeated] field1");

		// label1
		label1.create(prop_grp);
		label1.caption("Text:");
		label1.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// item_txt
		item_txt.create(prop_grp);
		item_txt.line_wrapped(false);
		item_txt.multi_lines(false);
		// label2
		label2.create(prop_grp);
		label2.caption("Image:");
		label2.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// img_lyt
		img_lyt_panel.create(prop_grp);
		img_lyt_place.bind(img_lyt_panel);
		img_lyt_place.div("<field1><weight=30 field2>");
		// img_txt
		img_txt.create(img_lyt_panel);
		img_lyt_place["field1"] << img_txt;
		img_txt.line_wrapped(false);
		img_txt.multi_lines(false);
		// img_btn
		img_btn.create(img_lyt_panel);
		img_lyt_place["field2"] << img_btn;
		img_btn.caption("...");

		prop_grp["field1"] << label1 << item_txt << label2 << img_lyt_panel;
		prop_grp.collocate();
	}
	else if(_type == ctrls::pg_type::collection_toolbar)
	{
		// toolbar
		toolbar.enable(6, false);
		toolbar.enable(7, false);

		prop_grp.div("vert margin=5 <weight=25 field1><vert arrange=[25,repeated] field2>");

		// normal_chk
		normal_chk.create(prop_grp);
		normal_chk.caption("Normal");
		rg_type.add(normal_chk);
		// separator_chk
		separator_chk.create(prop_grp);
		separator_chk.caption("Separator");
		rg_type.add(separator_chk);

		// label1
		label1.create(prop_grp);
		label1.caption("Text:");
		label1.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// item_txt
		item_txt.create(prop_grp);
		item_txt.line_wrapped(false);
		item_txt.multi_lines(false);
		// label2
		label2.create(prop_grp);
		label2.caption("Image:");
		label2.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// img_lyt
		img_lyt_panel.create(prop_grp);
		img_lyt_place.bind(img_lyt_panel);
		img_lyt_place.div("<field1><weight=30 field2>");
		// img_txt
		img_txt.create(img_lyt_panel);
		img_lyt_place["field1"] << img_txt;
		img_txt.line_wrapped(false);
		img_txt.multi_lines(false);
		// img_btn
		img_btn.create(img_lyt_panel);
		img_lyt_place["field2"] << img_btn;
		img_btn.caption("...");

		prop_grp["field1"] << normal_chk << separator_chk;
		prop_grp["field2"] << label1 << item_txt << label2 << img_lyt_panel;
		prop_grp.collocate();
	}
	else if(_type == ctrls::pg_type::collection_listbox)
	{
		// toolbar
		toolbar.enable(6, false);
		toolbar.enable(7, false);

		prop_grp.div("vert margin=5 arrange=[25,repeated] field1");

		// label1
		label1.create(prop_grp);
		label1.caption("Text:");
		label1.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// item_txt
		item_txt.create(prop_grp);
		item_txt.line_wrapped(false);
		item_txt.multi_lines(false);
		// label3
		label3.create(prop_grp);
		label3.caption("Width:");
		label3.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// width_txt
		width_txt.create(prop_grp);
		width_txt.line_wrapped(false);
		width_txt.multi_lines(false);

		prop_grp["field1"] << label1 << item_txt << label3 << width_txt;
		prop_grp.collocate();
	}
	else if(_type == ctrls::pg_type::collection_tabbar)
	{
		// toolbar
		toolbar.enable(6, false);
		toolbar.enable(7, false);

		prop_grp.div("vert margin=5 arrange=[25,repeated] field1");

		// label1
		label1.create(prop_grp);
		label1.caption("Text:");
		label1.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// item_txt
		item_txt.create(prop_grp);
		item_txt.line_wrapped(false);
		item_txt.multi_lines(false);

		prop_grp["field1"] << label1 << item_txt;
		prop_grp.collocate();
	}
	else if(_type == ctrls::pg_type::collection_menubar)
	{
		prop_grp.div("vert margin=5 arrange=[25,repeated] field1");

		// label1
		label1.create(prop_grp);
		label1.caption("Text:");
		label1.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// item_txt
		item_txt.create(prop_grp);
		item_txt.line_wrapped(false);
		item_txt.multi_lines(false);

		prop_grp["field1"] << label1 << item_txt;
		prop_grp.collocate();
	}
	else if(_type == ctrls::pg_type::collection_categorize)
	{
		prop_grp.div("vert margin=5 arrange=[25,repeated] field1");

		// label1
		label1.create(prop_grp);
		label1.caption("Text:");
		label1.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// item_txt
		item_txt.create(prop_grp);
		item_txt.line_wrapped(false);
		item_txt.multi_lines(false);

		prop_grp["field1"] << label1 << item_txt;
		prop_grp.collocate();
	}
	//ctrls - END


	_retval = nana::msgbox::pick_cancel;

	_update_selected();

	//------------------
	// events
	//------------------
	toolbar.events().selected([this](const nana::arg_toolbar & arg)
	{
		if(arg.button == 0)
		{
			// add item

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
		else if(arg.button == 6)
		{
			// move left

			if(sel.level() == 1)
				return; // this is the first level: nothing to do

			auto sn = sel._m_node();

			// remove selected node from current level
			auto prev = sel.owner().child(); // previous node

			if(prev == sel)
			{
				// this is the 1st sibling
				sn->owner->child = sn->next;
			}
			else
			{
				while(prev.sibling() != sel)
					prev = prev.sibling();

				auto pn = prev._m_node();
				pn->next = sn->next;
			}


			// insert selected node into previous level right after the owner node
			auto on = sel.owner()._m_node(); // owner node
			
			sn->owner = on->owner;
			sn->next = on->next;
			on->next = sn;

			nana::API::refresh_window(items_tree);
		}
		else if(arg.button == 7)
		{
			// move right
			
			auto prev = sel.owner().child(); // previous node
			if(prev == sel)
				return; // this is the 1st sibling: nothing to do

			while(prev.sibling() != sel)
				prev = prev.sibling();
			
			auto sn = sel._m_node();
			auto pn = prev._m_node();

			sn->owner = pn;
			pn->next = sn->next;
			// append as last children
			sn->next = 0;
			if(pn->child)
			{
				pn = pn->child;
				while(pn->next)
					pn = pn->next;
				pn->next = sn;
			}
			else
				pn->child = sn;

			prev.expand(true);
			nana::API::refresh_window(items_tree);
		}
	});

	// select item
	items_tree.events().selected([this](const nana::arg_treebox& arg)
	{
		if(arg.operated)
		{
			select_item(arg.item.key());
		}
		else
		{
			_selected = 0;
			_update_selected();
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



	if(_type == ctrls::pg_type::collection_toolbar)
	{
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
	}


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


	if(_type == ctrls::pg_type::collection_combox || _type == ctrls::pg_type::collection_toolbar)
	{
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

			fb.init_file(_selected->img);

			if(fb())
			{
				img_txt.caption(g_file_mgr.to_relative(fb.file()));
				_selected->img = fb.file();
			}
		});
	}


	if(_type == ctrls::pg_type::collection_listbox)
	{
		width_txt.events().key_press.connect_front([this](const nana::arg_keyboard& arg)
		{
			if(arg.key == nana::keyboard::enter)
			{
				unsigned u_val = -1;
				try
				{
					u_val = std::stoul(width_txt.caption());
				}
				catch(...)
				{
					width_txt.caption(_selected->width);
					arg.stop_propagation();
					return;
				}

				_selected->width = std::to_string(u_val);
				width_txt.caption(_selected->width);
				arg.stop_propagation();
			}
		});

		width_txt.set_accept([](wchar_t c) -> bool
		{
			return (isdigit(c) || c == nana::keyboard::cancel || c == nana::keyboard::backspace) ? true : false;
		});
	}


	if(_type == ctrls::pg_type::collection_tabbar)
	{
	}
	

	if(_type == ctrls::pg_type::collection_menubar)
	{
	}
	

	if(_type == ctrls::pg_type::collection_categorize)
	{

	}
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
		if(_selected)
		{
			item_txt.enabled(true);
			item_txt.caption(_selected->text);
			//
			img_txt.enabled(true);
			img_txt.caption(g_file_mgr.to_relative(_selected->img));
			img_btn.enabled(true);
			// set focus
			item_txt.focus();
		}
		else
		{
			item_txt.enabled(false);
			//
			img_txt.enabled(false);
			img_btn.enabled(false);
		}
	}
	else if(_type == ctrls::pg_type::collection_toolbar)
	{
		if(_selected)
		{
			normal_chk.enabled(true);
			separator_chk.enabled(true);

			if(_selected->separator)
			{
				separator_chk.check(true);
				//
				item_txt.enabled(false);
				//
				img_txt.enabled(false);
				img_btn.enabled(false);
			}
			else
			{
				normal_chk.check(true);
				//
				item_txt.enabled(true);
				//
				img_txt.enabled(true);
				img_btn.enabled(true);
			}

			item_txt.caption(_selected->text);
			img_txt.caption(g_file_mgr.to_relative(_selected->img));
			// set focus
			item_txt.focus();
		}
		else
		{
			normal_chk.enabled(false);
			separator_chk.enabled(false);
			//
			item_txt.enabled(false);
			//
			img_txt.enabled(false);
			img_btn.enabled(false);

		}
	}
	else if(_type == ctrls::pg_type::collection_listbox)
	{
		if(_selected)
		{
			item_txt.enabled(true);
			item_txt.caption(_selected->text);
			//
			width_txt.enabled(true);
			width_txt.caption(_selected->width);
			// set focus
			item_txt.focus();
		}
		else
		{
			item_txt.enabled(false);
			//
			width_txt.enabled(false);
		}
	}
	else if(_type == ctrls::pg_type::collection_tabbar)
	{
		if(_selected)
		{
			item_txt.enabled(true);
			item_txt.caption(_selected->text);
			// set focus
			item_txt.focus();
		}
		else
		{
			item_txt.enabled(false);
		}
	}
	else if(_type == ctrls::pg_type::collection_menubar)
	{
		if(_selected)
		{
			item_txt.enabled(true);
			item_txt.caption(_selected->text);
			// set focus
			item_txt.focus();
		}
		else
		{
			item_txt.enabled(false);
		}
	}
	else if(_type == ctrls::pg_type::collection_categorize)
	{
		if(_selected)
		{
			item_txt.enabled(true);
			item_txt.caption(_selected->text);
			// set focus
			item_txt.focus();
		}
		else
		{
			item_txt.enabled(false);
		}
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
		d.owner = CITEM_EMPTY_OWNER;
		std::string items_tree_text;

		// split item into properties (delimiter = CITEM_INNER_TKN)
		Tokenizer item_tkn(item);
		item_tkn.setDelimiter(CITEM_INNER_TKN);

		if(_type == ctrls::pg_type::collection_combox)
		{
			d.text = items_tree_text = item_tkn.next();
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
		else if(_type == ctrls::pg_type::collection_listbox)
		{
			d.text = items_tree_text = item_tkn.next();
			d.width = item_tkn.next();
		}
		else if(_type == ctrls::pg_type::collection_tabbar)
		{
			d.text = items_tree_text = item_tkn.next();
		}
		else if(_type == ctrls::pg_type::collection_menubar)
		{
			d.key = item_tkn.next();
			d.owner = item_tkn.next();
			d.text = items_tree_text = item_tkn.next();
		}
		else if(_type == ctrls::pg_type::collection_categorize)
		{
		}
		else
			return;
		

		if(d.owner == CITEM_EMPTY_OWNER)
		{
			auto ip = items_tree.insert(d.key, items_tree_text);
			if(_root.empty())
				_root = ip.owner();
		}
		else
		{
			bool found = false;
			auto p = _root.child();
			while(!p.empty() && !found)
			{
				p.visit_recursively([this, &d, &items_tree_text, &found](nana::treebox::item_proxy item) -> bool
				{
					if(d.owner == item.key())
					{
						item.append(d.key, items_tree_text);
						item.expand(true);
						
						found = true;
						return false;
					}

					return true;
				});

				p = p.sibling();
			}
		}
	
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
		p.visit_recursively([this, &vitems](nana::treebox::item_proxy item) -> bool
		{
			for(auto i : _data)
			{
				if(i.key == item.key())
				{
					if(_type == ctrls::pg_type::collection_combox)
					{
						vitems.push_back(i.text + CITEM_INNER_TKN + i.img);
					}
					else if(_type == ctrls::pg_type::collection_toolbar)
					{
						if(i.separator)
							vitems.push_back(CITEM_TLB_SEPARATOR);
						else
							vitems.push_back(i.text + CITEM_INNER_TKN + i.img);
					}
					else if(_type == ctrls::pg_type::collection_listbox)
					{
						vitems.push_back(i.text + CITEM_INNER_TKN + i.width);
					}
					else if(_type == ctrls::pg_type::collection_tabbar)
					{
						vitems.push_back(i.text);
					}
					else if(_type == ctrls::pg_type::collection_menubar)
					{
						vitems.push_back(item.key() + CITEM_INNER_TKN + (item.owner().key().empty() ? CITEM_EMPTY_OWNER : item.owner().key()) + CITEM_INNER_TKN + i.text);
					}
					else if(_type == ctrls::pg_type::collection_categorize)
					{
					}

					break;
				}
			}

			return true;
		});

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
