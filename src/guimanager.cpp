/*
 *		guimanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
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
#include "guimanager.h"
#include "lock_guard.h"
#include "style.h"


// utility functions

void draw_selection(nana::paint::graphics& graph)
{
	graph.rectangle(false, SELECT_CTRL_COL);
	graph.rectangle({ 1, 1, SELECT_CTRL_SIZE, SELECT_CTRL_SIZE }, true, SELECT_CTRL_COL);
	graph.rectangle({ 1, static_cast<int>(graph.height()) - SELECT_CTRL_SIZE -1, SELECT_CTRL_SIZE, SELECT_CTRL_SIZE }, true, SELECT_CTRL_COL);
	graph.rectangle({ static_cast<int>(graph.width()) - SELECT_CTRL_SIZE - 1, 1, SELECT_CTRL_SIZE, SELECT_CTRL_SIZE }, true, SELECT_CTRL_COL);
	graph.rectangle({ static_cast<int>(graph.width()) - SELECT_CTRL_SIZE - 1, static_cast<int>(graph.height()) - SELECT_CTRL_SIZE - 1, SELECT_CTRL_SIZE, SELECT_CTRL_SIZE }, true, SELECT_CTRL_COL);
}

void draw_highlight(nana::paint::graphics& graph, insert_position type)
{
	if(type == insert_position::into)
		graph.blend({ 0, 0, graph.width(), graph.height() }, HIGHLIGHT_CTRL_COL, 0.5);
	else if(type == insert_position::before)
		graph.blend({ 0, 0, graph.width()/2, graph.height() }, HIGHLIGHT_CTRL_COL, 0.5);
	else
		graph.blend({ static_cast<int>(graph.width()) / 2, 0, graph.width()/2, graph.height() }, HIGHLIGHT_CTRL_COL, 0.5);
}



//guimanager
guimanager::guimanager()
{
	// context menu
	// 0. move up
	_ctxmenu.append("Move Up", [this](const nana::menu::item_proxy& ip)
	{
		moveupselected();
	});
	nana::paint::image _img_up("icons/up.png");
	_ctxmenu.image(0, _img_up);
	// 1. move down
	_ctxmenu.append("Move Down", [this](const nana::menu::item_proxy& ip)
	{
		movedownselected();
	});
	nana::paint::image _img_down("icons/down.png");
	_ctxmenu.image(1, _img_down);
	// 2. -----
	_ctxmenu.append_splitter();
	// 3. delete
	_ctxmenu.append("Delete", [this](const nana::menu::item_proxy& ip)
	{
		deleteselected();
	});
	nana::paint::image _img_del("icons/delete.png");
	_ctxmenu.image(3, _img_del);
	// 4. -----
	_ctxmenu.append_splitter();
	// 5. cut
	_ctxmenu.append("Cut", [this](const nana::menu::item_proxy& ip)
	{
		copyselected(true);
	});
	nana::paint::image _img_cut("icons/cut.png");
	_ctxmenu.image(5, _img_cut);
	// 6. copy
	_ctxmenu.append("Copy", [this](const nana::menu::item_proxy& ip)
	{
		copyselected();
	});
	nana::paint::image _img_copy("icons/copy.png");
	_ctxmenu.image(6, _img_copy);
	// 7. paste
	_ctxmenu.append("Paste", [this](const nana::menu::item_proxy& ip)
	{
		pasteselected();
	});
	nana::paint::image _img_paste("icons/paste.png");
	_ctxmenu.image(7, _img_paste);
}


void guimanager::clear()
{
	_select_ctrl(0);

	auto root = _ctrls.get_root();
	if(root)
	{
		auto root_child = root->child;
		if(root_child)
		{
			_main_wd->remove(*root_child->value->nanawdg);
		}
	}

	_cut_copy_doc.reset();

	_ctrls.clear();
	_op->clear();
	_pp->set(0);
	_name_mgr.clear();

	enableGUI(false);
}


void guimanager::enableGUI(bool state)
{
	nana::API::enum_widgets(*_ap, true, [state](nana::widget &w)
	{
		w.enabled(state);
	});
	nana::API::enum_widgets(*_pp, true, [state](nana::widget &w)
	{
		w.enabled(state);
	});

	_tb->enable(TB_SAVE, state);
	_tb->enable(TB_SAVE_AS, state);
	_tb->enable(TB_GENERATE, state);
	_tb->enable(TB_DELETE, state);
	_tb->enable(TB_UP, state);
	_tb->enable(TB_DOWN, state);
	_tb->enable(TB_CUT, state);
	_tb->enable(TB_COPY, state);
	_tb->enable(TB_PASTE, state);
}


void guimanager::cursor(cursor_state state)
{
	if(state.action == cursor_action::add)
	{
		if(!_main_wd->haschild())
		{
			// reset mouse cursor
			state = cursor_state{ cursor_action::select };
		}
	}

	//XXX - statusbar
	if(state.type == "")
		_sb->set("");
	else
		_sb->set("Add control: " + state.type);

	_cursor_state = state;
}


void guimanager::new_project(const std::string& type, const std::string& filename)
{
	if(!addmainctrl(type))
		return;

	enableGUI(true);
}


tree_node<control_obj>* guimanager::addmainctrl(const std::string& type, const std::string& name)
{
	control_obj ctrl;

	if(type == CTRL_FORM)
		ctrl = control_obj(new ctrls::form(*_main_wd, name.empty() ? _name_mgr.add_numbered(CTRL_FORM) : name, _deserializing ? false : true));
	else if(type == CTRL_PANEL)
		ctrl = control_obj(new ctrls::panel(*_main_wd, name.empty() ? _name_mgr.add_numbered(CTRL_PANEL) : name, true, _deserializing ? false : true));
	else
		return 0;


	ctrl->nanawdg->bgcolor(nana::API::bgcolor(nana::API::get_parent_window(*_main_wd)));
	_main_wd->add(*ctrl->nanawdg);


	// events
	control_obj_ptr pctrl = ctrl;
	ctrl->nanawdg->events().mouse_enter([this, pctrl]()
	{
		if(!pctrl.lock()->highlighted() && cursor().action != cursor_action::select)
		{
			_insert_pos = insert_position::into;

			pctrl.lock()->set_highlight();
			nana::API::refresh_window(*pctrl.lock()->nanawdg);
		}
	});

	ctrl->nanawdg->events().mouse_leave([this, pctrl]()
	{
		if(pctrl.lock()->highlighted())
		{
			_insert_pos = insert_position::into;

			pctrl.lock()->reset_highlight();
			nana::API::refresh_window(*pctrl.lock()->nanawdg);
		}
	});

	// mouse click
	ctrl->nanawdg->events().mouse_down(nana::menu_popuper(_ctxmenu));
	ctrl->nanawdg->events().mouse_down.connect_front([this, pctrl](const nana::arg_mouse& arg)
	{
		if(arg.left_button)
			left_click_ctrl(pctrl.lock());
		else if(arg.right_button)
		{
			if(!right_click_ctrl(pctrl.lock()))
				arg.stop_propagation();
		}
	});

	nana::drawing dw{ *ctrl->nanawdg };
	dw.draw([this, pctrl](nana::paint::graphics& graph)
	{
		if(pctrl.lock()->selected())
			draw_selection(graph);

		if(pctrl.lock()->highlighted())
			draw_highlight(graph, insert_position::into);
	});


	return _registerobject(ctrl, {});
}


control_obj guimanager::_create_ctrl(control_obj parent, const std::string& type, const std::string& name)
{
	if(type == CTRL_LAYOUT)
		return control_obj(new ctrls::layout(*parent->nanawdg, name));
	else if(type == CTRL_PANEL)
		return control_obj(new ctrls::panel(*parent->nanawdg, name));
	else if(type == CTRL_GROUP)
		return control_obj(new ctrls::group(*parent->nanawdg, name));
	else if(type == CTRL_BUTTON)
		return control_obj(new ctrls::button(*parent->nanawdg, name));
	else if(type == CTRL_LABEL)
		return control_obj(new ctrls::label(*parent->nanawdg, name));
	else if(type == CTRL_TEXTBOX)
		return control_obj(new ctrls::textbox(*parent->nanawdg, name));
	else if(type == CTRL_COMBOX)
		return control_obj(new ctrls::combox(*parent->nanawdg, name));
	else if(type == CTRL_SPINBOX)
		return control_obj(new ctrls::spinbox(*parent->nanawdg, name));
	else if(type == CTRL_LISTBOX)
		return control_obj(new ctrls::listbox(*parent->nanawdg, name));
	else if(type == CTRL_CHECKBOX)
		return control_obj(new ctrls::checkbox(*parent->nanawdg, name));
	else if(type == CTRL_DATECHOOSER)
		return control_obj(new ctrls::date_chooser(*parent->nanawdg, name));
	else if(type == CTRL_TOOLBAR)
		return control_obj(new ctrls::toolbar(*parent->nanawdg, name));
	else if(type == CTRL_CATEGORIZE)
		return control_obj(new ctrls::categorize(*parent->nanawdg, name));
	else if(type == CTRL_MENUBAR)
		return control_obj(new ctrls::menubar(*parent->nanawdg, name));
	else if(type == CTRL_PICTURE)
		return control_obj(new ctrls::picture(*parent->nanawdg, name));
	else if(type == CTRL_PROGRESS)
		return control_obj(new ctrls::progress(*parent->nanawdg, name));
	else if(type == CTRL_SLIDER)
		return control_obj(new ctrls::slider(*parent->nanawdg, name));
	else if(type == CTRL_TABBAR)
		return control_obj(new ctrls::tabbar(*parent->nanawdg, name));
	else if(type == CTRL_TREEBOX)
		return control_obj(new ctrls::treebox(*parent->nanawdg, name));
	
	return 0;
}


tree_node<control_obj>* guimanager::addcommonctrl(add_position add_pos, const std::string& type, const std::string& name)
{
	control_obj parent_ = (add_pos.pos == insert_position::into) ? add_pos.ctrl->value : add_pos.ctrl->owner->value;

	std::string name_ = name.empty() ? _name_mgr.add_numbered(type) : name;

	// nana::widget + properties
	control_obj ctrl = _create_ctrl(parent_, type, name_);
	if(ctrl == 0)
		return 0;


	// append/insert
	if(add_pos.pos == insert_position::into)
		parent_->append(*ctrl->nanawdg);
	else
		parent_->insert(*add_pos.ctrl->value->nanawdg, *ctrl->nanawdg, (add_pos.pos == insert_position::after) ? true : false);


	// events
	control_obj_ptr pctrl = ctrl;
	control_obj_ptr pparent = parent_;

	// mouse enter
	if(type == CTRL_LAYOUT || type == CTRL_PANEL || type == CTRL_GROUP)
	{
		ctrl->nanawdg->events().mouse_enter([this, pctrl]()
		{
			if(!pctrl.lock()->highlighted() && cursor().action != cursor_action::select)
			{
				_insert_pos = insert_position::into;

				pctrl.lock()->set_highlight();
				nana::API::refresh_window(*pctrl.lock()->nanawdg);
			}
		});
	}
	else
	{
		ctrl->nanawdg->events().mouse_enter([this, pctrl, pparent]()
		{
			if(!pctrl.lock()->highlighted() && cursor().action != cursor_action::select)
			{
				_insert_pos = insert_position::into;

				pctrl.lock()->set_highlight();
				pparent.lock()->set_highlight();
				nana::API::refresh_window(*pctrl.lock()->nanawdg);
				nana::API::refresh_window(*pparent.lock()->nanawdg);
			}
		});
	}

	// mouse leave
	if(type == CTRL_LAYOUT || type == CTRL_PANEL || type == CTRL_GROUP)
	{
		ctrl->nanawdg->events().mouse_leave([this, pctrl]()
		{
			if(pctrl.lock()->highlighted())
			{
				_insert_pos = insert_position::into;

				pctrl.lock()->reset_highlight();
				nana::API::refresh_window(*pctrl.lock()->nanawdg);
			}
		});
	}
	else
	{
		ctrl->nanawdg->events().mouse_leave([this, pctrl, pparent]()
		{
			if(pctrl.lock()->highlighted())
			{
				_insert_pos = insert_position::into;

				pctrl.lock()->reset_highlight();
				pparent.lock()->reset_highlight();
				nana::API::refresh_window(*pctrl.lock()->nanawdg);
				nana::API::refresh_window(*pparent.lock()->nanawdg);
			}
		});
	}

	// mouse move
	if(type == CTRL_LAYOUT || type == CTRL_PANEL || type == CTRL_GROUP)
	{
		// do nothing !
	}
	else
	{
		ctrl->nanawdg->events().mouse_move([this, pctrl](const nana::arg_mouse& arg)
		{
			if(pctrl.lock()->highlighted())
			{
				if(arg.pos.x > pctrl.lock()->nanawdg->size().width / 2)
				{
					if(_insert_pos != insert_position::after)
					{
						_insert_pos = insert_position::after;
						nana::API::refresh_window(*pctrl.lock()->nanawdg);
					}
				}
				else
				{
					if(_insert_pos != insert_position::before)
					{
						_insert_pos = insert_position::before;
						nana::API::refresh_window(*pctrl.lock()->nanawdg);
					}
				}
			}
		});
	}

	// mouse click
	ctrl->nanawdg->events().mouse_down(nana::menu_popuper(_ctxmenu));
	ctrl->nanawdg->events().mouse_down.connect_front([this, pctrl](const nana::arg_mouse& arg)
	{
		if(arg.left_button)
			left_click_ctrl(pctrl.lock());
		else if(arg.right_button)
		{
			if(!right_click_ctrl(pctrl.lock()))
				arg.stop_propagation();
		}
	});


	// drawing
	nana::drawing dw{ *ctrl->nanawdg }; 
	if(type == CTRL_LAYOUT || type == CTRL_PANEL || type == CTRL_GROUP)
	{
		dw.draw([this, pctrl](nana::paint::graphics& graph)
		{
			if(pctrl.lock()->selected())
				draw_selection(graph);

			if(pctrl.lock()->highlighted())
				draw_highlight(graph, insert_position::into);
		});
	}
	else
	{
		dw.draw([this, pctrl](nana::paint::graphics& graph)
		{
			if(pctrl.lock()->selected())
				draw_selection(graph);

			if(pctrl.lock()->highlighted() && _insert_pos != insert_position::into)
				draw_highlight(graph, _insert_pos);
		});
	}

	return _registerobject(ctrl, add_pos);
}


void guimanager::deleteselected()
{
	if(!_selected)
		return;

	// main widget cannot be removed
	if(_selected == _ctrls.get_root()->child)
		return;

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
		_main_wd->remove(*toremove->value->nanawdg);
	}

	_ctrls.recursive_backward(toremove, [this](tree_node<control_obj>* node) -> bool
	{
		if(node->owner)
		{
			control_obj parent_ = node->owner->value;
			if(parent_)
				parent_->remove(*node->value->nanawdg);
		}

		_ctrls.remove(node);

		return true;
	});


	// delete objectspanel item
	_update_op();

	// select parent
	if(parent)
	{
		nana::API::refresh_window(*parent->value->nanawdg);
		left_click_ctrl(parent->value);
	}
}


void guimanager::moveupselected()
{
	if(!_selected)
		return;

	// move one position up
	if(!_ctrls.move_before_sibling(_selected))
		return;

	// if here is possible to move up
	auto parent = _selected->owner->value;
	parent->moveup(*_selected->value->nanawdg);

	// reorder objectspanel item
	_update_op();
}


void guimanager::movedownselected()
{
	if(!_selected)
		return;

	// move one position down
	if(!_ctrls.move_after_sibling(_selected))
		return;

	// if here is possible to move down
	auto parent = _selected->owner->value;
	parent->movedown(*_selected->value->nanawdg);

	// reorder objectspanel item
	_update_op();
}


void guimanager::copyselected(bool cut)
{
	if(!_selected)
		return;

	// main widget cannot be cut or copied
	if(_selected == _ctrls.get_root()->child)
	{
		nana::msgbox m(_root_wd, CREATOR_NAME, nana::msgbox::ok);
		m.icon(m.icon_error);
		m << "Impossible to cut/copy the main widget!";
		m();
		return;
	}

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
	if(!_selected)
		return;

	auto prev_selected = _selected;

	auto type = prev_selected->value->properties.property("type").as_string();
	if(type == CTRL_LAYOUT || type == CTRL_PANEL || type == CTRL_GROUP || type == CTRL_FORM)
	{
		// do nothing !!!
	}
	else
	{
		_select_ctrl(_selected->owner);
		if(!_selected)
			return;
	}
	

	// read root node
	pugi::xml_node root = _cut_copy_doc.child(NODE_ROOT);
	if(root.empty())
		return; // nothing to paste


	// deserialize the XML structure and avoid window update
	lock_guard des_lock(&_deserializing, true);
	_op->emit_events(false);
	_op->auto_draw(false);
		
	_deserialize(_selected, &root);
	
	_op->auto_draw(true);
	_op->emit_events(true);
	_update_op();


	if(!_copied)
		_cut_copy_doc.reset(); // cut items can be paste only once


	// select previous ctrl
	left_click_ctrl(prev_selected->value);
}


void guimanager::left_click_ctrl(control_obj ctrl)
{
	// search control
	tree_node<control_obj>*	_ctrl_node{ 0 };

	_ctrls.for_each([this, &ctrl, &_ctrl_node](tree_node<control_obj>* node) -> bool
	{
		if(node->value == ctrl)
		{
			_ctrl_node = node;
			return false;
		}

		return true;
	});

	if(!_ctrl_node)
		return;


	// select
	//---------------------
	if(cursor().action == cursor_action::select)
	{
		_select_ctrl(_ctrl_node);

		// select objectspanel item
		_op->select(ctrl->properties.property("name").as_string());

		// set properties panel
		_pp->set(&ctrl->properties);
		return;
	}


	// add
	//---------------------
	// deselect previous ctrl
	_ap->deselect();

	// reset ctrl highlight
	ctrl->reset_highlight();
	nana::API::refresh_window(*ctrl->nanawdg);

	// reset parent ctrl highlight
	auto _ctrl_node_parent = _ctrl_node->owner;
	if(_ctrl_node_parent)
	{
		if(_ctrl_node_parent->value)
		{
			_ctrl_node_parent->value->reset_highlight();
			nana::API::refresh_window(*_ctrl_node_parent->value->nanawdg);
		}
	}

	// add ctrl
	add_position add_pos;
	add_pos.ctrl = _ctrl_node;
	add_pos.pos = _insert_pos;

	addcommonctrl(add_pos, cursor().type);
}


bool guimanager::right_click_ctrl(control_obj ctrl)
{
	// search control
	tree_node<control_obj>*	_ctrl_node{ 0 };

	_ctrls.for_each([this, &ctrl, &_ctrl_node](tree_node<control_obj>* node) -> bool
	{
		if(node->value == ctrl)
		{
			_ctrl_node = node;
			return false;
		}

		return true;
	});

	if(!_ctrl_node)
		return false;


	// reset add action
	//---------------------
	if(cursor().action == cursor_action::add)
	{
		// deselect previous ctrl
		_ap->deselect();

		// reset ctrl highlight
		ctrl->reset_highlight();
		nana::API::refresh_window(*ctrl->nanawdg);

		// reset parent ctrl highlight
		auto _ctrl_node_parent = _ctrl_node->owner;
		if(_ctrl_node_parent)
		{
			if(_ctrl_node_parent->value)
			{
				_ctrl_node_parent->value->reset_highlight();
				nana::API::refresh_window(*_ctrl_node_parent->value->nanawdg);
			}
		}

		// reset mouse cursor
		cursor(cursor_state{ cursor_action::select });
		return false;
	}

	return true;
}


void guimanager::clickobjectspanel(const std::string& name)
{
	_ctrls.for_each([this, name](tree_node<control_obj>* node) -> bool
	{
		if(node->value->properties.property("name").as_string() == name)
		{
			_select_ctrl(node);

			// set properties panel
			_pp->set(&node->value->properties);

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

		for(size_t i = 0; i < node->value->properties.count(); ++i)
			xml_child.append_attribute(node->value->properties[i].name().c_str()) = node->value->properties[i].as_string().c_str();
	}

	if(node->child)
		_serialize(node->child, &xml_child);

	if(node->next && !children_only)
		_serialize(node->next, xml_parent);
}


bool guimanager::deserialize(pugi::xml_node* xml_parent)
{
	lock_guard des_lock(&_deserializing, true);
	_op->emit_events(false);
	_op->auto_draw(false);

	bool ret_val = _deserialize(0, xml_parent);

	_op->auto_draw(true);
	_op->emit_events(true);
	_update_op();

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

bool guimanager::_deserialize(tree_node<control_obj>* parent, pugi::xml_node* xml_parent)
{
	//ATTENTION !!! No check on deserialize

	// read children
	for(pugi::xml_node xml_node = xml_parent->first_child(); xml_node; xml_node = xml_node.next_sibling())
	{
		tree_node<control_obj>* node = 0;

		std::string node_name = xml_node.name();
		std::string ctrl_name = xml_node.attribute("name").as_string();

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
			add_position add_pos;
			add_pos.ctrl = parent;
			add_pos.pos = insert_position::into;
			node = addcommonctrl(add_pos, node_name, ctrl_name);
		}

		if(node == 0)
		{
			std::cout << "UNKNOWN NODE: " << xml_node.name() << std::endl;
			continue;
		}


		// deserialize properties
		_deserializeproperties(&node->value->properties, &xml_node);

		// align control name
		if(ctrl_name_changed)
			node->value->properties.property("name") = ctrl_name;

		// update nana::widget
		_updatectrl(node);


		// deserialize children
		if(!_deserialize(node, &xml_node))
			return false;
	}

	enableGUI(true);
	return true;
}


tree_node<control_obj>* guimanager::_registerobject(control_obj ctrl, add_position add_pos)
{
	tree_node<control_obj>* child = 0;

	// append/insert to controls tree
	if(add_pos.pos == insert_position::into)
		child = _ctrls.append(add_pos.ctrl, ctrl);
	else if(add_pos.pos == insert_position::after)
		child = _ctrls.insert_after(add_pos.ctrl, ctrl);
	else
		child = _ctrls.insert_before(add_pos.ctrl, ctrl);


	_updatectrl(child);


	// select new control
	_select_ctrl(child);

	if(!_deserializing)
	{
		_update_op();

		// set properties panel
		_pp->set(&ctrl->properties);

		// set focus to new object
		ctrl->nanawdg->focus();

		// reset mouse cursor
		cursor(cursor_state{ cursor_action::select });
	}

	return _selected;
}


void guimanager::_deserializeproperties(ctrls::properties_collection* properties, pugi::xml_node* xml_node)
{
	for(auto i = xml_node->attributes_begin(); i != xml_node->attributes_end(); ++i)
	{
		std::string name(i->name());
		if(name.find("item") == 0)
			properties->append(name) = i->value();
		else
			properties->property(name) = i->value();
	}
}


bool guimanager::_updatectrlname(ctrls::properties_collection* properties, const std::string& new_name)
{
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

	// update children ctrls
	if(node->child && update_children)
		_updatechildrenctrls(node);

	// update parent ctrl
	if(node->owner && update_owner)
		_updateparentctrl(node);
}


void guimanager::_updateparentctrl(tree_node<control_obj>* node)
{
	auto ctrl = node->value;
	if(ctrl->properties.property("mainclass").as_bool())
		return;

	if(!node->owner)
		return;

	std::string weight = ctrl->properties.property("weight").as_string();
	if(weight[0] == '-')
		weight = "";

	std::string margin = ctrl->properties.property("margin").as_string();

	auto parent = node->owner->value;
	parent->updatefield(*ctrl->nanawdg, weight, margin);
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

	if(_selected)
	{
		if(_selected->value)
		{
			_selected->value->reset_select();
			nana::API::refresh_window(*_selected->value->nanawdg);
		}
	}

	_selected = to_select;

	if(_selected)
	{
		if(_selected->value)
		{
			_selected->value->set_select();
			nana::API::refresh_window(*_selected->value->nanawdg);
		}
	}
}

