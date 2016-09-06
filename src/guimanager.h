/*
 *		guimanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_GUIMANAGER_H
#define NANA_CREATOR_GUIMANAGER_H

#include "config.h"
#include "pugixml/pugixml.hpp"
#include "tree.h"
#include "ctrls/ctrl.h"
#include "ctrls/layout.h"
#include "namemanager.h"
#include "codegenerator.h"
#include "propertiespanel.h"
#include "assetspanel.h"
#include "objectspanel.h"
#include "resizablecanvas.h"


enum class cursor_action
{
	select,
	add
};


struct cursor_state
{
	cursor_action	action;
	std::string		type;
};



class guimanager
{
	friend class codegenerator;

public:
	guimanager() = default;


	void root_wd(nana::window wd)
	{
		_root_wd = wd;
	}
	void init(propertiespanel* pp, assetspanel* ap, objectspanel* op, resizablecanvas* main_wd)
	{
		_pp = pp; _ap = ap; _op = op; _main_wd = main_wd;
	}
	void clear();


	void cursor(cursor_state state);
	cursor_state cursor() { return _cursor_state; }


	tree_node<control_struct>* addmainpanel(const std::string& name = "");
	tree_node<control_struct>* addlayout(tree_node<control_struct>* parent, ctrls::layout_orientation layout, const std::string& name = "");
	tree_node<control_struct>* addcommonctrl(tree_node<control_struct>* parent, const std::string& type, const std::string& name = "");

	void deleteselected();
	void moveupselected();
	void movedownselected();


	void updateselectedname(const std::string& name)
	{
		_updatectrlname(&_selected->value->properties, name);
	}
	void updateselected()
	{
		_updatectrl(_selected);
	}


	void clickctrl(control_struct ctrl);
	void clickobjectspanel(const std::string& name);


	void serialize(pugi::xml_node* xml_parent);
	void serialize(tree_node<control_struct>* node, pugi::xml_node* xml_parent);

	bool deserialize(pugi::xml_node* xml_parent);
	bool deserialize(tree_node<control_struct>* node, pugi::xml_node* xml_parent);

private:
	bool _checksonship(const std::string& child, const std::string& parent);

	tree_node<control_struct>* _registerobject(control_struct ctrl, tree_node<control_struct>* parent);

	void _deserializeproperties(ctrls::properties_collection* properties, pugi::xml_node* xml_node);

	bool _updatectrlname(ctrls::properties_collection* properties, const std::string& new_name);
	void _updatectrl(tree_node<control_struct>* node, bool update_owner = true, bool update_children = true);
	void _updateparentctrl(tree_node<control_struct>* node);
	void _updatechildrenctrls(tree_node<control_struct>* node);


	nana::window			_root_wd;

	tree<control_struct>		_ctrls;
	tree_node<control_struct>*	_selected;

	cursor_state			_cursor_state{ cursor_action::select };

	propertiespanel*		_pp{ 0 };
	assetspanel*			_ap{ 0 };
	objectspanel*			_op{ 0 };
	resizablecanvas*		_main_wd{ 0 };

	namemanager				_name_mgr;	// manage the controls name used in the creator
};

#endif //NANA_CREATOR_GUIMANAGER_H
