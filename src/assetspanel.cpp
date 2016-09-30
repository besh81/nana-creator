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
#include "style.h"


extern guimanager		g_gui_mgr;


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
	auto & img_h_layout = _assets.icon(CTRL_LAYOUT);
	img_h_layout.normal.open("icons/horizontal_layout_dark.png");

	auto & img_btn = _assets.icon(CTRL_BUTTON);
	img_btn.normal.open("icons/button_dark.png");

	auto & img_lbl = _assets.icon(CTRL_LABEL);
	img_lbl.normal.open("icons/label_dark.png");

	auto & img_txt = _assets.icon(CTRL_TEXTBOX);
	img_txt.normal.open("icons/textbox_dark.png");

	auto & img_pnl = _assets.icon(CTRL_PANEL);
	img_pnl.normal.open("icons/panel_dark.png");

	auto & img_com = _assets.icon(CTRL_COMBOX);
	img_com.normal.open("icons/combox_dark.png");

	auto & img_spn = _assets.icon(CTRL_SPINBOX);
	img_spn.normal.open("icons/spinbox_dark.png");

	// events
	_assets.events().selected([this](const nana::arg_treebox & arg)
	{
		if(arg.operated)
		{
			g_gui_mgr.cursor(arg.item.value<cursor_state>());
		}
	});


	// populate
	auto set = _assets.insert("forms", "Forms").value(cursor_state{ cursor_action::select });
	set.append("panel", CTRL_WIDGET_NAME, cursor_state{ cursor_action::add, CTRL_WIDGET }).icon(CTRL_PANEL);
	set.expand(true);
	//
	set = _assets.insert("layouts", "Layouts").value(cursor_state{ cursor_action::select });
	set.append("layouts/hv", CTRL_LAYOUT_NAME, cursor_state{ cursor_action::add, CTRL_LAYOUT }).icon(CTRL_LAYOUT);
	set.expand(true);
	//
	set = _assets.insert("controls", "Controls").value(cursor_state{ cursor_action::select });
	set.append("controls/button", CTRL_BUTTON_NAME, cursor_state{ cursor_action::add, CTRL_BUTTON }).icon(CTRL_BUTTON);
	set.append("controls/combox", CTRL_COMBOX_NAME, cursor_state{ cursor_action::add, CTRL_COMBOX }).icon(CTRL_COMBOX);
	set.append("controls/label", CTRL_LABEL_NAME, cursor_state{ cursor_action::add, CTRL_LABEL }).icon(CTRL_LABEL);
	set.append("controls/panel", CTRL_PANEL_NAME, cursor_state{ cursor_action::add, CTRL_PANEL }).icon(CTRL_PANEL);
	set.append("controls/spinbox", CTRL_SPINBOX_NAME, cursor_state{ cursor_action::add, CTRL_SPINBOX }).icon(CTRL_SPINBOX);
	set.append("controls/textbox", CTRL_TEXTBOX_NAME, cursor_state{ cursor_action::add, CTRL_TEXTBOX }).icon(CTRL_TEXTBOX);
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
