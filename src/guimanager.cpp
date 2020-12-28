/*
 *		guimanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include <cstring>
#include "ctrls/field.h"
#include "ctrls/panel.h"
#include "ctrls/button.h"
#include "ctrls/label.h"
#include "ctrls/textbox.h"
#include "ctrls/combox.h"
#include "ctrls/spinbox.h"
#include "ctrls/listbox.h"
#include "ctrls/checkbox.h"
#include "ctrls/date_chooser.h"
#include "ctrls/toolbar.h"
#include "ctrls/form.h"
#include "ctrls/categorize.h"
#include "ctrls/group.h"
#include "ctrls/menubar.h"
#include "ctrls/picture.h"
#include "ctrls/progress.h"
#include "ctrls/slider.h"
#include "ctrls/tabbar.h"
#include "ctrls/treebox.h"
#include "ctrls/splitterbar.h"
#include "ctrls/notebook.h"
#include "ctrls/page.h"
#include "ctrls/custom.h"
#include "guimanager.h"
#include "lock_guard.h"
#include "style.h"


//TODO: inserire in file ini e modificare da settings
#define UNDO_LEN		10


//guimanager
guimanager::guimanager(nana::window wd)
	: _root_wd(wd)
{
	// context menu
	// move up
	_ctxmenu.append("Move up", [this](const nana::menu::item_proxy& ip)
	{
		moveupselected();
	});
	nana::paint::image _img_up("icons/up.png");
	_ctxmenu.image(0, _img_up);
	// move down
	_ctxmenu.append("Move down", [this](const nana::menu::item_proxy& ip)
	{
		movedownselected();
	});
	nana::paint::image _img_down("icons/down.png");
	_ctxmenu.image(1, _img_down);
	// move into
	_ctxmenu.append("Move into");
	auto* sub_move_into = _ctxmenu.create_sub_menu(2);
	sub_move_into->append("field", [this](const nana::menu::item_proxy& ip)
		{
			moveintofield();
		});
	sub_move_into->image(0, nana::paint::image("icons/horizontal_layout.png"));
	sub_move_into->append("grid", [this](const nana::menu::item_proxy& ip)
		{
			moveintogrid();
		});
	sub_move_into->image(1, nana::paint::image("icons/grid_layout.png"));
	sub_move_into->append("panel", [this](const nana::menu::item_proxy& ip)
		{
			moveintopanel();
		});
	sub_move_into->image(2, nana::paint::image("icons/panel.png"));
	// -----
	_ctxmenu.append_splitter();
	// delete
	_ctxmenu.append("Delete", [this](const nana::menu::item_proxy& ip)
		{
			deleteselected();
		});
	nana::paint::image _img_del("icons/delete.png");
	_ctxmenu.image(4, _img_del);
	// -----
	_ctxmenu.append_splitter();
	// cut
	_ctxmenu.append("Cut", [this](const nana::menu::item_proxy& ip)
		{
			copyselected(true);
		});
	nana::paint::image _img_cut("icons/cut.png");
	_ctxmenu.image(6, _img_cut);
	// copy
	_ctxmenu.append("Copy", [this](const nana::menu::item_proxy& ip)
		{
			copyselected();
		});
	nana::paint::image _img_copy("icons/copy.png");
	_ctxmenu.image(7, _img_copy);
	// paste
	_ctxmenu.append("Paste", [this](const nana::menu::item_proxy& ip)
		{
			pasteselected();
		});
	nana::paint::image _img_paste("icons/paste.png");
	_ctxmenu.image(8, _img_paste);
}


void guimanager::init(propertiespanel* pp, assetspanel* ap, objectspanel* op, itemseditorpanel* iep, scrollablecanvas* main_wd)
{
	_pp = pp;
	_pp->name_changed([this](const std::string& arg)
	{
		_updatectrlname(_selected, arg);
	});
	_pp->property_changed([this](const std::string& arg)
	{
		updateselected();
	});
	
	_ap = ap;
	_ap->selected([this](const std::string& arg)
	{
		if(arg.empty())
			cursor(cursor_state{ cursor_action::select });
		else
			cursor(cursor_state{ cursor_action::add, arg });
	});
	
	_op = op;
	_op->selected([this](const std::string& arg)
	{
		click_objectspanel(arg);
	});
	_op->contex_menu(&_ctxmenu);

	_iep = iep;
	_iep->property_changed([this](const std::string& arg)
		{
			updateselected();
		});

	_main_wd = main_wd;
	
	enableGUI(false, true);
}


void guimanager::clear()
{
	_select_ctrl(0);

	auto root = _ctrls.get_root();
	if(root)
	{
		auto root_child = root->child;
		if(root_child)
			_main_wd->remove();
	}

	if(!_cut_copy_doc.empty())
		_cut_copy_doc.reset();

	_ctrls.clear();
	_op->clear();
	_pp->set(0);
	_iep->set(0, 0);
	_name_mgr.clear();

	enableGUI(false, true);
}


void guimanager::enableGUI(bool state, bool new_load)
{
	nana::API::enum_widgets(*_ap, true, [state](nana::widget &w)
		{
			w.enabled(state);
		});
	nana::API::enum_widgets(*_pp, true, [state](nana::widget &w)
		{
			w.enabled(state);
		});
	nana::API::enum_widgets(*_iep, true, [state](nana::widget& w)
		{
			w.enabled(state);
		});
	_iep->enable(state);

	if(_enableGUI_f)
		_enableGUI_f(state, new_load, !_undo.empty(), !_redo.empty());
}


void guimanager::cursor(cursor_state state)
{
	_cursor_state = state;

	//statusbar
	if(_setStatusbar_f)
		_setStatusbar_f(state.type == "" ? "" : "Add control: " + state.type);
}


void guimanager::new_project(const std::string& type, const std::string& name)
{
	if(!addmainctrl(type, name))
		return;

	enableGUI(true, true);
}


void guimanager::_error_message(const std::string& txt)
{
	nana::msgbox m(_root_wd, CREATOR_NAME, nana::msgbox::ok);
	m.icon(m.icon_error);
	m << txt;
	m();
}


bool guimanager::_check_relationship(control_obj parent, const std::string& child_type)
{
	// check parent
	if(parent->get_type() == CTRL_GRID)
		if(child_type == CTRL_FIELD || child_type == CTRL_GRID || child_type == CTRL_SPLITTERBAR)
			return false;

	if(child_type == CTRL_PAGE)
		return parent->get_type() == CTRL_NOTEBOOK ? true : false;

	// check siblings
	if(!parent->children())
		return true;

	if(parent->children_fields())
	{
		if(child_type == CTRL_FIELD || child_type == CTRL_GRID || child_type == CTRL_SPLITTERBAR)
			return true;
	}
	else
	{
		if(child_type != CTRL_FIELD && child_type != CTRL_GRID && child_type != CTRL_SPLITTERBAR)
			return true;
	}

	return false;
}


tree_node<control_obj>* guimanager::addmainctrl(const std::string& type, const std::string& name)
{
	control_obj ctrl;

	if(type == CTRL_FORM)
		ctrl = control_obj(new ctrls::form(_main_wd->get_handle(), name.empty() ? _name_mgr.add_numbered(CTRL_FORM) : name, false, _deserializing ? false : true));
	else if(type == CTRL_PANEL)
		ctrl = control_obj(new ctrls::form(_main_wd->get_handle(), name.empty() ? _name_mgr.add_numbered(CTRL_PANEL) : name, true, _deserializing ? false : true));
	else
		return 0;

	_main_wd->add(*ctrl->nanawdg);


	// events
	control_obj_ptr pctrl = ctrl;

	ctrl->connect_mouse_enter([this, pctrl](const nana::arg_mouse& arg)
	{
		if(!pctrl.lock()->highlighted() && cursor().action == cursor_action::add)
		{
			if(_check_relationship(pctrl.lock(), cursor().type))
			{
				pctrl.lock()->highlight(ctrls::highlight_mode::into);
				pctrl.lock()->refresh();
			}
		}
	});

	ctrl->connect_mouse_leave([pctrl](const nana::arg_mouse& arg)
	{
		if(pctrl.lock()->highlighted())
		{
			pctrl.lock()->highlight(ctrls::highlight_mode::none);
			pctrl.lock()->refresh();
		}
	});

	// mouse click
	ctrl->connect_mouse_down([this, pctrl](const nana::arg_mouse& arg)
	{
		if(click_ctrl(pctrl.lock(), arg))
			arg.stop_propagation();
	});

	return _registerobject(ctrl, 0, insert_mode::into);
}


control_obj guimanager::_create_ctrl(control_obj parent, const std::string& type, const std::string& name)
{
	if(type == CTRL_FIELD)
		return control_obj(new ctrls::field(parent.get(), name, false, _deserializing ? false : true));
	else if(type == CTRL_GRID)
		return control_obj(new ctrls::field(parent.get(), name, true, _deserializing ? false : true));
	else if(type == CTRL_SPLITTERBAR)
		return control_obj(new ctrls::splitterbar(parent.get(), name));
	else if(type == CTRL_PANEL)
		return control_obj(new ctrls::panel(parent.get(), name, _deserializing ? false : true));
	else if(type == CTRL_GROUP)
		return control_obj(new ctrls::group(parent.get(), name, _deserializing ? false : true));
	else if(type == CTRL_BUTTON)
		return control_obj(new ctrls::button(parent.get(), name));
	else if(type == CTRL_LABEL)
		return control_obj(new ctrls::label(parent.get(), name));
	else if(type == CTRL_TEXTBOX)
		return control_obj(new ctrls::textbox(parent.get(), name));
	else if(type == CTRL_COMBOX)
		return control_obj(new ctrls::combox(parent.get(), name));
	else if(type == CTRL_CUSTOM)
		return control_obj(new ctrls::custom(parent.get(), name));
	else if(type == CTRL_SPINBOX)
		return control_obj(new ctrls::spinbox(parent.get(), name));
	else if(type == CTRL_LISTBOX)
		return control_obj(new ctrls::listbox(parent.get(), name));
	else if(type == CTRL_CHECKBOX)
		return control_obj(new ctrls::checkbox(parent.get(), name));
	else if(type == CTRL_DATECHOOSER)
		return control_obj(new ctrls::date_chooser(parent.get(), name));
	else if(type == CTRL_TOOLBAR)
		return control_obj(new ctrls::toolbar(parent.get(), name));
	else if(type == CTRL_CATEGORIZE)
		return control_obj(new ctrls::categorize(parent.get(), name));
	else if(type == CTRL_MENUBAR)
		return control_obj(new ctrls::menubar(parent.get(), name));
	else if(type == CTRL_PICTURE)
		return control_obj(new ctrls::picture(parent.get(), name));
	else if(type == CTRL_PROGRESS)
		return control_obj(new ctrls::progress(parent.get(), name));
	else if(type == CTRL_SLIDER)
		return control_obj(new ctrls::slider(parent.get(), name));
	else if(type == CTRL_TABBAR)
		return control_obj(new ctrls::tabbar(parent.get(), name));
	else if(type == CTRL_TREEBOX)
		return control_obj(new ctrls::treebox(parent.get(), name));
	else if(type == CTRL_NOTEBOOK)
		return control_obj(new ctrls::notebook(parent.get(), name));
	else if(type == CTRL_PAGE)
		return control_obj(new ctrls::page(parent.get(), name, _deserializing ? false : true));
	
	return 0;
}


tree_node<control_obj>* guimanager::addcommonctrl(tree_node<control_obj>* node, const std::string& type, insert_mode mode, const std::string& name)
{
	control_obj parent_ = (mode == insert_mode::into) ? node->value : node->owner->value;

	std::string name_ = name.empty() ? _name_mgr.add_numbered(type) : name;

	// nana::widget + properties
	control_obj ctrl = _create_ctrl(parent_, type, name_);
	if(ctrl == 0)
		return 0;


	// append/insert
	if(mode == insert_mode::into)
		parent_->append(ctrl.get());
	else
		parent_->insert(ctrl.get(), node->value.get(), (mode == insert_mode::after) ? true : false);


	// events
	control_obj_ptr pctrl = ctrl;
	control_obj_ptr pparent = parent_;

	// mouse enter
	if(type == CTRL_FIELD || type == CTRL_GRID || type == CTRL_PANEL || type == CTRL_GROUP || type == CTRL_PAGE)
	{
		ctrl->connect_mouse_enter([this, pctrl](const nana::arg_mouse& arg)
		{
			if(!pctrl.lock()->highlighted() && cursor().action == cursor_action::add)
			{
				if(_check_relationship(pctrl.lock(), cursor().type))
				{
					pctrl.lock()->highlight(ctrls::highlight_mode::into);
					pctrl.lock()->refresh();
				}
			}
		});
	}
	else if(type == CTRL_NOTEBOOK)
	{
		ctrl->connect_mouse_enter([this, pctrl, pparent](const nana::arg_mouse& arg)
		{
			if(!pctrl.lock()->highlighted() && cursor().action == cursor_action::add)
			{
				if(cursor().type == CTRL_PAGE)
				{
					pctrl.lock()->highlight(ctrls::highlight_mode::into);
					pctrl.lock()->refresh();
				}
				else
				{
					if(_check_relationship(pparent.lock(), cursor().type))
					{
						pctrl.lock()->highlight(ctrls::highlight_mode::before_after);
						pparent.lock()->highlight(ctrls::highlight_mode::into);
						pctrl.lock()->refresh();
						pparent.lock()->refresh();
					}
				}
			}
		});
	}
	else
	{
		ctrl->connect_mouse_enter([this, pctrl, pparent](const nana::arg_mouse& arg)
		{
			if(!pctrl.lock()->highlighted() && cursor().action == cursor_action::add)
			{
				if(_check_relationship(pparent.lock(), cursor().type))
				{
					pctrl.lock()->highlight(ctrls::highlight_mode::before_after);
					pparent.lock()->highlight(ctrls::highlight_mode::into);
					pctrl.lock()->refresh();
					pparent.lock()->refresh();
				}
			}
		});
	}

	// mouse leave
	if(type == CTRL_FIELD || type == CTRL_GRID || type == CTRL_PANEL || type == CTRL_GROUP || type == CTRL_PAGE)
	{
		ctrl->connect_mouse_leave([pctrl](const nana::arg_mouse& arg)
		{
			if(pctrl.lock()->highlighted())
			{
				pctrl.lock()->highlight(ctrls::highlight_mode::none);
				pctrl.lock()->refresh();
			}
		});
	}
	else
	{
		ctrl->connect_mouse_leave([pctrl, pparent](const nana::arg_mouse& arg)
		{
			if(pctrl.lock()->highlighted())
			{
				pctrl.lock()->highlight(ctrls::highlight_mode::none);
				pparent.lock()->highlight(ctrls::highlight_mode::none);
				pctrl.lock()->refresh();
				pparent.lock()->refresh();
			}
		});
	}

	// mouse click
	ctrl->connect_mouse_down([this, pctrl](const nana::arg_mouse& arg)
	{
		if(click_ctrl(pctrl.lock(), arg))
			arg.stop_propagation();
	});

	if(type == CTRL_NOTEBOOK)
	{
		ctrls::notebook* ntb = static_cast<ctrls::notebook*>(ctrl.get());
		ntb->connect_tab_click([this, pctrl, ntb](const nana::arg_tabbar_mouse<size_t>& arg)
		{
			if(cursor().action == cursor_action::add)
			{
				if(click_ctrl(pctrl.lock(), arg))
					arg.stop_propagation();
				return;
			}

			ctrls::ctrl* page = ntb->get_page(arg.item_pos);
			if(!page)
				return;

			// search control
			control_obj_ptr	page_wptr;

			_ctrls.for_each([page, &page_wptr](tree_node<control_obj>* node) -> bool
				{
					if(node->value.get() == page)
					{
						page_wptr = node->value;
						return false;
					}
					return true;
				});

			if(page_wptr.expired())
				return;

			click_ctrl(page_wptr.lock(), arg);
		});
	}


	return _registerobject(ctrl, node, mode);
}


void guimanager::deleteselected()
{
	if(!_selected || _selected == _ctrls.get_root()->child) // main widget cannot be removed
		return;

	//_push_state();

	auto toremove = _selected;
	auto parent = toremove->owner;
	_select_ctrl(0);


	// delete ctrl name
	_ctrls.for_each(toremove, [this](tree_node<control_obj>* node) -> bool
		{
			_name_mgr.remove(node->value->properties.property("name").as_string());
			return true;
		});
	

	// delete ctrl
	if(toremove == _ctrls.get_root()->child)
	{
		parent = 0;
		_pp->set(0);
		_iep->set(0, 0);
		_main_wd->remove();
	}

	_ctrls.recursive_backward(toremove, [this](tree_node<control_obj>* node) -> bool
	{
		if(node->owner)
		{
			control_obj parent_ = node->owner->value;
			if(parent_)
				parent_->remove(node->value.get());
		}

		_ctrls.remove(node);

		return true;
	});


	// delete objectspanel item
	_update_op();

	// select parent
	if(parent)
	{
		parent->value->refresh();
		left_click_ctrl(parent->value);
	}
}


void guimanager::moveupselected(bool push_undo)
{
	if(!_selected)
		return;

	// move one position up
	if(!_ctrls.move_before_sibling(_selected))
		return;

	if(push_undo)
		_push_undo(ur_action::move_up, _selected);

	// if here is possible to move up
	auto parent = _selected->owner->value;
	parent->moveup(_selected->value.get());

	// reorder objectspanel item
	_update_op();
}


void guimanager::movedownselected(bool push_undo)
{
	if(!_selected)
		return;

	// move one position down
	if(!_ctrls.move_after_sibling(_selected))
		return;

	if(push_undo)
		_push_undo(ur_action::move_down, _selected);

	// if here is possible to move down
	auto parent = _selected->owner->value;
	parent->movedown(_selected->value.get());

	// reorder objectspanel item
	_update_op();
}


bool guimanager::_moveinto_check_relationship(tree_node<control_obj>* ctrl, move_into into)
{
	if(!ctrl)
		return false;

	auto type = ctrl->value->properties.property("type").as_string();
	if(type == CTRL_PAGE || type == CTRL_FORM || ctrl == _ctrls.get_root()->child)
	{
		_error_message("Impossible to move the selected control!");
		return false;
	}

	auto parent = ctrl->owner;
	auto parent_type = parent->value->properties.property("type").as_string();

	if(into == move_into::field || into == move_into::grid)
	{
		if(parent_type == CTRL_GRID)
		{
			_error_message("Impossible to create field/grid inside a grid!");
			return false;
		}
	}

	if(into == move_into::grid)
	{
		if(type == CTRL_FIELD || type == CTRL_GRID)
		{
			_error_message("Impossible to create field/grid inside a grid!");
			return false;
		}
	}

	return true;
}

void guimanager::_moveinto(tree_node<control_obj>* ctrl, move_into into)
{
	if(!_moveinto_check_relationship(ctrl, into))
		return;

	//_push_state();

	auto parent = ctrl->owner;
	_select_ctrl(0);

	// disable GUI
	enableGUI(false, false);
	lock_guard des_lock(&_deserializing, true);
	_op->emit_events(false);
	_op->auto_draw(false);

	// serialize ctrls to move
	pugi::xml_document to_move;
	pugi::xml_node root = to_move.append_child(NODE_ROOT);
	_serialize(parent->child, &root, false);

	// delete ctrls to move
	int num_siblings = 0;
	auto sibling = parent->child;
	while(sibling)
	{
		num_siblings++;

		// delete ctrls name
		_ctrls.for_each(sibling, [this](tree_node<control_obj>* node) -> bool
			{
				_name_mgr.remove(node->value->properties.property("name").as_string());
				return true;
			});

		auto next = sibling->next;

		_ctrls.recursive_backward(sibling, [this](tree_node<control_obj>* node) -> bool
			{
				if(node->owner)
				{
					control_obj parent_ = node->owner->value;
					if(parent_)
						parent_->remove(node->value.get());
				}

				_ctrls.remove(node);

				return true;
			});

		sibling = next;
	}

	// add new field node
	tree_node<control_obj>* new_node = nullptr;
	if(into == move_into::field)
	{
		new_node = addcommonctrl(parent, CTRL_FIELD, insert_mode::into);
		// adjust layout orientation
		new_node->value->properties.property("layout").value(parent->value->properties.property("layout").as_int());
	}
	else if(into == move_into::grid)
	{
		new_node = addcommonctrl(parent, CTRL_GRID, insert_mode::into);
		// adjust grid size
		int n = std::max(int(ceil(sqrt(num_siblings))), 2);
		new_node->value->properties.property("columns").value(n);
		new_node->value->properties.property("rows").value(n);
	}
	else if(into == move_into::panel)
	{
		new_node = addcommonctrl(parent, CTRL_PANEL, insert_mode::into);
		new_node->value->properties.property("layout").value(parent->value->properties.property("layout").as_int());
	}

	// deserialize ctrls to move
	if(!_deserialize(new_node, &root, true))
	{
		_error_message("Unable to move the selected controls!");
	}

	// enable GUI
	_op->auto_draw(true);
	_op->emit_events(true);
	_update_op();
	enableGUI(true, true);

	// select new_node
	if(new_node)
	{
		new_node->value->refresh();
		left_click_ctrl(new_node->value);
	}
}



void guimanager::copyselected(bool cut)
{
	if(!_selected)
		return;

	// main widget cannot be cut or copied
	if(_selected == _ctrls.get_root()->child)
	{
		_error_message("Impossible to cut/copy the main widget!");
		return;
	}

	if(!_cut_copy_doc.empty())
		_cut_copy_doc.reset();
	_copied = !cut;

	// append root node
	pugi::xml_node root = _cut_copy_doc.append_child(NODE_ROOT);
	_serialize(_selected, &root, true);

	if(cut)
		deleteselected(); // erase ctrls
}


void guimanager::pasteselected()
{
	// read root node
	pugi::xml_node root = _cut_copy_doc.child(NODE_ROOT);
	if(root.empty())
		return; // nothing to paste

	if(!_selected)
		return;

	auto prev_selected = _selected;

	auto type = prev_selected->value->properties.property("type").as_string();
	if(type == CTRL_FIELD || type == CTRL_GRID || type == CTRL_PANEL || type == CTRL_GROUP || type == CTRL_PAGE || type == CTRL_FORM)
	{
		// do nothing !!!
	}
	else if(type == CTRL_NOTEBOOK && (std::strcmp(root.first_child().name(), CTRL_PAGE) == 0))
	{
		// do nothing !!!
	}
	else
	{
		_select_ctrl(_selected->owner);
		if(!_selected)
			return;
	}

	// deserialize the XML structure and avoid window update
	enableGUI(false, false);
	lock_guard des_lock(&_deserializing, true);
	_op->emit_events(false);
	_op->auto_draw(false);

	bool paste_ok = _deserialize(_selected, &root, true);
	if(!paste_ok)
		_error_message("Unable to move the selected controls!");

	_op->auto_draw(true);
	_op->emit_events(true);
	_update_op();
	enableGUI(true, true);

	if(!_copied && paste_ok)  // cut items can be paste only once
		if(!_cut_copy_doc.empty())
			_cut_copy_doc.reset();

	// select previous ctrl
	left_click_ctrl(prev_selected->value);
}


bool guimanager::click_ctrl(control_obj ctrl, const nana::arg_mouse& arg)
{
	// search control
	tree_node<control_obj>*	_ctrl_node{ 0 };

	_ctrls.for_each([&ctrl, &_ctrl_node](tree_node<control_obj>* node) -> bool
		{
			if(node->value == ctrl)
			{
				_ctrl_node = node;
				return false;
			}
			return true;
		});

	if(!_ctrl_node)
		return false; // continue propagation


	// select
	//---------------------
	if(cursor().action == cursor_action::select)
	{
		_select_ctrl(_ctrl_node);

		// select objectspanel item
		_op->emit_events(false);
		_op->select(ctrl->properties.property("name").as_string());
		_op->emit_events(true);

		// set properties panel
		_pp->set(&ctrl->properties);
		// set items editor panel
		_iep->set(&ctrl->properties, &ctrl->items);

		if(arg.left_button)
			return true; // stop propagation

		if(arg.right_button)
		{
			_ctxmenu.popup(*ctrl->nanawdg, arg.pos.x, arg.pos.y);
			return true; // stop propagation
		}

		return false; // continue propagation
	}


	// add
	//---------------------
	if(cursor().action == cursor_action::add)
	{
		if(arg.left_button)
		{
			if(!ctrl->highlighted())
			{
				//TODO message box con errore
				return true; // stop propagation
			}

			insert_mode mode;
			if(ctrl->highlight() == ctrls::highlight_mode::into)
				mode = insert_mode::into;
			else // ctrls::highlight_mode::before_after
			{
				auto cursor_pos = ctrl->get_cursor_pos();
				if(cursor_pos == ctrls::mouse_position::up_right || cursor_pos == ctrls::mouse_position::down_right)
					mode = insert_mode::after;
				else
					mode = insert_mode::before;
			}


			// deselect previous ctrl
			_ap->deselect();

			// reset ctrl highlight
			ctrl->highlight(ctrls::highlight_mode::none);
			ctrl->refresh();

			// reset parent ctrl highlight
			auto _ctrl_node_parent = _ctrl_node->owner;
			if(_ctrl_node_parent)
			{
				if(_ctrl_node_parent->value)
				{
					_ctrl_node_parent->value->highlight(ctrls::highlight_mode::none);
					_ctrl_node_parent->value->refresh();
				}
			}

			// add ctrl
			if(!addcommonctrl(_ctrl_node, cursor().type, mode))
			{
				_error_message("Unable to add control!");
			}

			// reset mouse cursor
			cursor(cursor_state{ cursor_action::select });

			return true; // stop propagation
		}

		if(arg.right_button)
		{
			// deselect previous ctrl
			_ap->deselect();

			// reset ctrl highlight
			ctrl->highlight(ctrls::highlight_mode::none);
			ctrl->refresh();

			// reset parent ctrl highlight
			auto _ctrl_node_parent = _ctrl_node->owner;
			if(_ctrl_node_parent)
			{
				if(_ctrl_node_parent->value)
				{
					_ctrl_node_parent->value->highlight(ctrls::highlight_mode::none);
					_ctrl_node_parent->value->refresh();
				}
			}

			// reset mouse cursor
			cursor(cursor_state{ cursor_action::select });

			return true; // stop propagation
		}
	}

	return false; // continue propagation
}


void guimanager::left_click_ctrl(control_obj ctrl)
{
	nana::arg_mouse arg;
	arg.left_button = true;
	click_ctrl(ctrl, arg);
}


void guimanager::click_objectspanel(const std::string& name)
{
	_ctrls.for_each([this, name](tree_node<control_obj>* node) -> bool
		{
			if(node->value->properties.property("name").as_string() == name)
			{
				_select_ctrl(node);

				// set properties panel
				_pp->set(&node->value->properties);
				// set items editor panel
				_iep->set(&node->value->properties, &node->value->items);

				// set focus to new object
				node->value->nanawdg->focus();

				return false;
			}

			return true;
		});
}


void guimanager::serialize(pugi::xml_node* xml_parent)
{
	_serialize(0, xml_parent);
}

void guimanager::_serialize(tree_node<control_obj>* node, pugi::xml_node* xml_parent, bool children_only)
{
	pugi::xml_node xml_child;
	if(!node)
	{
		node = _ctrls.get_root();
		xml_child = *xml_parent;
	}
	else
	{
		xml_child = xml_parent->append_child(node->value->properties.property("type").as_string().c_str());

		// serialize attributes
		for(size_t i = 0; i < node->value->properties.count(); ++i)
			xml_child.append_attribute(node->value->properties[i].name().c_str()) = node->value->properties[i].as_string().c_str();

		// serialize items
		node->value->items.for_each([&xml_child](tree_node<ctrls::properties_collection>* node) -> bool
			{
				auto xml_item = xml_child.append_child(NODE_ITEM);

				for(size_t i = 0; i < node->value.count(); ++i)
					xml_item.append_attribute(node->value[i].name().c_str()) = node->value[i].as_string().c_str();
				return true;
			});
	}

	if(node->child)
		_serialize(node->child, &xml_child);

	if(node->next && !children_only)
		_serialize(node->next, xml_parent);
}


bool guimanager::deserialize(pugi::xml_node* xml_parent)
{
	enableGUI(false, false);
	lock_guard des_lock(&_deserializing, true);
	_op->emit_events(false);
	_op->auto_draw(false);

	bool ret_val = _deserialize(0, xml_parent);

	_op->auto_draw(true);
	_op->emit_events(true);
	_update_op();
	enableGUI(true, true);

	if(!ret_val)
		return false;

	if(_ctrls.get_root()->child)
	{
		// show main panel
		_ctrls.get_root()->child->value->nanawdg->show();

		// select main panel
		left_click_ctrl(_ctrls.get_root()->child->value);
	}
	return true;
}

bool guimanager::_deserialize(tree_node<control_obj>* parent, pugi::xml_node* xml_parent, bool paste)
{
	// read children
	for(pugi::xml_node xml_node = xml_parent->first_child(); xml_node; xml_node = xml_node.next_sibling())
	{
		tree_node<control_obj>* node = 0;

		std::string node_name = xml_node.name();
		std::string ctrl_name = xml_node.attribute("name").as_string();


		if(node_name == NODE_ITEM)
		{
			ctrls::properties_collection item;

			// init item properties
			if(parent->value->get_type() == CTRL_GRID)
				ctrls::field::init_item(item);
			else if(parent->value->get_type() == CTRL_COMBOX)
				ctrls::combox::init_item(item);
			else if(parent->value->get_type() == CTRL_LISTBOX)
				ctrls::listbox::init_item(item);
			else if(parent->value->get_type() == CTRL_MENUBAR)
				ctrls::menubar::init_item(item, xml_node.attribute("type").as_string());
			else if(parent->value->get_type() == CTRL_TABBAR)
				ctrls::tabbar::init_item(item);
			else if(parent->value->get_type() == CTRL_TOOLBAR)
				ctrls::toolbar::init_item(item, xml_node.attribute("type").as_string());

			// deserialize attributes
			for(auto i = xml_node.attributes_begin(); i != xml_node.attributes_end(); ++i)
				item.property(i->name()).value(i->value());

			if(item.property("owner").as_string().empty())
			{
				parent->value->items.append(item);
			}
			else
			{
				parent->value->items.for_each([&item](tree_node<ctrls::properties_collection>* node) -> bool
					{
						if(item.property("owner").as_string() == node->value.property("key").as_string())
						{
							node->append(item);
							return false;
						}
						return true;
					});
			}

			continue;
		}

		// add name to name manager (and check)
		bool ctrl_name_changed = false;
		if(!_name_mgr.add(ctrl_name))
		{
			ctrl_name = _name_mgr.add_numbered(ctrl_name);
			ctrl_name_changed = true;
		}

		if(xml_node.attribute("mainclass").as_bool())
		{
			node = addmainctrl(node_name, ctrl_name);
		}
		else
		{
			// check parent and siblings
			if(_check_relationship(parent->value, xml_node.attribute("type").as_string()))
				node = addcommonctrl(parent, node_name, insert_mode::into, ctrl_name);
		}

		if(node == 0)
		{
			if(paste)
				return false;

			//TODO message box con errore
			std::cout << "UNKNOWN NODE: " << xml_node.name() << std::endl;
			continue;
		}

		// deserialize attributes
		for(auto i = xml_node.attributes_begin(); i != xml_node.attributes_end(); ++i)
			node->value->properties.property(i->name()).value(i->value());

		// align control name
		if(ctrl_name_changed)
			node->value->properties.property("name") = ctrl_name;

		// deserialize children
		if(!_deserialize(node, &xml_node))
			return false;

		// update nana::widget
		_updatectrl(node);
	}

	return true;
}


tree_node<control_obj>* guimanager::_registerobject(control_obj ctrl, tree_node<control_obj>* node, insert_mode mode)
{
	tree_node<control_obj>* child = 0;

	// append/insert to controls tree
	if(!node)
		child = _ctrls.append(0, ctrl);
	else if(mode == insert_mode::into)
		child = _ctrls.append(node, ctrl);
	else if(mode == insert_mode::after)
		child = _ctrls.insert_after(node, ctrl);
	else
		child = _ctrls.insert_before(node, ctrl);


	_updatectrl(child);


	// select new control
	_select_ctrl(child);

	if(!_deserializing)
	{
		_update_op();

		// set properties panel
		_pp->set(&ctrl->properties);
		// set items editor panel
		_iep->set(&ctrl->properties, &ctrl->items);

		// set focus to new object
		ctrl->nanawdg->focus();

		// reset mouse cursor
		cursor(cursor_state{ cursor_action::select });
	}

	return _selected;
}


bool guimanager::_updatectrlname(tree_node<control_obj>* node, const std::string& new_name)
{
	ctrls::properties_collection* properties = &_selected->value->properties;

	if(properties->property("name").as_string() == new_name)
		return false;

	// update name manager
	if(!_name_mgr.add(new_name))
		return false;
	_name_mgr.remove(properties->property("name").as_string());

	// update properties
	properties->property("name") = new_name;

	// update objects panel
	_update_op();

	return true;
}


void guimanager::_updatectrl(tree_node<control_obj>* node, bool update_owner, bool update_children)
{
	node->value->update();

	// needs to update scroll bars
	if(node->value->properties.property("mainclass").as_bool())
		_main_wd->refresh();

	// update children ctrls
	if(node->child && update_children)
		_updatechildrenctrls(node);

	// update parent ctrl
	if(node->owner && update_owner)
	{
		if(node->owner->value)
			node->owner->value->update();
	}
}


void guimanager::_updatechildrenctrls(tree_node<control_obj>* node)
{
	auto* this_node = node;
	_ctrls.for_each(node, [this, this_node](tree_node<control_obj>* node) -> bool
		{
			if(this_node == node)
				return true;

			_updatectrl(node, false, false);
			return true;
		});
}


void guimanager::_update_op()
{
	_op->emit_events(false);
	_op->auto_draw(false);

	_op->clear();

	_ctrls.for_each([this](tree_node<control_obj>* node) -> bool
		{
			auto ctrl = node->value;

			if(node->owner->value)
			{
				auto parent = node->owner->value;
				_op->append(parent->properties.property("name").as_string(), ctrl->properties.property("name").as_string(), ctrl->properties.property("type").as_string());
			}
			else
				_op->append("", ctrl->properties.property("name").as_string(), ctrl->properties.property("type").as_string());
			return true;
		});

	_op->auto_draw(true);
	_op->emit_events(true);
	_op->refresh();

	if(_selected)
		_op->select(_selected->value->properties.property("name").as_string());
}

void guimanager::_select_ctrl(tree_node<control_obj>* to_select)
{
	if(_selected == to_select)
		return;

	if(_selected && _selected->value)
	{
		_selected->value->select(false);
		_selected->value->refresh();
	}

	_selected = to_select;

	if(_selected && _selected->value)
	{
		// if selected ctrl is inside a notebook then show the container page
		auto pctrl = _selected->owner;
		while(pctrl && pctrl->value)
		{
			if(pctrl->value->get_type() == CTRL_PAGE)
			{
				ctrls::page* pag = static_cast<ctrls::page*>(pctrl->value.get());
				ctrls::notebook* ntb = static_cast<ctrls::notebook*>(pctrl->owner->value.get());
				ntb->show_page(pag);
			}
			pctrl = pctrl->owner;
		}


		_selected->value->select(true);
		_selected->value->refresh();
	}
}


/*---------------*/
/*   UNDO/REDO   */
/*---------------*/
void guimanager::undo()
{
	if(_undo.size() == 0)
		return;

	enableGUI(false, false);

	auto& ustate = _undo.back();
	switch(ustate.action)
	{
	case ur_action::move_up:
		_select_ctrl(ustate.ctrl);
		movedownselected(false);
		break;
	case ur_action::move_down:
		_select_ctrl(ustate.ctrl);
		moveupselected(false);
		break;
	default:
		// error
		break;
	}

	auto& rstate = _redo.emplace_back();
	rstate = ustate;

	_undo.pop_back();

	enableGUI(true, false);
}


void guimanager::redo()
{
	if(_redo.size() == 0)
		return;

	enableGUI(false, false);

	auto& rstate = _redo.back();
	switch(rstate.action)
	{
	case ur_action::move_up:
		_select_ctrl(rstate.ctrl);
		moveupselected(false);
		break;
	case ur_action::move_down:
		_select_ctrl(rstate.ctrl);
		movedownselected(false);
		break;
	default:
		// error
		break;
	}

	auto& ustate = _undo.emplace_back();
	ustate = rstate;

	_redo.pop_back();

	enableGUI(true, false);
}

/*
void guimanager::_queue_push(std::deque<pugi::xml_document>* q)
{
	pugi::xml_document& udoc = q->emplace_back();

	pugi::xml_node root = udoc.append_child(NODE_ROOT);
	serialize(&root);
}
*/

void guimanager::_push_undo(ur_action action, tree_node<control_obj>* ctrl)
{
	_modified = true;

	auto& ustate = _undo.emplace_back();
	ustate.action = action;
	ustate.ctrl = ctrl;

	// invalidate REDO QUEUE
	_redo.clear();

	if(_enableGUI_f)
		_enableGUI_f(true, false, !_undo.empty(), !_redo.empty());
}
