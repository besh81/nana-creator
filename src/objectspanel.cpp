/*
 *		objectspanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include "objectspanel.h"
#include "guimanager.h"
#include "imagemanager.h"
#include "style.h"


extern guimanager	g_gui_mgr;
extern imagemanager	g_img_mgr;


//objectspanel
objectspanel::objectspanel(nana::window wd, bool visible)
	: nana::panel<true>(wd, visible)
{
	bgcolor(CREATOR_WINDOW_BG);

	_place.div("vertical <weight=20 caption><objects>");

	// caption
	_caption.bgcolor(CREATOR_WINDOW_CAP_BG);
	_caption.fgcolor(CREATOR_WINDOW_CAP_FG);
	_caption.caption("Objects");
	_place.field("caption") << _caption;


	// context menu
	// 0. move up
	_ctxmenu.append("Move Up", [this](const nana::menu::item_proxy& ip)
	{
		g_gui_mgr.moveupselected();
	});
	nana::paint::image _img_up("icons/up.png");
	_ctxmenu.image(0, _img_up);
	// 1. move down
	_ctxmenu.append("Move Down", [this](const nana::menu::item_proxy& ip)
	{
		g_gui_mgr.movedownselected();
	});
	nana::paint::image _img_down("icons/down.png");
	_ctxmenu.image(1, _img_down);
	// 2. -----
	_ctxmenu.append_splitter();
	// 3. delete
	_ctxmenu.append("Delete", [this](const nana::menu::item_proxy& ip)
	{
		g_gui_mgr.deleteselected();
	});
	nana::paint::image _img_del("icons/delete.png");
	_ctxmenu.image(3, _img_del);
	// 4. -----
	_ctxmenu.append_splitter();
	// 5. cut
	_ctxmenu.append("Cut", [this](const nana::menu::item_proxy& ip)
	{
		g_gui_mgr.copyselected(true);
	});
	nana::paint::image _img_cut("icons/cut.png");
	_ctxmenu.image(5, _img_cut);
	// 6. copy
	_ctxmenu.append("Copy", [this](const nana::menu::item_proxy& ip)
	{
		g_gui_mgr.copyselected();
	});
	nana::paint::image _img_copy("icons/copy.png");
	_ctxmenu.image(6, _img_copy);
	// 7. paste
	_ctxmenu.append("Paste", [this](const nana::menu::item_proxy& ip)
	{
		g_gui_mgr.pasteselected();
	});
	nana::paint::image _img_paste("icons/paste.png");
	_ctxmenu.image(7, _img_paste);


	// events
	_objects.events().selected([this](const nana::arg_treebox& arg)
	{
		if(!_evt_emit)
			return;

		if(arg.operated)
		{
			g_gui_mgr.clickobjectspanel(arg.item.key());
		}
	});
	//
	_objects.events().mouse_down(nana::menu_popuper(_ctxmenu));


	_objects.bgcolor(CREATOR_WINDOW_BG);
	_place.field("objects") << _objects;
	_place.collocate();
}


bool objectspanel::append(const std::string& parent, const std::string& name, const std::string& type)
{
	// load icon
	auto & icon_ = _objects.icon(type);
	if(icon_.normal.empty())
		icon_.normal.open(g_img_mgr.path(type));

	std::string objname = name + " : " + type;

	if(parent.empty())
	{
		_roots.push_back(_objects.insert(name, objname).icon(type).select(true));
		return true;
	}

	nana::treebox::item_proxy pnode;
	std::string objparent = parent + " :";

	if(_roots.empty())
		return false; // error

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

	return true;
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
	if(_roots.empty())
		return false; // error

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
