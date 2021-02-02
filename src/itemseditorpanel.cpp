/*
 *		nana::itemseditorpanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "itemseditorpanel.h"
#include "lock_guard.h"
#include "propertygrid_helper.h"
#include "ctrls/toolbar.h"
#include "ctrls/combox.h"
#include "ctrls/listbox.h"
#include "ctrls/tabbar.h"
#include "ctrls/menubar.h"
#include "ctrls/field.h" // collection_collapse


#define ITEM_NODE_NAME		"n"
#define ITEM_TXT_SEPARATOR	"-----"
#define ITEM_TXT_GO_RIGHT	">>>   Go right   >>>"



void itemseditorpanel::set(ctrls::properties_collection* properties, tree<ctrls::properties_collection>* items)
{
	if(_properties == properties)
		return;

	_properties = properties;

	// check if ctrl has items
	_items = 0;
	_selected = 0;
	_type = ctrls::pg_type::empty;
	if(_properties)
	{
		for(size_t i = 0; i < _properties->count(); ++i)
		{
			auto type = (*_properties)[i].type();
			if(type == ctrls::pg_type::collection_toolbar || type == ctrls::pg_type::collection_combox
				|| type == ctrls::pg_type::collection_listbox || type == ctrls::pg_type::collection_tabbar
				|| type == ctrls::pg_type::collection_menubar || type == ctrls::pg_type::collection_collapse)
			{
				_items = items;
				_type = type;
				_set_toolbar();
				break;
			}
		}
	}

	_update_items_tree();

	_enable();

	// select first item
	if(!_roots.empty())
		_roots[0].select(true);
	else
	{
		//_selected = _items->end();
		_update_selected();
	}
}


void itemseditorpanel::enable(bool state)
{
	_enable_state = state;
	_enable();
}


void itemseditorpanel::_init_ctrls()
{
	_update_selected();

	//------------------
	// events
	//------------------
	propgrid.events().property_changed([this](const nana::arg_propertygrid& arg)
		{
			if(_grid_setup)
				return;

			auto cat = propgrid.at(arg.item.pos().cat);

			for(size_t i = 0; i < _selected->value.count(); ++i)
			{
				auto pi = _selected->value[i];
				if(arg.item.label() == pi.label() && cat.text() == pi.category())
				{
					// check text
					if(arg.item.label() == "Text")
					{
						if(arg.item.value().empty())
						{
							// if empty restore old value
							arg.item.value(pi.value());
							return;
						}

						_update_text(*items_tree.selected(), arg.item.value());
					}
					// check image
					else if(arg.item.label() == "Image")
					{
						_update_image(*items_tree.selected(), arg.item.value());
					}

					pi.value(arg.item.value());

					// look for properties bonds - DISABLED in ITEMS EDITOR
					//propertygrid_helper::enabled_bonds(&propgrid, _selected, pi.name(), pi.as_bool());

					if(_property_changed_f)
						_property_changed_f();
					break;
				}
			}
		});

	// select item
	items_tree.events().selected([this](const nana::arg_treebox& arg)
		{
			if(_grid_setup)
				return;

			if(arg.operated)
				_select_item(arg.item.key());
		});
}


void itemseditorpanel::_set_toolbar()
{
	toolbar.clear();

	// Add ...
	auto ip = toolbar.append(nana::toolbar::tools::dropdown, "Add", nana::paint::image("icons/add.png")).textout(true);
	if(_type == ctrls::pg_type::collection_toolbar)
	{
		ip.dropdown_append("button", nana::paint::image("icons/item_generic.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_add_item_after_selected("button");
			});
		ip.dropdown_append("toggle", nana::paint::image("icons/checkbox.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_add_item_after_selected("toggle");
			});
		ip.dropdown_append("dropdown", nana::paint::image("icons/combox.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_add_item_after_selected("dropdown");
			});
		ip.dropdown_append("separator", nana::paint::image("icons/splitter.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_add_item_after_selected("separator");
			});
		ip.dropdown_append("go right", nana::paint::image("icons/goright.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_add_item_after_selected("go_right");
			});
	}
	else if(_type == ctrls::pg_type::collection_combox)
	{
		ip.dropdown_append("option", nana::paint::image("icons/item_generic.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_add_item_after_selected();
			});
	}
	else if(_type == ctrls::pg_type::collection_listbox)
	{
		ip.dropdown_append("column", nana::paint::image("icons/item_generic.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_add_item_after_selected();
			});
	}
	else if(_type == ctrls::pg_type::collection_tabbar)
	{
		ip.dropdown_append("tab", nana::paint::image("icons/item_generic.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_add_item_after_selected();
			});
	}
	else if(_type == ctrls::pg_type::collection_menubar)
	{
		ip.dropdown_append("menu", nana::paint::image("icons/item_generic.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_add_item_after_selected("item");
			});
	}
	else if(_type == ctrls::pg_type::collection_collapse)
	{
		ip.dropdown_append("collapse", nana::paint::image("icons/item_generic.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_add_item_after_selected();
			});
	}

	toolbar.append_separator();
	toolbar.append(nana::toolbar::tools::button, "Delete", nana::paint::image("icons/delete.png"),
		[this](const nana::toolbar::item_proxy& ip) {
			_delete_selected();
		});
	toolbar.append_separator();
	toolbar.append(nana::toolbar::tools::button, "Move up", nana::paint::image("icons/up.png"),
		[this](const nana::toolbar::item_proxy& ip) {
			_move_up_selected();
		});
	toolbar.append(nana::toolbar::tools::button, "Move down", nana::paint::image("icons/down.png"),
		[this](const nana::toolbar::item_proxy& ip) {
			_move_down_selected();
		});

	if(_type == ctrls::pg_type::collection_toolbar)
	{
		toolbar.go_right();
		toolbar.append(nana::toolbar::tools::button, "Add dropdown item", nana::paint::image("icons/menu_item_add.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_toolbar_add_dropdown_item_after_selected();
			}).enable(false);
	}
	else if(_type == ctrls::pg_type::collection_menubar)
	{
		toolbar.go_right();
		auto ip = toolbar.append(nana::toolbar::tools::dropdown, "Menu", nana::paint::image("icons/menu_item_add.png")).textout(true);
		ip.dropdown_append("item", nana::paint::image("icons/menu_item.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_menubar_add_menu_item_after_selected("menu");
			});
		ip.dropdown_append("separator", nana::paint::image("icons/menu_separator.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_menubar_add_menu_item_after_selected("separator");
			});
		ip.dropdown_append("submenu", nana::paint::image("icons/menu.png"),
			[this](const nana::toolbar::item_proxy& ip) {
				_menubar_add_menu_item_after_selected("submenu");
			});
	}
}


void itemseditorpanel::_enable_toolbar_specific()
{
	if(!_selected)
	{
		if(_type == ctrls::pg_type::collection_toolbar)
		{
			toolbar.enable(6, false);
		}
		else if(_type == ctrls::pg_type::collection_menubar)
		{
			toolbar.enable(6, false);
		}
		return;
	}

	auto type = _selected->value.property("type").as_string();
	if(_type == ctrls::pg_type::collection_toolbar)
	{
		if(type == "dropdown" || type == "dropdown-item")
		{
			toolbar.enable(0, type == "dropdown" ? true : false);
			toolbar.enable(6, true);
		}
		else
		{
			toolbar.enable(0, true);
			toolbar.enable(6, false);
		}
	}
	else if(_type == ctrls::pg_type::collection_menubar)
	{
		toolbar.enable(0, type == "item" ? true : false);
		toolbar.enable(6, true);
	}
}


void itemseditorpanel::_add_item_after_selected(const std::string& type)
{
	if(type == "go_right" && _go_right)
	{
		nana::msgbox m(*this, CREATOR_NAME, nana::msgbox::ok);
		m.icon(m.icon_information);
		m << "\"Go right\" is already present !";
		m();
		return;
	}

	if(!_selected)
		_selected = _items->append(ctrls::properties_collection{});
	else
		_selected = _items->insert_after(_selected, ctrls::properties_collection{});

	// init item properties
	if(_type == ctrls::pg_type::collection_toolbar)
		ctrls::toolbar::init_item(_selected->value, type);
	else if(_type == ctrls::pg_type::collection_combox)
		ctrls::combox::init_item(_selected->value);
	else if(_type == ctrls::pg_type::collection_listbox)
		ctrls::listbox::init_item(_selected->value);
	else if(_type == ctrls::pg_type::collection_tabbar)
		ctrls::tabbar::init_item(_selected->value);
	else if(_type == ctrls::pg_type::collection_menubar)
		ctrls::menubar::init_item(_selected->value, type);
	else if(_type == ctrls::pg_type::collection_collapse)
			ctrls::field::init_item(_selected->value);

	_selected->value.property("key") = _node_mgr.add_numbered(ITEM_NODE_NAME);

	_push_undo(undoredo::action::add);

	_update_items_tree();
	_select_item_tree(_selected->value.property("key").as_string());

	if(_property_changed_f)
		_property_changed_f();
}


void itemseditorpanel::_delete_selected(bool push_undo)
{
	if(!_selected)
		return;

	if(push_undo)
	{
		nana::msgbox m(*this, CREATOR_NAME, nana::msgbox::yes_no);
		m.icon(m.icon_question);
		m << "Delete selected item?";
		if(m() != nana::msgbox::pick_yes)
			return;

		_push_undo(undoredo::action::remove);
	}

	// node to select after deletion
	auto pnext = _selected->next;
	if(!pnext && _selected->owner)
	{
		pnext = _selected->owner->child;
		if(pnext != _selected)
		{
			while(pnext->next != _selected)
				pnext = pnext->next;
		}
		else
		{
			pnext = _selected->owner;
			if(pnext == _items->get_root())
				pnext = nullptr;
		}
	}

	_items->remove(_selected);
	_selected = pnext;

	_update_items_tree();

	if(_selected)
		_select_item_tree(_selected->value.property("key").as_string());
	else
		_update_selected();

	if(_property_changed_f)
		_property_changed_f();
}


void itemseditorpanel::_move_up_selected(bool push_undo)
{
	if(!_items->move_before_sibling(_selected))
		return;

	if(push_undo)
		_push_undo(undoredo::action::move_up);

	_update_items_tree();
	_select_item_tree(_selected->value.property("key").as_string());

	if(_property_changed_f)
		_property_changed_f();
}


void itemseditorpanel::_move_down_selected(bool push_undo)
{
	if(!_items->move_after_sibling(_selected))
		return;

	if(push_undo)
		_push_undo(undoredo::action::move_down);

	_update_items_tree();
	_select_item_tree(_selected->value.property("key").as_string());

	if(_property_changed_f)
		_property_changed_f();
}


void itemseditorpanel::_toolbar_add_dropdown_item_after_selected()
{
	if(!_selected)
		return;

	std::string owner_key;

	if(_selected->level() == 0)
	{
		owner_key = _selected->value.property("key").as_string();
		_selected = _selected->append(ctrls::properties_collection{});
	}
	else
	{
		owner_key = _selected->value.property("owner").as_string();
		_selected = _selected->insert_after(ctrls::properties_collection{});
	}

	// init item properties
	ctrls::toolbar::init_item(_selected->value, "dropdown-item");
	_selected->value.property("key") = _node_mgr.add_numbered(ITEM_NODE_NAME);
	_selected->value.property("owner") = owner_key;

	_push_undo(undoredo::action::add);

	_update_items_tree();
	_select_item_tree(_selected->value.property("key").as_string());

	if(_property_changed_f)
		_property_changed_f();
}


void itemseditorpanel::_menubar_add_menu_item_after_selected(const std::string& type)
{
	if(!_selected)
		return;

	std::string owner_key;

	if(_selected->level() == 0 || type == "submenu")
	{
		if(_selected->value.property("type").as_string() == "separator")
			return; // separator cannot have children

		owner_key = _selected->value.property("key").as_string();
		_selected = _selected->append(ctrls::properties_collection{});
	}
	else
	{
		owner_key = _selected->value.property("owner").as_string();
		_selected = _selected->insert_after(ctrls::properties_collection{});
	}

	// init item properties
	ctrls::menubar::init_item(_selected->value, type == "submenu" ? "menu" : type);
	_selected->value.property("key") = _node_mgr.add_numbered(ITEM_NODE_NAME);
	_selected->value.property("owner") = owner_key;

	_push_undo(undoredo::action::add);

	_update_items_tree();
	_select_item_tree(_selected->value.property("key").as_string());

	if(_property_changed_f)
		_property_changed_f();
}


nana::treebox::item_proxy itemseditorpanel::_find_items_tree(const std::string& key)
{
	nana::treebox::item_proxy ip;
	for(auto& root : _roots)
	{
		if(!ip.empty())
			break;

		root.visit_recursively([&key, &ip](nana::treebox::item_proxy item) -> bool
			{
				if(key == item.key())
				{
					ip = item;
					return false;
				}
				return true;
			});
	}

	return ip;
}


void itemseditorpanel::_select_item_tree(const std::string& key)
{
	auto ip = _find_items_tree(key);
	if(!ip.empty())
	{
		ip.select(true);
		items_tree.scroll_into_view(ip);
	}
}


void itemseditorpanel::_select_item(const std::string& key)
{
	_selected = nullptr;
	if(!key.empty())
	{
		_items->for_each([this, &key](tree_node<ctrls::properties_collection>* node) -> bool
			{
				if(node->value.property("key").as_string() == key)
				{
					_selected = node;
					return false;
				}
				return true;
			});
	}

	_update_selected();
}


void itemseditorpanel::_update_items_tree()
{
	lock_guard grid_change(&_grid_setup, true);

	items_tree.auto_draw(false);

	for(auto r : _roots)
		items_tree.erase(r);
	_roots.clear();

	_node_mgr.clear();
	_go_right = false;

	if(_items)
	{
		// populate the items_tree
		_items->for_each([this](tree_node<ctrls::properties_collection>* node) -> bool
			{
				_node_mgr.add(node->value.property("key").as_string());

				std::string text;
				std::string type = node->value.property("type").as_string();
				if(type == "go_right")
				{
					text = ITEM_TXT_GO_RIGHT;
					_go_right = true;
				}
				else if(type == "separator")
					text = ITEM_TXT_SEPARATOR;
				else if(type == "collapse")
					text = "collapse";
				else
					text = node->value.property("text").as_string();


				if(node->value.property("owner").as_string().empty())
				{
					auto ip = items_tree.insert(node->value.property("key").as_string(), text);
					_roots.push_back(ip);

					_update_image(ip, node->value.property("image").as_string());
				}
				else
				{
					for(auto& root : _roots)
					{
						auto ip_owner = _find_items_tree(node->value.property("owner").as_string());
						if(!ip_owner.empty())
						{
							auto ip = ip_owner.append(node->value.property("key").as_string(), text);
							ip_owner.expand(true);

							_update_image(ip, node->value.property("image").as_string());
						}
					}
				}

				return true;
			});
	}

	items_tree.auto_draw(true);
}


void itemseditorpanel::_update_text(nana::drawerbase::treebox::item_proxy& ip, const std::string& text)
{
	if(ip.empty())
		return;

	ip.text(text);
	nana::API::refresh_window(items_tree);
}


void itemseditorpanel::_update_image(nana::drawerbase::treebox::item_proxy& ip, const std::string& filename)
{
	if(ip.empty())
		return;

	static std::hash<std::string> h;
	auto img_id = h(filename.empty() ? "icons/empty.png" : filename);

	auto& img = items_tree.icon(std::to_string(img_id));
	if(img.normal.empty())
		img.normal.open(filename.empty() ? "icons/empty.png" : filename);

	ip.icon(std::to_string(img_id));
	nana::API::refresh_window(items_tree);
}


void itemseditorpanel::_update_selected()
{
	propgrid.enabled(false);
	lock_guard grid_change(&_grid_setup, true);

	propgrid.erase();
	// enable/disable specific toolbar tools based on current selection
	_enable_toolbar_specific();

	if(!_selected)
		return;

	propertygrid_helper::append(&propgrid, &_selected->value);

	propgrid.enabled(true);
}


void itemseditorpanel::_enable()
{
	bool s = _enable_state && _items;

	for(size_t i = 0; i < toolbar.count(); ++i)
		toolbar.enable(i, s);
}


	/*---------------*/
	/*   UNDO/REDO   */
	/*---------------*/
