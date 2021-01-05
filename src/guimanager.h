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
#include "namemanager.h"
#include "propertiespanel.h"
#include "assetspanel.h"
#include "objectspanel.h"
#include "itemseditorpanel.h"
#include "scrollablecanvas.h"


typedef std::shared_ptr<ctrls::ctrl>	control_obj;
typedef std::weak_ptr<ctrls::ctrl>		control_obj_ptr;


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

enum class insert_mode
{
	before,
	into,
	after
};



class guimanager
{
public:
	guimanager(nana::window wd);

	void registerEnableGUI(std::function<void(bool state, bool new_load, bool undo, bool redo)> f) { _enableGUI_f = f; }
	void registerSetStatusbar(std::function<void(const std::string& str)> f) { _setStatusbar_f = f; }

	void init(propertiespanel* pp, assetspanel* ap, objectspanel* op, itemseditorpanel* iep, scrollablecanvas* main_wd);
	void clear();

	void enableGUI(bool state, bool new_load);

	void cursor(cursor_state state);
	cursor_state cursor() { return _cursor_state; }

	void new_project(const std::string& type, const std::string& name);

	tree_node<control_obj>* addmainctrl(const std::string& type, const std::string& name = "");
	tree_node<control_obj>* addcommonctrl(tree_node<control_obj>* node, const std::string& type, insert_mode mode, const std::string& name = "");

	void deleteselected(bool push_undo = true);
	void moveupselected(bool push_undo = true);
	void movedownselected(bool push_undo = true);

	void moveintofield() { _moveinto(_selected, move_into::field); }
	void moveintogrid() { _moveinto(_selected, move_into::grid); }
	void moveintopanel() { _moveinto(_selected, move_into::panel); }

	void cutselected() { copyselected(true); }
	void copyselected(bool cut = false);
	void pasteselected();

	tree_node<control_obj>* get_root() { return _ctrls.get_root(); }

	void updateselected() { _updatectrl(_selected); }

	bool click_ctrl(control_obj ctrl, const nana::arg_mouse& arg);
	void left_click_ctrl(control_obj ctrl);
	void click_ctrlname(const std::string& name);


	void serialize(pugi::xml_node* xml_parent);
	bool deserialize(pugi::xml_node* xml_parent);

	bool modified() { return _modified; }

	/*---------------*/
	/*   UNDO/REDO   */
	/*---------------*/
	void undo();
	void redo();
	

private:
	enum class move_into
	{
		field,
		grid,
		panel
	};

	void _error_message(const std::string& txt);

	bool _check_relationship(control_obj parent, const std::string& child_type);

	control_obj _create_ctrl(control_obj parent, const std::string& type, const std::string& name);

	tree_node<control_obj>* _registerobject(control_obj ctrl, tree_node<control_obj>* node, insert_mode mode);

	void _serialize(tree_node<control_obj>* node, pugi::xml_node* xml_parent, bool children_only = false);
	bool _deserialize(tree_node<control_obj>* node, pugi::xml_node* xml_parent, insert_mode mode, bool push_undo = false);

	bool _updatectrlname(tree_node<control_obj>* node, const std::string& new_name);
	void _updatectrl(tree_node<control_obj>* node, bool update_owner = true, bool update_children = true);
	void _updatechildrenctrls(tree_node<control_obj>* node);

	void _update_op();

	void _select_ctrl(tree_node<control_obj>* to_select);

	bool _moveinto_check_relationship(tree_node<control_obj>* ctrl, move_into into);
	void _moveinto(tree_node<control_obj>* ctrl, move_into into);


	nana::window			_root_wd;
	nana::menu				_ctxmenu;

	tree<control_obj>		_ctrls;
	tree_node<control_obj>*	_selected{ 0 };

	cursor_state			_cursor_state{ cursor_action::select };

	propertiespanel*		_pp{ 0 };
	assetspanel*			_ap{ 0 };
	objectspanel*			_op{ 0 };
	itemseditorpanel*		_iep{ 0 };
	scrollablecanvas*		_main_wd{ 0 };

	std::function<void(bool state, bool new_load, bool undo, bool redo)> _enableGUI_f;
	std::function<void(const std::string& str)> _setStatusbar_f;

	namemanager				_name_mgr;	// manage the controls name used in the creator

	bool					_deserializing{ false };
	bool					_modified{ false };
	pugi::xml_document		_cut_copy_doc;


	/*---------------*/
	/*   UNDO/REDO   */
	/*---------------*/
	enum class ur_action
	{
		add,
		remove,
		move_up,
		move_down,
		modify,
		empty
	};

	struct ur_state {
		ur_action action{ ur_action::empty };
		std::string name;
		pugi::xml_document snapshot;
	};

	void _ur_restore_ctrls(const ur_state& state);
	void _push_undo(ur_action action, tree_node<control_obj>* ctrl);

	std::deque<ur_state> _undo;
	std::deque<ur_state> _redo;
};

#endif //NANA_CREATOR_GUIMANAGER_H
