/*
 *		Creator config
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_CONFIG_H
#define NANA_CREATOR_CONFIG_H


#define CREATOR_NAME			"Nana Creator"
#define CREATOR_VERSION			"0.8.0"



// available controls
//--------------------------------------------------------
// templates/forms
#define CTRL_MAINFORM			"mainform"
#define CTRL_MAINPANEL			"mainpanel"

// controls with child/children
#define CTRL_PANEL				"panel"
#define CTRL_FORM				"form"
#define CTRL_GROUP				"group"
//
#define CTRL_LAYOUT				"layout"

// controls without child/children
#define CTRL_BUTTON				"button"
#define CTRL_LABEL				"label"
#define CTRL_TEXTBOX			"textbox"
#define CTRL_COMBOX				"combox"
#define CTRL_SPINBOX			"spinbox"
#define CTRL_LISTBOX			"listbox"
#define CTRL_CHECKBOX			"checkbox"
#define CTRL_DATECHOOSER		"datechooser"
#define CTRL_TOOLBAR			"toolbar"
#define CTRL_CATEGORIZE			"categorize"
#define CTRL_MENUBAR			"menubar"
#define CTRL_PICTURE			"picture"
#define CTRL_PROGRESS			"progress"
#define CTRL_SLIDER				"slider"
#define CTRL_TABBAR				"tabbar"
#define CTRL_TREEBOX			"treebox"



// project file
//--------------------------------------------------------
#define PROJECT_EXT				"ncp"

// nodes name
#define NODE_ROOT				"nana-creator"



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
//
#define CITEM_BILATERAL			"Bilateral"
#define CITEM_FORWD				"Forward"
#define CITEM_BACKWD			"Backward"


// code-generator
//--------------------------------------------------------
#define CTRL_NAMESPACE			"nana::"
#define DEFAULT_FILENAME		"code"


#define CREATOR_SUPPORTED_IMG	"*.bmp;*.png;*.jpg;*.jpe*"
#define CITEM_INNER_TKN			"\r"
#define CITEM_TKN				"\n"
#define CITEM_EMPTY_OWNER		" "
#define CITEM_SEPARATOR			"{sep}"
// toolbar separator
#define CITEM_TLB_SEPARATOR		CITEM_SEPARATOR CITEM_INNER_TKN CITEM_SEPARATOR


// UI
//--------------------------------------------------------
#define MAIN_WDG_W				640
#define MAIN_WDG_H				480



#endif //NANA_CREATOR_CONFIG_H
