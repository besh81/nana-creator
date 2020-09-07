/*
 *		objectspanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include "objectspanel.h"
#include "imagemanager.h"
#include "style.h"


extern imagemanager	g_img_mgr;


//objectspanel
objectspanel::objectspanel(nana::window wd, bool visible)
	: nana::panel<true>(wd, nana::rectangle(), visible)
{
	bgcolor(CREATOR_WINDOW_BG);

	_place.div("objects");

	_objects.bgcolor(CREATOR_WINDOW_BG);
	_objects.use_entire_line(true);
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
				_objects.scroll_into_view(item);
				ret = true;
				return false;
			}

			return true;
		});
	}

	return ret;
}


void objectspanel::selected(std::function<void(const std::string&)> f)
{
	_objects.events().selected([this, f](const nana::arg_treebox& arg) {
		if(!_evt_emit)
			return;

		if(arg.operated)
			f(arg.item.key());
	});
}


void objectspanel::contex_menu(nana::menu* ctx)
{
	_ctxmenu = ctx;

	_objects.events().mouse_down([this](const nana::arg_mouse& arg) {
		_right_button = arg.right_button;
	});

	_objects.events().mouse_up([this](const nana::arg_mouse& arg) {
		if(!_evt_emit)
			return;

		if(_right_button && !_objects.hovered(false).empty())
		{
			_ctxmenu->popup(*this, arg.pos.x, arg.pos.y);
		}
	});
}