void itemseditorpanel::_serialize_items(pugi::xml_node* xml_node)
{
	_items->for_each([&xml_node](tree_node<ctrls::properties_collection>* node) -> bool
		{
			auto xml_item = xml_node->append_child(NODE_ITEM);

			for(size_t i = 0; i < node->value.count(); ++i)
				xml_item.append_attribute(node->value[i].name().c_str()) = node->value[i].as_string().c_str();
			return true;
		});
}


void itemseditorpanel::_deserialize_items(pugi::xml_node* xml_parent)
{
	// read children
	for(pugi::xml_node xml_node = xml_parent->first_child(); xml_node; xml_node = xml_node.next_sibling())
	{
		ctrls::properties_collection item;

		// init item properties
		if(_type == ctrls::pg_type::collection_toolbar)
			ctrls::toolbar::init_item(item, xml_node.attribute("type").as_string());
		else if(_type == ctrls::pg_type::collection_combox)
			ctrls::combox::init_item(item);
		else if(_type == ctrls::pg_type::collection_listbox)
			ctrls::listbox::init_item(item);
		else if(_type == ctrls::pg_type::collection_tabbar)
			ctrls::tabbar::init_item(item);
		else if(_type == ctrls::pg_type::collection_menubar)
			ctrls::menubar::init_item(item, xml_node.attribute("type").as_string());
		else if(_type == ctrls::pg_type::collection_collapse)
			ctrls::field::init_item(item);

		// deserialize attributes
		for(auto i = xml_node.attributes_begin(); i != xml_node.attributes_end(); ++i)
			item.property(i->name()).value(i->value());

		if(item.property("owner").as_string().empty())
		{
			_items->append(item);
		}
		else
		{
			_items->for_each([&item](tree_node<ctrls::properties_collection>* node) -> bool
				{
					if(item.property("owner").as_string() == node->value.property("key").as_string())
					{
						node->append(item);
						return false;
					}
					return true;
				});
		}
	}

	_update_items_tree();
}


