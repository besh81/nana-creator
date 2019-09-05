/*
 *		assetspanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "assetspanel.h"
#include "imagemanager.h"
#include "style.h"


extern imagemanager		g_img_mgr;


//assetspanel
assetspanel::assetspanel(nana::window wd, bool visible)
	: nana::panel<true>(wd, visible)
{
	bgcolor(CREATOR_WINDOW_BG);

	_place.div("assets");

	// sets the images
	unsigned i = 0;
	while(!g_img_mgr.name(i).empty())
	{
		auto & img = _assets.icon(g_img_mgr.name(i));
		img.normal.open(g_img_mgr.path(i));
		++i;
	}
	

	// populate
	auto set = _assets.insert("layouts", "Layouts").value(std::string(""));
	set.append("layouts/field", CTRL_FIELD, std::string(CTRL_FIELD)).icon(CTRL_FIELD);
	set.append("layouts/grid", CTRL_GRID, std::string(CTRL_GRID)).icon(CTRL_GRID);
	set.append("layouts/splitterbar", CTRL_SPLITTERBAR, std::string(CTRL_SPLITTERBAR)).icon(CTRL_SPLITTERBAR);
	set.expand(true);
	//
	set = _assets.insert("containers", "Containers").value(std::string(""));
	set.append("containers/group", CTRL_GROUP, std::string(CTRL_GROUP)).icon(CTRL_GROUP);
	set.append("containers/notebook", CTRL_NOTEBOOK, std::string(CTRL_NOTEBOOK)).icon(CTRL_NOTEBOOK);
	set.append("containers/page", CTRL_PAGE " (" CTRL_NOTEBOOK ")", std::string(CTRL_PAGE)).icon(CTRL_PAGE);
	set.append("containers/panel", CTRL_PANEL, std::string(CTRL_PANEL)).icon(CTRL_PANEL);
	set.expand(true);
	//
	set = _assets.insert("controls", "Controls").value(std::string(""));
	set.append("controls/button", CTRL_BUTTON, std::string(CTRL_BUTTON)).icon(CTRL_BUTTON);
	set.append("controls/categorize", CTRL_CATEGORIZE, std::string(CTRL_CATEGORIZE)).icon(CTRL_CATEGORIZE);
	set.append("controls/checkbox", CTRL_CHECKBOX, std::string(CTRL_CHECKBOX)).icon(CTRL_CHECKBOX);
	set.append("controls/combox", CTRL_COMBOX, std::string(CTRL_COMBOX)).icon(CTRL_COMBOX);
	set.append("controls/custom", CTRL_CUSTOM, std::string(CTRL_CUSTOM)).icon(CTRL_CUSTOM);
	set.append("controls/datechooser", CTRL_DATECHOOSER, std::string(CTRL_DATECHOOSER)).icon(CTRL_DATECHOOSER);
	set.append("controls/label", CTRL_LABEL, std::string(CTRL_LABEL)).icon(CTRL_LABEL);
	set.append("controls/listbox", CTRL_LISTBOX, std::string(CTRL_LISTBOX)).icon(CTRL_LISTBOX);
	set.append("controls/menubar", CTRL_MENUBAR, std::string(CTRL_MENUBAR)).icon(CTRL_MENUBAR);
	set.append("controls/picture", CTRL_PICTURE, std::string(CTRL_PICTURE)).icon(CTRL_PICTURE);
	set.append("controls/progress", CTRL_PROGRESS, std::string(CTRL_PROGRESS)).icon(CTRL_PROGRESS);
	set.append("controls/slider", CTRL_SLIDER, std::string(CTRL_SLIDER)).icon(CTRL_SLIDER);
	set.append("controls/spinbox", CTRL_SPINBOX, std::string(CTRL_SPINBOX)).icon(CTRL_SPINBOX);
	set.append("controls/tabbar", CTRL_TABBAR, std::string(CTRL_TABBAR)).icon(CTRL_TABBAR);
	set.append("controls/textbox", CTRL_TEXTBOX, std::string(CTRL_TEXTBOX)).icon(CTRL_TEXTBOX);
	set.append("controls/toolbar", CTRL_TOOLBAR, std::string(CTRL_TOOLBAR)).icon(CTRL_TOOLBAR);
	set.append("controls/treebox", CTRL_TREEBOX, std::string(CTRL_TREEBOX)).icon(CTRL_TREEBOX);
	set.expand(true);

	_assets.bgcolor(CREATOR_WINDOW_BG);
	_place.field("assets") << _assets;
	_place.collocate();
}


void assetspanel::selected(std::function<void(const std::string&)> f)
{
	_assets.events().selected([this, f](const nana::arg_treebox & arg)
	{
		if(arg.operated)
			f(arg.item.value<std::string>());
	});
}


void assetspanel::deselect()
{
	auto item = _assets.selected();
	if(!item.empty())
		item.select(false);
}
