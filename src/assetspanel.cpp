/*
 *		assetspanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "assetspanel.h"
#include "guimanager.h"
#include "imagemanager.h"
#include "style.h"


extern guimanager		g_gui_mgr;
extern imagemanager		g_img_mgr;


//assetspanel
assetspanel::assetspanel(nana::window wd, bool visible)
	: nana::panel<true>(wd, visible)
{
	bgcolor(CREATOR_WINDOW_BG);

	_place.div("vertical <weight=20 caption><assets>");

	//TEMP caption
	_caption.bgcolor(CREATOR_WINDOW_CAP_BG);
	_caption.fgcolor(CREATOR_WINDOW_CAP_FG);
	_caption.caption("Assets");
	_place.field("caption") << _caption;


	// sets the images
	unsigned i = 0;
	while(!g_img_mgr.name(i).empty())
	{
		auto & img = _assets.icon(g_img_mgr.name(i));
		img.normal.open(g_img_mgr.path(i));
		++i;
	}


	// events
	_assets.events().selected([this](const nana::arg_treebox & arg)
	{
		if(arg.operated)
		{
			g_gui_mgr.cursor(arg.item.value<cursor_state>());
		}
	});


	// populate
	auto set = _assets.insert("widgets", "Widgets").value(cursor_state{ cursor_action::select });
	set.append("widgets/form", CTRL_FORM, cursor_state{ cursor_action::add, CTRL_MAINFORM }).icon(CTRL_FORM);
	set.append("widgets/panel", CTRL_PANEL, cursor_state{ cursor_action::add, CTRL_MAINPANEL }).icon(CTRL_PANEL);
	set.expand(true);
	//
	set = _assets.insert("layouts", "Layouts").value(cursor_state{ cursor_action::select });
	set.append("layouts/layout", CTRL_LAYOUT, cursor_state{ cursor_action::add, CTRL_LAYOUT }).icon(CTRL_LAYOUT);
	set.expand(true);
	//
	set = _assets.insert("controls", "Controls").value(cursor_state{ cursor_action::select });
	set.append("controls/button", CTRL_BUTTON, cursor_state{ cursor_action::add, CTRL_BUTTON }).icon(CTRL_BUTTON);
	set.append("controls/categorize", CTRL_CATEGORIZE, cursor_state{ cursor_action::add, CTRL_CATEGORIZE }).icon(CTRL_CATEGORIZE);
	set.append("controls/checkbox", CTRL_CHECKBOX, cursor_state{ cursor_action::add, CTRL_CHECKBOX }).icon(CTRL_CHECKBOX);
	set.append("controls/combox", CTRL_COMBOX, cursor_state{ cursor_action::add, CTRL_COMBOX }).icon(CTRL_COMBOX);
	set.append("controls/datechooser", CTRL_DATECHOOSER, cursor_state{ cursor_action::add, CTRL_DATECHOOSER }).icon(CTRL_DATECHOOSER);
	set.append("controls/group", CTRL_GROUP, cursor_state{ cursor_action::add, CTRL_GROUP }).icon(CTRL_GROUP);
	set.append("controls/label", CTRL_LABEL, cursor_state{ cursor_action::add, CTRL_LABEL }).icon(CTRL_LABEL);
	set.append("controls/listbox", CTRL_LISTBOX, cursor_state{ cursor_action::add, CTRL_LISTBOX }).icon(CTRL_LISTBOX);
	set.append("controls/menubar", CTRL_MENUBAR, cursor_state{ cursor_action::add, CTRL_MENUBAR }).icon(CTRL_MENUBAR);
	set.append("controls/panel", CTRL_PANEL, cursor_state{ cursor_action::add, CTRL_PANEL }).icon(CTRL_PANEL);
	set.append("controls/picture", CTRL_PICTURE, cursor_state{ cursor_action::add, CTRL_PICTURE }).icon(CTRL_PICTURE);
	set.append("controls/progress", CTRL_PROGRESS, cursor_state{ cursor_action::add, CTRL_PROGRESS }).icon(CTRL_PROGRESS);
	set.append("controls/slider", CTRL_SLIDER, cursor_state{ cursor_action::add, CTRL_SLIDER }).icon(CTRL_SLIDER);
	set.append("controls/spinbox", CTRL_SPINBOX, cursor_state{ cursor_action::add, CTRL_SPINBOX }).icon(CTRL_SPINBOX);
	set.append("controls/tabbar", CTRL_TABBAR, cursor_state{ cursor_action::add, CTRL_TABBAR }).icon(CTRL_TABBAR);
	set.append("controls/textbox", CTRL_TEXTBOX, cursor_state{ cursor_action::add, CTRL_TEXTBOX }).icon(CTRL_TEXTBOX);
	set.append("controls/toolbar", CTRL_TOOLBAR, cursor_state{ cursor_action::add, CTRL_TOOLBAR }).icon(CTRL_TOOLBAR);
	set.append("controls/treebox", CTRL_TREEBOX, cursor_state{ cursor_action::add, CTRL_TREEBOX }).icon(CTRL_TREEBOX);
	set.expand(true);


	_assets.bgcolor(CREATOR_WINDOW_BG);
	_place.field("assets") << _assets;
	_place.collocate();
}


void assetspanel::deselect()
{
	auto item = _assets.selected();
	if(!item.empty())
		item.select(false);
}
