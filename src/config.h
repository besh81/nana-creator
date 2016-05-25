/*
 *		Creator config
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_CONFIG_H
#define NANA_CREATOR_CONFIG_H


#define CREATOR_NAME			"Nana Creator"
#define CREATOR_VERSION			"0.2.0"


// available controls
//--------------------------------------------------------
// templates/forms
#define CTRL_WIDGET				"widget"
#define CTRL_WIDGET_NAME		"Widget"

// controls with child/children
#define CTRL_LAYOUT				"layout"
#define CTRL_LAYOUT_NAME		"Horizontal/Vertical"
//
#define CTRL_PANEL				"panel"
#define CTRL_PANEL_NAME			"Panel"

// controls without child/children
#define CTRL_BUTTON				"button"
#define CTRL_BUTTON_NAME		"Button"
#define CTRL_LABEL				"label"
#define CTRL_LABEL_NAME			"Label"
#define CTRL_TEXTBOX			"textbox"
#define CTRL_TEXTBOX_NAME		"Textbox"



// project file
//--------------------------------------------------------
// nodes name
#define PROJECT_EXT				"ncp"

#define NODE_ROOT				"nana-creator"
#define NODE_LAYOUT				CTRL_LAYOUT
#define NODE_BUTTON				CTRL_BUTTON
#define NODE_LABEL				CTRL_LABEL
#define NODE_TEXTBOX			CTRL_TEXTBOX
#define NODE_PANEL				CTRL_PANEL



// categories
//--------------------------------------------------------
#define CAT_COMMON				"Common"
#define CAT_APPEARANCE			"Appearance"
#define CAT_LAYOUT				"Layout"


// choices
#define CITEM_HLAYOUT			"Horizontal"
#define CITEM_VLAYOUT			"Vertical"
//
#define CITEM_LEFT				"Left"
#define CITEM_CENTER			"Center"
#define CITEM_RIGHT				"Right"
#define CITEM_TOP				"Top"
#define CITEM_BOTTOM			"Bottom"



// code-generator
//--------------------------------------------------------
#define CTRL_NAMESPACE			"nana::"


#endif //NANA_CREATOR_CONFIG_H
