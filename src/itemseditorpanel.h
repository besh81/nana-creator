/*****************************************************
 *	C++ code generated with Nana Creator (0.31.0)
 *	GitHub repo: https://github.com/besh81/nana-creator
 *
 * PLEASE EDIT ONLY INSIDE THE TAGS:
 *		//<*tag
 *			user code
 *		//*>
*****************************************************/

#ifndef ITEMSEDITORPANEL_H
#define ITEMSEDITORPANEL_H

#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include "nana_extra/propertygrid.h"

//<*includes
#include "tree.h"
#include "ctrls/property.h"
#include "namemanager.h"
#include "undoredo.h"
//*>


class itemseditorpanel
	: public nana::panel<true>
{
public:
	itemseditorpanel() = default;

	itemseditorpanel(nana::window wd, const nana::rectangle& r = {}, bool visible = true)
		: nana::panel<true>(wd, r, visible)
	{
		this->create(wd, r, visible);
	}

	~itemseditorpanel()
	{
		//<*dtor

		//*>
	}

	bool create(nana::window wd, const nana::rectangle& r = {}, bool visible = true)
	{
		if(!nana::panel<true>::create(wd, r, visible))
			return false;

		init_();

		//<*ctor
		_init_ctrls();
		//*>

		return true;
	}


private:
	void init_()
	{
		place_.bind(*this);
		place_.div("vert margin=1 <vert weight=32 arrange=[32] field1><vert gap=2 field2>|<gap=2 field3>");
		bgcolor(nana::color(64,86,141));
		// toolbar
		toolbar.create(*this);
		place_["field1"] << toolbar;
		toolbar.bgcolor(nana::color(204,213,240));
		toolbar.tools_height(22);
		toolbar.append(nana::toolbar::tools::dropdown, "Add", nana::paint::image("icons/add.png")).textout(true);
		toolbar.append_separator();
		toolbar.append(nana::toolbar::tools::button, "Delete", nana::paint::image("icons/delete.png"));
		toolbar.append_separator();
		toolbar.append(nana::toolbar::tools::button, "Move up", nana::paint::image("icons/up.png"));
		toolbar.append(nana::toolbar::tools::button, "Move down", nana::paint::image("icons/down.png"));
		// items_tree
		items_tree.create(*this);
		place_["field2"] << items_tree;
		items_tree.use_entire_line(true);
		// propgrid
		propgrid.create(*this);
		place_["field3"] << propgrid;

		place_.collocate();
	}


protected:
	nana::place place_;
	nana::toolbar toolbar;
	nana::treebox items_tree;
	nana::propertygrid propgrid;


	//<*declarations
public:
	void set(ctrls::properties_collection* properties, tree<ctrls::properties_collection>* items);

	void enable(bool state);

	void property_changed(std::function<void()> f)
	{
		_property_changed_f = f;
	}

	/*---------------*/
	/*   UNDO/REDO   */
	/*---------------*/
	void push_undo(std::function<void(undoredo::state* s)> f)
	{
		_push_undo_f = f;
	}
	void undo(undoredo::state* ustate, undoredo::state* rstate);
	void redo(undoredo::state* rstate, undoredo::state* ustate);


private:
	void _init_ctrls();
	void _set_toolbar();
	void _enable_toolbar_specific();

	void _add_item_after_selected(const std::string& type = "");
	void _delete_selected(bool push_undo = true);
	void _move_up_selected(bool push_undo = true);
	void _move_down_selected(bool push_undo = true);

	// ctrls specif
	void _toolbar_add_dropdown_item_after_selected();
	void _menubar_add_menu_item_after_selected(const std::string& type);

	void _update_items_tree();
	void _update_text(nana::drawerbase::treebox::item_proxy& ip, const std::string& text);
	void _update_image(nana::drawerbase::treebox::item_proxy& ip, const std::string& filename);
	void _update_selected();

	nana::treebox::item_proxy _find_items_tree(const std::string& key);
	void _select_item_tree(const std::string& key);
	void _select_item(const std::string& key);

	void _enable();
	bool _enable_state{ false };

	ctrls::properties_collection* _properties{ nullptr };
	tree<ctrls::properties_collection>* _items;
	tree_node<ctrls::properties_collection>* _selected{ nullptr };

	namemanager _node_mgr;
	std::vector<nana::treebox::item_proxy> _roots;
	ctrls::pg_type _type{ ctrls::pg_type::empty };

	bool _go_right{ false }; // used by toolbar type

	bool _grid_setup{ false };

	std::function<void()> _property_changed_f;


	/*---------------*/
	/*   UNDO/REDO   */
	/*---------------*/
	void _serialize_items(pugi::xml_node* xml_node);
	void _deserialize_items(pugi::xml_node* xml_parent);
	void _push_undo(undoredo::action action, const std::string& value = "");

	std::function<void(undoredo::state* s)> _push_undo_f;
	//*>
};

#endif //ITEMSEDITORPANEL_H

