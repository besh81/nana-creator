/*
 *		nana::items_dialog Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "items_dialog.h"
#include <nana/gui/filebox.hpp>
#include "nana_extra/color_helper.h"
#include "nana_extra/propgrid_items.h"
#include "pg_items.h"
#include "filemanager.h"
#include "tokenizer/Tokenizer.h"
#include "style.h"


#define NODE_NAME		"n"
#define NEW_ITEM_TXT	"New item"
#define COLLAPSE_TXT	"Collapse"
#define SEPARATOR_TXT	"-----"


extern filemanager		g_file_mgr;



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


	// toolbar: left/right buttons
	if(_type != ctrls::pg_type::collection_menubar)
	{
		toolbar.enable(6, false);
		toolbar.enable(7, false);
	}

	// append properties
	auto cat = _propgrid.append("Properties");

	if(_type == ctrls::pg_type::collection_combox)
	{
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string("Text", "")));
		cat.append(nana::propertygrid::pgitem_ptr(new pg_image("Image", "")));
	}
	else if(_type == ctrls::pg_type::collection_toolbar)
	{
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_check("Separator", false)));
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string("Text", "")));
		cat.append(nana::propertygrid::pgitem_ptr(new pg_image("Image", "")));
	}
	else if(_type == ctrls::pg_type::collection_listbox)
	{
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string("Text", "")));
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string_uint("Width", "")));
	}
	else if(_type == ctrls::pg_type::collection_tabbar)
	{
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string("Text", "")));
		cat.append(nana::propertygrid::pgitem_ptr(new pg_image("Image", "")));
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_color("Background", "", true)));
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_color("Foreground", "", true)));
	}
	else if(_type == ctrls::pg_type::collection_menubar)
	{
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_check("Separator", false)));
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string("Text", "")));
		cat.append(nana::propertygrid::pgitem_ptr(new pg_image("Image", "")));
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_check("Enabled", true)));
		auto item = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_choice("Check style")));
		auto pgc = static_cast<nana::pg_choice*>(item._m_pgitem());
		pgc->push_back(CITEM_NONE);
		pgc->push_back(CITEM_OPTION);
		pgc->push_back(CITEM_HIGHLIGHT);
		pgc->option(0); // CITEM_NONE
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_check("Checked", false)));
	}
	else if(_type == ctrls::pg_type::collection_categorize)
	{
	}
	else if(_type == ctrls::pg_type::collection_collapse)
	{
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string_uint("Left", "")));
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string_uint("Top", "")));
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string_uint("Columns", "")));
		cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string_uint("Rows", "")));
	}


	_propgrid.events().property_changed([this](const nana::arg_propertygrid& arg)
	{
		if(arg.item.label() == "Text")
		{
			// collection_combox
			// collection_toolbar
			// collection_listbox
			// collection_tabbar
			// collection_menubar

			// text cannot be empty
			if(arg.item.value().empty())
			{
				if(_type == ctrls::pg_type::collection_toolbar || _type == ctrls::pg_type::collection_menubar)
					_propgrid.at(nana::propertygrid::index_pair(1, 1)).value(_selected->text);
				else
					_propgrid.at(nana::propertygrid::index_pair(1, 0)).value(_selected->text);

				return;
			}
			_selected->text = arg.item.value();

			update_text(*items_tree.selected(), _selected->text);
		}
		else if(arg.item.label() == "Image")
		{
			// collection_combox
			// collection_toolbar
			// collection_tabbar
			// collection_menubar

			_selected->img = arg.item.value();

			update_image(*items_tree.selected(), _selected->img);
		}
		else if(arg.item.label() == "Separator")
		{
			// collection_toolbar
			// collection_menubar

			_selected->separator = arg.item.value() == "true" ? true : false;
			update_selected();

			auto ip = items_tree.selected();
			update_text(ip, arg.item.value() == "true" ? SEPARATOR_TXT : _selected->text);
			update_image(ip, arg.item.value() == "true" ? "" : _selected->img);
		}
		else if(arg.item.label() == "Width")
		{
			// collection_listbox

			_selected->width = arg.item.value();
		}
		else if(arg.item.label() == "Background")
		{
			// collection_tabbar

			_selected->bgcolor = arg.item.value();
		}
		else if(arg.item.label() == "Foreground")
		{
			// collection_tabbar

			_selected->fgcolor = arg.item.value();
		}
		// collection_menubar
		else if(arg.item.label() == "Enabled")
			_selected->enabled = arg.item.value();
		else if(arg.item.label() == "Check style")
			_selected->check_style = arg.item.value();
		else if(arg.item.label() == "Checked")
			_selected->checked = arg.item.value();
		// collection_collapse
		else if(arg.item.label() == "Left")
			_selected->left = arg.item.value();
		else if(arg.item.label() == "Top")
			_selected->top = arg.item.value();
		else if(arg.item.label() == "Columns")
			_selected->cols = arg.item.value();
		else if(arg.item.label() == "Rows")
			_selected->rows = arg.item.value();

	});
	//ctrls - END


	_retval = nana::msgbox::pick_cancel;

	update_selected();

	//------------------
	// events
	//------------------
	toolbar.events().selected([this](const nana::arg_toolbar & arg)
	{
		if(arg.button == 0)
		{
			// add item

			std::string item_txt = _type == ctrls::pg_type::collection_collapse ? COLLAPSE_TXT : NEW_ITEM_TXT;

			auto ip = items_tree.insert(_node_mgr.add_numbered(NODE_NAME), item_txt);
			if(_root.empty())
				_root = ip.owner();

			_data_struct ds;
			ds.key = ip.key();
			ds.text = item_txt;
			ds.bgcolor = nana::to_string(ITEMS_DEF_BGCOL, true);
			ds.fgcolor = nana::to_string(ITEMS_DEF_FGCOL, true);

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

				if(_root.empty())
					select_item("");
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

			if(_type == ctrls::pg_type::collection_menubar)
			{
				if(_selected->separator && sel.level() == 2)
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
		else if(arg.button == 7)
		{
			// move right
			
			auto prev = sel.owner().child(); // previous node
			if(prev == sel)
				return; // this is the 1st sibling: nothing to do

			while(prev.sibling() != sel)
				prev = prev.sibling();
			
			if(_type == ctrls::pg_type::collection_menubar)
			{
				auto _prev = find_item(prev->key());
				if(_prev->separator)
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
					update_image(sel, _selected->img);
			}
			prev.expand(true);
			nana::API::refresh_window(items_tree);
			update_selected();
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

	for(auto i = _data.begin(); i < _data.end(); ++i)
	{
		if(i->key == key)
		{
			_data.erase(i);
			break;
		}
	}
}


items_dialog::_data_struct* items_dialog::find_item(const std::string& key)
{
	if(key.empty())
		return 0;

	for(auto i = _data.begin(); i < _data.end(); ++i)
	{
		if(i->key == key)
			return &(*i);
	}

	return 0;
}


void items_dialog::update_selected()
{
	_propgrid.enabled(_selected ? true : false);

	if(_type == ctrls::pg_type::collection_combox)
	{
		if(_selected)
		{
			_propgrid.at(nana::propertygrid::index_pair(1, 0)).value(_selected->text);
			_propgrid.at(nana::propertygrid::index_pair(1, 1)).value(_selected->img);

			// set focus
			//TODO
		}
		else
		{
			_propgrid.at(nana::propertygrid::index_pair(1, 0)).value("");
			_propgrid.at(nana::propertygrid::index_pair(1, 1)).value("");
		}
	}
	else if(_type == ctrls::pg_type::collection_toolbar)
	{
		if(_selected)
		{
			_propgrid.at(nana::propertygrid::index_pair(1, 0)).value(_selected->separator ? "true" : "false");

			if(_selected->separator)
			{
				_propgrid.at(nana::propertygrid::index_pair(1, 1)).enabled(false);
				_propgrid.at(nana::propertygrid::index_pair(1, 2)).enabled(false);

				_propgrid.at(nana::propertygrid::index_pair(1, 1)).value("");
				_propgrid.at(nana::propertygrid::index_pair(1, 2)).value("");
			}
			else
			{
				_propgrid.at(nana::propertygrid::index_pair(1, 1)).enabled(true);
				_propgrid.at(nana::propertygrid::index_pair(1, 2)).enabled(true);

				_propgrid.at(nana::propertygrid::index_pair(1, 1)).value(_selected->text);
				_propgrid.at(nana::propertygrid::index_pair(1, 2)).value(_selected->img);
			}
		}
		else
		{
			_propgrid.at(nana::propertygrid::index_pair(1, 0)).value("false");
			_propgrid.at(nana::propertygrid::index_pair(1, 1)).value("");
			_propgrid.at(nana::propertygrid::index_pair(1, 2)).value("");
		}
	}
	else if(_type == ctrls::pg_type::collection_listbox)
	{
		if(_selected)
		{
			_propgrid.at(nana::propertygrid::index_pair(1, 0)).value(_selected->text);
			_propgrid.at(nana::propertygrid::index_pair(1, 1)).value(_selected->width);
		}
		else
		{
			_propgrid.at(nana::propertygrid::index_pair(1, 0)).value("");
			_propgrid.at(nana::propertygrid::index_pair(1, 1)).value("");
		}
	}
	else if(_type == ctrls::pg_type::collection_tabbar)
	{
		if(_selected)
		{
			_propgrid.at(nana::propertygrid::index_pair(1, 0)).value(_selected->text);
			_propgrid.at(nana::propertygrid::index_pair(1, 1)).value(_selected->img);
			_propgrid.at(nana::propertygrid::index_pair(1, 2)).value(_selected->bgcolor.empty() ? nana::to_string(ITEMS_DEF_BGCOL, true) : _selected->bgcolor);
			_propgrid.at(nana::propertygrid::index_pair(1, 3)).value(_selected->fgcolor.empty() ? nana::to_string(ITEMS_DEF_FGCOL, true) : _selected->fgcolor);
		}
		else
		{
			_propgrid.at(nana::propertygrid::index_pair(1, 0)).value("");
			_propgrid.at(nana::propertygrid::index_pair(1, 1)).value("");
			_propgrid.at(nana::propertygrid::index_pair(1, 2)).value("");
			_propgrid.at(nana::propertygrid::index_pair(1, 3)).value("");
		}
	}
	else if(_type == ctrls::pg_type::collection_menubar)
	{
		if(_selected)
		{
			auto ip = items_tree.selected();
			// separator
			if(ip.level() == 1 || ip.size())
			{
				_propgrid.at(nana::propertygrid::index_pair(1, 0)).enabled(false);
				_propgrid.at(nana::propertygrid::index_pair(1, 0)).value("false");
			}
			else
			{
				_propgrid.at(nana::propertygrid::index_pair(1, 0)).enabled(true);
				_propgrid.at(nana::propertygrid::index_pair(1, 0)).value(_selected->separator ? "true" : "false");
			}

			if(_selected->separator)
			{
				// text
				_propgrid.at(nana::propertygrid::index_pair(1, 1)).enabled(false);
				_propgrid.at(nana::propertygrid::index_pair(1, 1)).value("");
				// image
				_propgrid.at(nana::propertygrid::index_pair(1, 2)).enabled(false);
				_propgrid.at(nana::propertygrid::index_pair(1, 2)).value("");
				// enabled
				_propgrid.at(nana::propertygrid::index_pair(1, 3)).enabled(false);
				_propgrid.at(nana::propertygrid::index_pair(1, 3)).value("true");
				// check_style
				_propgrid.at(nana::propertygrid::index_pair(1, 4)).enabled(false);
				_propgrid.at(nana::propertygrid::index_pair(1, 4)).value("");
				// checked
				_propgrid.at(nana::propertygrid::index_pair(1, 5)).enabled(false);
				_propgrid.at(nana::propertygrid::index_pair(1, 5)).value("false");
			}
			else
			{
				// text
				_propgrid.at(nana::propertygrid::index_pair(1, 1)).enabled(true);
				_propgrid.at(nana::propertygrid::index_pair(1, 1)).value(_selected->text);
				// image
				_propgrid.at(nana::propertygrid::index_pair(1, 2)).enabled(ip.level() == 1 ? false : true);
				_propgrid.at(nana::propertygrid::index_pair(1, 2)).value(ip.level() == 1 ? "" : _selected->img);
				// enabled
				_propgrid.at(nana::propertygrid::index_pair(1, 3)).enabled(ip.level() == 1 ? false : true);
				_propgrid.at(nana::propertygrid::index_pair(1, 3)).value(ip.level() == 1 ? "true" : _selected->enabled);
				// check_style
				_propgrid.at(nana::propertygrid::index_pair(1, 4)).enabled(ip.level() == 1 ? false : true);
				auto pgc = static_cast<nana::pg_choice*>(_propgrid.at(nana::propertygrid::index_pair(1, 4))._m_pgitem());
				pgc->option(std::atoi(_selected->check_style.c_str()));
				// checked
				_propgrid.at(nana::propertygrid::index_pair(1, 5)).enabled(ip.level() == 1 ? false : true);
				_propgrid.at(nana::propertygrid::index_pair(1, 5)).value(ip.level() == 1 ? "false" : _selected->checked);
			}
		}
		else
		{
			_propgrid.at(nana::propertygrid::index_pair(1, 0)).value("false"); // separator
			_propgrid.at(nana::propertygrid::index_pair(1, 1)).value(""); // text
			_propgrid.at(nana::propertygrid::index_pair(1, 2)).value(""); // image
			_propgrid.at(nana::propertygrid::index_pair(1, 3)).value("true"); // enabled
			_propgrid.at(nana::propertygrid::index_pair(1, 4)).value(""); // check_style
			_propgrid.at(nana::propertygrid::index_pair(1, 5)).value("false"); // checked
		}
	}
	else if(_type == ctrls::pg_type::collection_categorize)
	{
	}
	else if(_type == ctrls::pg_type::collection_collapse)
	{
		if(_selected)
		{
			_propgrid.at(nana::propertygrid::index_pair(1, 0)).value(_selected->left);
			_propgrid.at(nana::propertygrid::index_pair(1, 1)).value(_selected->top);
			_propgrid.at(nana::propertygrid::index_pair(1, 2)).value(_selected->cols);
			_propgrid.at(nana::propertygrid::index_pair(1, 3)).value(_selected->rows);
		}
		else
		{
			_propgrid.at(nana::propertygrid::index_pair(1, 0)).value("");
			_propgrid.at(nana::propertygrid::index_pair(1, 1)).value("");
			_propgrid.at(nana::propertygrid::index_pair(1, 2)).value("");
			_propgrid.at(nana::propertygrid::index_pair(1, 3)).value("");
		}
	}
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
		d.owner = CITEM_EMPTY;
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
			d.img = item_tkn.next();
			d.bgcolor = item_tkn.next();
			d.fgcolor = item_tkn.next();
		}
		else if(_type == ctrls::pg_type::collection_menubar)
		{
			d.key = item_tkn.next();
			d.owner = item_tkn.next();

			auto it = item_tkn.next();
			if(it == CITEM_SEPARATOR)
			{
				d.separator = true;
				d.text = NEW_ITEM_TXT;
				items_tree_text = SEPARATOR_TXT;
			}
			else
			{
				d.text = items_tree_text = it;
				d.img = item_tkn.next();
				it = item_tkn.next();
				if(!it.empty())
					d.enabled = it;
				it = item_tkn.next();
				if(!it.empty())
					d.check_style = it;
				it = item_tkn.next();
				if(!it.empty())
					d.checked = it;
			}
		}
		else if(_type == ctrls::pg_type::collection_categorize)
		{
		}
		else if(_type == ctrls::pg_type::collection_collapse)
		{
			items_tree_text = COLLAPSE_TXT;
			d.left = item_tkn.next();
			if(d.left == CITEM_EMPTY) d.left = "0";
			d.top = item_tkn.next();
			if(d.top == CITEM_EMPTY) d.top = "0";
			d.cols = item_tkn.next();
			if(d.cols == CITEM_EMPTY) d.cols = "0";
			d.rows = item_tkn.next();
			if(d.rows == CITEM_EMPTY) d.rows = "0";
		}
		else
			return;


		if(d.owner == CITEM_EMPTY)
		{
			auto ip = items_tree.insert(d.key, items_tree_text);
			if(_root.empty())
				_root = ip.owner();

			// load treebox image
			if(!d.img.empty())
			{
				update_image(ip, d.img);
			}
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
						auto ip = item.append(d.key, items_tree_text);
						item.expand(true);

						// load treebox image
						if(!d.img.empty())
						{
							update_image(ip, d.img);
						}
						
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
							vitems.push_back(i.text + CITEM_INNER_TKN + (i.img.empty() ? CITEM_EMPTY : i.img));
					}
					else if(_type == ctrls::pg_type::collection_listbox)
					{
						vitems.push_back(i.text + CITEM_INNER_TKN + i.width);
					}
					else if(_type == ctrls::pg_type::collection_tabbar)
					{
						vitems.push_back(i.text + CITEM_INNER_TKN + (i.img.empty() ? CITEM_EMPTY : i.img) + CITEM_INNER_TKN + i.bgcolor + CITEM_INNER_TKN + i.fgcolor);
					}
					else if(_type == ctrls::pg_type::collection_menubar)
					{
						std::string str = item.key() + CITEM_INNER_TKN + (item.owner().key().empty() ? CITEM_EMPTY : item.owner().key()) + CITEM_INNER_TKN;
						if(i.separator)
							str.append(CITEM_SEPARATOR);
						else
						{
							str.append(i.text + CITEM_INNER_TKN);
							if(item.level() == 1)
								str.append(CITEM_EMPTY CITEM_INNER_TKN "true" CITEM_INNER_TKN "0" CITEM_INNER_TKN "false");
							else
								str.append((i.img.empty() ? CITEM_EMPTY : i.img) + CITEM_INNER_TKN + i.enabled + CITEM_INNER_TKN + i.check_style + CITEM_INNER_TKN + i.checked);
						}
						vitems.push_back(str);
					}
					else if(_type == ctrls::pg_type::collection_categorize)
					{
					}
					else if(_type == ctrls::pg_type::collection_collapse)
					{
						vitems.push_back(i.left + CITEM_INNER_TKN + i.top + CITEM_INNER_TKN + i.cols + CITEM_INNER_TKN + i.rows);
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
