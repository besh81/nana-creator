/*
 *		objectspanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include "ctrls/panel.h"
#include "objectspanel.h"
#include "guimanager.h"
#include "style.h"


extern guimanager	g_gui_mgr;


//objectspanel
objectspanel::objectspanel(nana::window wd, bool visible)
	: nana::panel<true>(wd, visible)
{
	bgcolor(CREATOR_WINDOW_BG);

	_place.div("vertical <weight=20 caption><objects>");

	//TEMP caption
	_caption.bgcolor(CREATOR_WINDOW_CAP_BG);
	_caption.fgcolor(CREATOR_WINDOW_CAP_FG);
	_caption.caption("Objects");
	_place.field("caption") << _caption;


	// sets the images
	auto & img_h_layout = _objects.icon(CTRL_LAYOUT);
	img_h_layout.normal.open("icons/horizontal_layout_dark.png");

	auto & img_pnl = _objects.icon(CTRL_PANEL);
	img_pnl.normal.open("icons/panel_dark.png");

	auto & img_btn = _objects.icon(CTRL_BUTTON);
	img_btn.normal.open("icons/button_dark.png");

	auto & img_lbl = _objects.icon(CTRL_LABEL);
	img_lbl.normal.open("icons/label_dark.png");

	auto & img_txt = _objects.icon(CTRL_TEXTBOX);
	img_txt.normal.open("icons/textbox_dark.png");

	auto & img_com = _objects.icon(CTRL_COMBOX);
	img_com.normal.open("icons/combox_dark.png");

	auto & img_spn = _objects.icon(CTRL_SPINBOX);
	img_spn.normal.open("icons/spinbox_dark.png");


	// context menu
	_ctxmenu.append("Move Up", [this](const nana::menu::item_proxy& ip)
	{
		g_gui_mgr.moveupselected();
	});
	_ctxmenu.append("Move Down", [this](const nana::menu::item_proxy& ip)
	{
		g_gui_mgr.movedownselected();
	});
	_ctxmenu.append_splitter();
	_ctxmenu.append("Delete", [this](const nana::menu::item_proxy& ip)
	{
		g_gui_mgr.deleteselected();
	});


	// events
	_objects.events().selected([this](const nana::arg_treebox& arg)
	{
		if(arg.operated)
		{
			g_gui_mgr.clickobjectspanel(arg.item.key());
		}
	});
	//
	_objects.events().mouse_down(nana::menu_popuper(_ctxmenu));
	//
	/*_objects.events().mouse_down([this](const nana::arg_mouse& arg)
	{
		if(arg.right_button)
			std::cout << "right click: " << _objects.selected().text() << "\n";
	});
	*/


	_objects.bgcolor(CREATOR_WINDOW_BG);
	_place.field("objects") << _objects;
	_place.collocate();
}


bool objectspanel::append(const std::string& parent, const std::string& name, const std::string& type)
{
	std::string objname = name + " : " + type;

	if(!parent.empty())
	{
		nana::treebox::item_proxy pnode;
		std::string objparent = parent + " :";

		auto item = _roots[0];
		if(item.empty())
			return false;

		item.visit_recursively([this, &pnode, objparent](nana::treebox::item_proxy item) -> bool
		{
			if(item.text().find(objparent) == 0)
			{
				pnode = item;
				return false;
			}

			return true;
		});

		if(pnode.empty())
			return false;

		_objects.insert(pnode, name, objname).icon(type).select(true);
		pnode.expand(true);
	}
	else
	{
		_roots.push_back(_objects.insert(name, objname).icon(type).select(true));
	}
	return true;
}


bool objectspanel::remove(const std::string& name)
{
	bool ret = false;
	std::string objname = name + " : ";

	auto item = _roots[0];
	if(!item.empty())
	{
		item.visit_recursively([this, &ret, objname](nana::treebox::item_proxy item) -> bool
		{
			if(item.text().find(objname) == 0)
			{
				_objects.erase(item);

				if(item == _roots[0])
					_roots.clear();

				ret = true;
				return false;
			}

			return true;
		});

		if(ret)
			nana::API::refresh_window(_objects);
	}

	return ret;
}


bool objectspanel::move_before_sibling(const std::string& name)
{
	bool ret = false;
	std::string objname = name + " : ";

	auto item = _roots[0];
	if(!item.empty())
	{
		item.visit_recursively([this, &ret, objname](nana::treebox::item_proxy item) -> bool
		{
			if(item.text().find(objname) == 0)
			{
				auto item_node = item._m_node();

				if(!item_node->owner)
					return false;

				auto t = item_node->owner->child;
				if(t == item_node)
					// this is the 1st sibling: nothing to do
					return false;

				// hierarchy modification
				ret = true;

				if(t->next == item_node)
				{
					// this is the 2nd sibling
					t->next = item_node->next;
					item_node->next = t;
					item_node->owner->child = item_node;
					return false;
				}

				while(t->next->next != item_node)
					t = t->next;

				t->next->next = item_node->next;
				item_node->next = t->next;
				t->next = item_node;
				return false;
			}

			return true;
		});

		if(ret)
			nana::API::refresh_window(_objects);
	}

	return ret;
}


bool objectspanel::move_after_sibling(const std::string& name)
{
	bool ret = false;
	std::string objname = name + " : ";

	auto item = _roots[0];
	if(!item.empty())
	{
		item.visit_recursively([this, &ret, objname](nana::treebox::item_proxy item) -> bool
		{
			if(item.text().find(objname) == 0)
			{
				auto item_node = item._m_node();

				if(!item_node->next)
					// this is the last sibling: nothing to do
					return false;

				if(!item_node->owner)
					return false;

				// hierarchy modification
				ret = true;

				auto t = item_node->owner->child;
				if(t == item_node)
				{
					// this is the 1st sibling
					item_node->owner->child = item_node->next;
					item_node->next = item_node->owner->child->next;
					item_node->owner->child->next = item_node;
					return false;
				}

				while(t->next != item_node)
					t = t->next;

				t->next = item_node->next;
				item_node->next = t->next->next;
				t->next->next = item_node;
				return false;
			}

			return true;
		});

		if(ret)
			nana::API::refresh_window(_objects);
	}

	return ret;
}


bool objectspanel::clear()
{
	if(!_roots.empty())
	{
		_objects.erase(_roots[0]);
		_roots.clear();
	}
	return true;
}


bool objectspanel::select(const std::string& name)
{
	bool ret = false;
	std::string objname = name + " : ";

	auto item = _roots[0];
	if(!item.empty())
	{
		item.visit_recursively([this, &ret, objname](nana::treebox::item_proxy item) -> bool
		{
			if(item.text().find(objname) == 0)
			{
				item->select(true);
				ret = true;
				return false;
			}

			return true;
		});
	}

	return ret;
}


bool objectspanel::change_name(const std::string& old_name, const std::string& new_name, const std::string& new_type)
{
	bool ret = false;
	std::string objname = old_name + " : ";

	auto item = _roots[0];
	if(!item.empty())
	{
		item.visit_recursively([this, &ret, objname, new_name, new_type](nana::treebox::item_proxy item) -> bool
		{
			if(item.text().find(objname) == 0)
			{
				item->text(new_name + " : " + new_type);
				item->key(new_name);
				ret = true;
				return false;
			}

			return true;
		});
	}

	if(ret)
		nana::API::refresh_window(_objects);

	return ret;
}
