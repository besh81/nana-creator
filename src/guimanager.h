/*
 *		guimanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_GUIMANAGER_H
#define NANA_CREATOR_GUIMANAGER_H

#include "config.h"
#include <nana/gui/widgets/toolbar.hpp>
#include "pugixml/pugixml.hpp"
#include "tree.h"
#include "ctrls/ctrl.h"
#include "ctrls/layout.h"
#include "namemanager.h"
#include "propertiespanel.h"
#include "assetspanel.h"
#include "objectspanel.h"
#include "resizablecanvas.h"
#include "statusbar.h"


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


enum class insert_position
{
	before,
	into,
	after
};


struct add_position
{
	tree_node<control_obj>*	ctrl{ 0 };
	insert_position			pos{ insert_position::into };
};



class guimanager
{
public:
	guimanager();


	void root_wd(nana::window wd)
	{
		_root_wd = wd;
	}
	void init(propertiespanel* pp, assetspanel* ap, objectspanel* op, resizablecanvas* main_wd, nana::toolbar* tb, statusbar* sb)
	{
		_pp = pp; _ap = ap; _op = op; _main_wd = main_wd; _tb = tb; _sb = sb;
		enableGUI(false);
	}
	void clear();

	void enableGUI(bool state);

	void cursor(cursor_state state);
	cursor_state cursor() { return _cursor_state; }

	void new_project(const std::string& type, const std::string& filename);

	tree_node<control_obj>* addmainctrl(const std::string& type, const std::string& name = "");
	tree_node<control_obj>* addcommonctrl(add_position add_pos, const std::string& type, const std::string& name = "");

	void deleteselected();
	void moveupselected();
	void movedownselected();

	void cutselected() { copyselected(true); }
	void copyselected(bool cut = false);
	void pasteselected();


	tree_node<control_obj>* get_root()
	{
		return _ctrls.get_root();
	}


	void updateselectedname(const std::string& name)
	{
		_updatectrlname(&_selected->value->properties, name);
	}
	void updateselected()
	{
		_updatectrl(_selected);
	}


	void left_click_ctrl(control_obj ctrl);
	bool right_click_ctrl(control_obj ctrl);
	void clickobjectspanel(const std::string& name);


	void serialize(pugi::xml_node* xml_parent);
	bool deserialize(pugi::xml_node* xml_parent);
	

private:
	control_obj _create_ctrl(control_obj parent, const std::string& type, const std::string& name);

	tree_node<control_obj>* _registerobject(control_obj ctrl, add_position add_pos);

	void _serialize(tree_node<control_obj>* node, pugi::xml_node* xml_parent, bool children_only = false);
	bool _deserialize(tree_node<control_obj>* node, pugi::xml_node* xml_parent);
	void _deserializeproperties(ctrls::properties_collection* properties, pugi::xml_node* xml_node);

	bool _updatectrlname(ctrls::properties_collection* properties, const std::string& new_name);
	void _updatectrl(tree_node<control_obj>* node, bool update_owner = true, bool update_children = true);
	void _updateparentctrl(tree_node<control_obj>* node);
	void _updatechildrenctrls(tree_node<control_obj>* node);

	void _update_op();

	void _select_ctrl(tree_node<control_obj>* to_select);


	nana::window			_root_wd;
	nana::menu				_ctxmenu;

	tree<control_obj>		_ctrls;
	tree_node<control_obj>*	_selected{ 0 };

	cursor_state			_cursor_state{ cursor_action::select };

	propertiespanel*		_pp{ 0 };
	assetspanel*			_ap{ 0 };
	objectspanel*			_op{ 0 };
	resizablecanvas*		_main_wd{ 0 };
	nana::toolbar*			_tb{ 0 };
	statusbar*				_sb{ 0 };

	namemanager				_name_mgr;	// manage the controls name used in the creator

	bool					_deserializing{ false };

	pugi::xml_document		_cut_copy_doc;
	bool					_copied{ false };

	insert_position			_insert_pos;
};

#endif //NANA_CREATOR_GUIMANAGER_H