void itemseditorpanel::_push_undo(undoredo::action action, const std::string& value)
{
	if(!_push_undo_f)
		return;

	undoredo::state s;
	s.item_action = action;
	s.item_name = _selected->value.property("key").as_string();

	if(action == undoredo::action::remove)
	{
		// save items
		auto unode = s.snapshot.append_child(NODE_ROOT);
		_serialize_items(&unode);
	}

	_push_undo_f(&s);
}


void itemseditorpanel::undo(undoredo::state* ustate, undoredo::state* rstate)
{
	if(ustate->item_action == undoredo::action::add)
	{
		// save items
		auto rnode = rstate->snapshot.append_child(NODE_ROOT);
		_serialize_items(&rnode);

		_select_item(ustate->item_name);
		_delete_selected(false);
	}
	else if(ustate->item_action == undoredo::action::remove)
	{
		// restore items
		_items->clear();
		auto xml_node = ustate->snapshot.first_child();
		_deserialize_items(&xml_node);
		_select_item(ustate->item_name);
		_select_item_tree(ustate->item_name);
	}
	else if(ustate->item_action == undoredo::action::move_up)
	{
		_select_item(ustate->item_name);
		_move_down_selected(false);
	}
	else if(ustate->item_action == undoredo::action::move_down)
	{
		_select_item(ustate->item_name);
		_move_up_selected(false);
	}
}


void itemseditorpanel::redo(undoredo::state* rstate, undoredo::state* ustate)
{
	if(rstate->item_action == undoredo::action::add)
	{
		// restore items
		_items->clear();
		auto xml_node = rstate->snapshot.first_child();
		_deserialize_items(&xml_node);
		_select_item(rstate->item_name);
		_select_item_tree(rstate->item_name);
	}
	else if(rstate->item_action == undoredo::action::remove)
	{
		// save items
		auto unode = ustate->snapshot.append_child(NODE_ROOT);
		_serialize_items(&unode);

		_select_item(rstate->item_name);
		_delete_selected(false);
	}
	else if(rstate->item_action == undoredo::action::move_up)
	{
		_select_item(rstate->item_name);
		_move_up_selected(false);
	}
	else if(rstate->item_action == undoredo::action::move_down)
	{
		_select_item(rstate->item_name);
		_move_down_selected(false);
	}
}
