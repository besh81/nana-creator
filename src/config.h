/*
 *		Creator config
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_CONFIG_H
#define NANA_CREATOR_CONFIG_H


#define CREATOR_NAME			"Nana Creator"
#define CREATOR_VERSION			"0.4.0"



// available controls
//--------------------------------------------------------
// templates/forms
#define CTRL_MAINFORM			"mainform"
#define CTRL_MAINPANEL			"mainpanel"

// controls with child/children
#define CTRL_PANEL				"panel"
#define CTRL_PANEL_NAME			"Panel"
#define CTRL_FORM				"form"
#define CTRL_FORM_NAME			"Form"
//
#define CTRL_LAYOUT				"layout"
#define CTRL_LAYOUT_NAME		"Horizontal/Vertical"

// controls without child/children
#define CTRL_BUTTON				"button"
#define CTRL_BUTTON_NAME		"Button"
#define CTRL_LABEL				"label"
#define CTRL_LABEL_NAME			"Label"
#define CTRL_TEXTBOX			"textbox"
#define CTRL_TEXTBOX_NAME		"Textbox"
#define CTRL_COMBOX				"combox"
#define CTRL_COMBOX_NAME		"Combox"
#define CTRL_SPINBOX			"spinbox"
#define CTRL_SPINBOX_NAME		"Spinbox"
#define CTRL_LISTBOX			"listbox"
#define CTRL_LISTBOX_NAME		"Listbox"
#define CTRL_CHECKBOX			"checkbox"
#define CTRL_CHECKBOX_NAME		"Checkbox"
#define CTRL_DATECHOOSER		"datechooser"
#define CTRL_DATECHOOSER_NAME	"DateChooser"
#define CTRL_TOOLBAR			"toolbar"
#define CTRL_TOOLBAR_NAME		"Toolbar"



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
#define NODE_COMBOX				CTRL_COMBOX
#define NODE_SPINBOX			CTRL_SPINBOX
#define NODE_LISTBOX			CTRL_LISTBOX
#define NODE_CHECKBOX			CTRL_CHECKBOX
#define NODE_DATECHOOSER		CTRL_DATECHOOSER
#define NODE_TOOLBAR			CTRL_TOOLBAR
#define NODE_FORM				CTRL_FORM



// categories
//--------------------------------------------------------
#define CAT_CPPCODE				"C++ Code"
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
#define DEFAULT_FILENAME		"code"



// UI
//--------------------------------------------------------
#define MAIN_WDG_W				400
#define MAIN_WDG_H				300



#endif //NANA_CREATOR_CONFIG_H
