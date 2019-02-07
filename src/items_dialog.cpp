/*
 *		nana::items_dialog Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "items_dialog.h"
#include "nana_extra/color_helper.h"
#include "nana_extra/pgitems.h"
#include "propertygrid_helper.h"
#include "style.h"
#include "ctrls/combox.h"
#include "ctrls/listbox.h"
#include "ctrls/field.h"
#include "ctrls/menubar.h"
#include "ctrls/tabbar.h"
#include "ctrls/toolbar.h"


#define NODE_NAME		"n"
#define SEPARATOR_TXT	"-----"
#define COLLAPSE_TXT	"Collapse"



size_t hash_string(std::string const& str)
{
	static std::hash<std::string> h;
	return h(str);
}


//items_dialog
void items_dialog::init()
{
	//ctrls - START
	_place["right_lyt"] << _propgrid;
	_place.collocate();

	// toolbar: add separator button
	if(_type != ctrls::pg_type::collection_toolbar && _type != ctrls::pg_type::collection_menubar)
	{
		toolbar.enable(1, false);
	}
	// toolbar: left/right buttons
	if(_type != ctrls::pg_type::collection_menubar)
	{
		toolbar.enable(7, false);
		toolbar.enable(8, false);
	}
	//ctrls - END


	update_selected();

	//------------------
	// events
	//------------------
	_propgrid.events().property_changed([this](const nana::arg_propertygrid& arg)
	{
		if(_grid_setup)
			return;

		auto cat = _propgrid.at(arg.item.pos().cat);

		for(size_t i = 0; i < _selected->count(); ++i)
		{
			auto pi = (*_selected)[i];
			if(arg.item.label() == pi.label() && cat.text() == pi.category())
			{
				// check text
				if(arg.item.label() == "Text")
				{
					if(arg.item.value().empty())
					{
						// if empty restore old value
						arg.item.value(pi.value());
						return;
					}

					update_text(*items_tree.selected(), arg.item.value());
				}
				// check image
				else if(arg.item.label() == "Image")
				{
					update_image(*items_tree.selected(), arg.item.value());
				}


				pi.value(arg.item.value());

				// look for properties bonds
				propertygrid_helper::enabled_bonds(&_propgrid, _selected, pi.name(), pi.as_bool());
				break;
			}
		}
	});


	toolbar.events().selected([this](const nana::arg_toolbar & arg)
	{
		if(arg.button == 0)
		{
			// add item
			tb_add_item();
		}
		else if(arg.button == 1)
		{
			// add separator
			tb_add_item(true);
		}
		else if(arg.button == 2)
		{
			// delete item
			tb_delete_selected_item();
		}
		else if(arg.button == 4)
		{
			// move up
			tb_move_up_selected_item();
		}
		else if(arg.button == 5)
		{
			// move down
			tb_move_down_selected_item();
		}
		else if(arg.button == 7)
		{
			// move left
			tb_move_left_selected_item();
		}
		else if(arg.button == 8)
		{
			// move right
			tb_move_right_selected_item();
		}
	});

	// select item
	items_tree.events().selected([this](const nana::arg_treebox& arg)
	{
		if(arg.operated)
			select_item(arg.item.key());
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
}


void items_dialog::tb_add_item(bool separator)
{
	_items.push_back(ctrls::properties_collection{});
	auto& item = _items.back();

	// init item properties
	if(_type == ctrls::pg_type::collection_collapse)
		ctrls::field::init_item(item);
	else if(_type == ctrls::pg_type::collection_combox)
		ctrls::combox::init_item(item);
	else if(_type == ctrls::pg_type::collection_listbox)
		ctrls::listbox::init_item(item);
	else if(_type == ctrls::pg_type::collection_menubar)
		ctrls::menubar::init_item(item);
	else if(_type == ctrls::pg_type::collection_tabbar)
		ctrls::tabbar::init_item(item);
	else if(_type == ctrls::pg_type::collection_toolbar)
		ctrls::toolbar::init_item(item);

	if(separator)
	{
		item.property("separator") = true;
		item.property("text") = SEPARATOR_TXT;
	}

	// append to treebox
	auto text = item.property("text").as_string();
	if(_type == ctrls::pg_type::collection_collapse)
		text = COLLAPSE_TXT;
	auto ip = items_tree.insert(_node_mgr.add_numbered(NODE_NAME), text);
	if(_root.empty())
		_root = ip.owner();

	update_image(ip, "");

	item.property("key") = ip.key();

	// select
	ip.select(true);
	return;
}


//-------------------------------------------------------------------------------
//   ATTENTION !!!
//
// Removing an item its property_collection is removed from _items too.
// If the item has children the children's property_collection isn't removed.
// This is fixed by calling get_items() function who clean the _items list.
//-------------------------------------------------------------------------------
void items_dialog::tb_delete_selected_item()
{
	nana::msgbox m(*this, CREATOR_NAME, nana::msgbox::yes_no);
	m.icon(m.icon_question);
	m << "Delete selected item?";

	if(m() == nana::msgbox::pick_yes)
	{
		auto sel = items_tree.selected();
		if(sel.empty())
			return;


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

		if(_root.empty())
			select_item("");
	}
}


void items_dialog::tb_move_up_selected_item()
{
	auto sel = items_tree.selected();
	if(sel.empty())
		return;

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


void items_dialog::tb_move_down_selected_item()
{
	auto sel = items_tree.selected();
	if(sel.empty())
		return;

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


void items_dialog::tb_move_left_selected_item()
{
	auto sel = items_tree.selected();
	if(sel.empty())
		return;

	if(sel.level() == 1)
		return; // this is the first level: nothing to do

	if(_type == ctrls::pg_type::collection_menubar)
	{
		if(_selected->property("separator").as_bool() && sel.level() == 2)
		{
			nana::msgbox m(*this, CREATOR_NAME, nana::msgbox::ok);
			m.icon(m.icon_error);
			m << "Separator cannot be a menubar item !";
			m();
			return;
		}
	}

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

	if(_type == ctrls::pg_type::collection_menubar)
	{
		if(sel.level() == 1)
			update_image(sel, "");
	}
	nana::API::refresh_window(items_tree);
	update_selected();
}


void items_dialog::tb_move_right_selected_item()
{
	auto sel = items_tree.selected();
	if(sel.empty())
		return;

	auto prev = sel.owner().child(); // previous node
	if(prev == sel)
		return; // this is the 1st sibling: nothing to do

	while(prev.sibling() != sel)
		prev = prev.sibling();

	if(_type == ctrls::pg_type::collection_menubar)
	{
		auto _prev = find_item(prev->key());
		if(_prev->property("separator").as_bool())
		{
			nana::msgbox m(*this, CREATOR_NAME, nana::msgbox::ok);
			m.icon(m.icon_error);
			m << "Separator cannot have child item !";
			m();
			return;
		}
	}

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

	if(_type == ctrls::pg_type::collection_menubar)
	{
		if(sel.level() == 2)
			update_image(sel, _selected->property("image").as_string());
	}
	prev.expand(true);
	nana::API::refresh_window(items_tree);
	update_selected();
}


void items_dialog::select_item(const std::string& key)
{
	_selected = 0;
	if(!key.empty())
		_selected = find_item(key);
	update_selected();
}


void items_dialog::erase_item(const std::string& key)
{
	if(key.empty())
		return;

	for(auto i = _items.begin(); i < _items.end(); ++i)
		if(i->property("key").as_string() == key)
		{
			_items.erase(i);
			return;
		}
}


ctrls::properties_collection* items_dialog::find_item(const std::string& key)
{
	if(key.empty())
		return 0;

	for(auto& i : _items)
		if(i.property("key").as_string() == key)
			return &i;

	return 0;
}


void items_dialog::update_selected()
{
	_propgrid.enabled(_selected ? true : false);
	_propgrid.erase();

	if(_selected)
	{
		if(_selected->property("separator").as_bool() == false)
		{
			propertygrid_helper::append(&_propgrid, _selected, 0);

			if(_type == ctrls::pg_type::collection_menubar)
			{
				auto sel = items_tree.selected();
				if(!sel.empty())
					if(sel.level() == 1)
					{
						//TODO: find a better way to disable properties -> hide ???
						_propgrid.at(nana::propertygrid::index_pair(1, 1)).enabled(false);
						_propgrid.at(nana::propertygrid::index_pair(1, 1)).value("");
						_propgrid.at(nana::propertygrid::index_pair(1, 2)).enabled(false);
						_propgrid.at(nana::propertygrid::index_pair(1, 2)).value("true");
						_propgrid.at(nana::propertygrid::index_pair(1, 3)).enabled(false);
						_propgrid.at(nana::propertygrid::index_pair(1, 3)).value("None");
						_propgrid.at(nana::propertygrid::index_pair(1, 4)).enabled(false);
						_propgrid.at(nana::propertygrid::index_pair(1, 4)).value("false");
					}
			}
		}
	}	

	_propgrid.enabled(_selected ? true : false);
}


void items_dialog::update_text(nana::drawerbase::treebox::item_proxy& ip, const std::string& text)
{
	if(ip.empty())
		return;

	ip.text(text);
	nana::API::refresh_window(items_tree);
}


void items_dialog::update_image(nana::drawerbase::treebox::item_proxy& ip, const std::string& filename)
{
	if(ip.empty())
		return;

	auto img_id = hash_string(filename);

	auto & img = items_tree.icon(std::to_string(img_id));
	if(img.normal.empty())
		img.normal.open(filename);

	ip.icon(std::to_string(img_id));
	nana::API::refresh_window(items_tree);
}


void items_dialog::set_items(const std::vector<ctrls::properties_collection>& items)
{
	_items = items;

	items_tree.clear();

	// populate the items_tree
	for(auto i : _items)
	{
		_node_mgr.add(i.property("key").as_string());

		if(i.property("owner").as_string().empty())
		{
			auto text = i.property("text").as_string();
			if(_type == ctrls::pg_type::collection_collapse)
				text = COLLAPSE_TXT;
			auto ip = items_tree.insert(i.property("key").as_string(), text);
			if(_root.empty())
				_root = ip.owner();

			update_image(ip, i.property("image").as_string());
		}
		else
		{
			bool found = false;
			auto p = _root.child();
			while(!p.empty() && !found)
			{
				p.visit_recursively([this, &i, &found](nana::treebox::item_proxy item) -> bool
				{
					if(i.property("owner").as_string() == item.key())
					{
						auto ip = item.append(i.property("key").as_string(), i.property("text").as_string());
						item.expand(true);

						update_image(ip, i.property("image").as_string());

						found = true;
						return false;
					}

					return true;
				});

				p = p.sibling();
			}
		}
	}

	// select first item
	if(!_root.empty())
		if(!_root.child().empty())
			_root.child().select(true);
}


std::vector<ctrls::properties_collection>& items_dialog::get_items()
{
	std::vector<ctrls::properties_collection> cleaned_items;
	
	if(_root.empty())
	{
		_items.clear();
		return _items;
	}

	auto p = _root.child();
	while(!p.empty())
	{
		p.visit_recursively([this, &cleaned_items](nana::treebox::item_proxy item) -> bool
		{
			auto i = find_item(item.key());
			if(i)
			{
				i->property("owner").value(item.owner().key().empty() ? "" : item.owner().key());
				cleaned_items.push_back(*i);
			}

			return true;
		});

		p = p.sibling();
	}
	
	_items.clear();
	_items = cleaned_items;

	return _items;
}
