/*****************************************************
 *	C++ code generated with Nana Creator (0.23.0)
 *	GitHub repo: https://github.com/besh81/nana-creator
 *
 * PLEASE EDIT ONLY INSIDE THE TAGS:
 *		//<*tag
 *			user code
 *		//*>
*****************************************************/

#ifndef ITEMS_DIALOG_H
#define ITEMS_DIALOG_H

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/button.hpp>

//<*includes
#include "ctrls/property.h"
#include "nana_extra/propertygrid.h"
#include "namemanager.h"
//*>


class items_dialog
	: public nana::form
{
public:
	items_dialog(nana::window wd, const ::nana::size& sz = {660, 480}, const nana::appearance& apr = {true, true, false, false, false, false, false})
		: nana::form(wd, sz, apr)
	{
		init_();

		//<*ctor
		init();
		//*>
	}

	~items_dialog()
	{
		//<*dtor

		//*>
	}


private:
	void init_()
	{
		_place.div("vert margin=5 <<vert margin=3 gap=2 arrange=[variable,30] left_lyt>|<margin=3 gap=2 right_lyt>><weight=40 margin=5 gap=3 arrange=[variable,90,90] footer_lyt>");
		caption("Form");
		// items_tree
		items_tree.create(*this);
		_place["left_lyt"] << items_tree;
		items_tree.use_entire_line(true);
		// toolbar
		toolbar.create(*this);
		_place["left_lyt"] << toolbar;
		toolbar.scale(21);
		toolbar.append("Add item", nana::paint::image("icons/item_add.png"));
		toolbar.append("Add separator", nana::paint::image("icons/sep_add.png"));
		toolbar.append("Delete", nana::paint::image("icons/item_delete.png"));
		toolbar.separate();
		toolbar.append("Move up", nana::paint::image("icons/up.png"));
		toolbar.append("Move down", nana::paint::image("icons/down.png"));
		// panel1
		panel1.create(*this);
		panel1.transparent(true);
		_place["footer_lyt"] << panel1;
		// ok_btn
		ok_btn.create(*this);
		_place["footer_lyt"] << ok_btn;
		ok_btn.caption("OK");
		// canc_btn
		canc_btn.create(*this);
		_place["footer_lyt"] << canc_btn;
		canc_btn.caption("Cancel");

		_place.collocate();
	}


protected:
	nana::place _place{ *this };
	nana::treebox items_tree;
	nana::toolbar toolbar;
	nana::panel<true> panel1;
	nana::button ok_btn;
	nana::button canc_btn;


	//<*declarations
public:
	items_dialog(nana::window wd, ctrls::pg_type ctrltype)
		: nana::form(wd, { 640, 480 }, { true, true, false, false, false, false, false }), _type(ctrltype)
	{
		init_();
		init();
	}
	ctrls::pg_type type()
	{
		return _type;
	}

	void set_items(const std::vector<ctrls::properties_collection>& items);
	std::vector<ctrls::properties_collection>& get_items();

	nana::msgbox::pick_t return_val() { return _retval; }


protected:
	void init();
	void tb_add_item_same_level_selected_item(bool separator = false);
	void tb_delete_selected_item();
	void tb_move_up_selected_item();
	void tb_move_down_selected_item();
	void tb_move_left_selected_item();
	void tb_move_right_selected_item();
	void tb_add_go_right();

	void update_selected();
	void update_text(nana::drawerbase::treebox::item_proxy& ip, const std::string& text);
	void update_image(nana::drawerbase::treebox::item_proxy& ip, const std::string& filename);

	ctrls::pg_type				_type;

	nana::treebox::item_proxy	_root;
	nana::msgbox::pick_t		_retval{ nana::msgbox::pick_cancel };

	namemanager					_node_mgr;

	nana::propertygrid			_propgrid{ *this };

	bool										_grid_setup{ false };
	std::vector<ctrls::properties_collection>	_items;
	ctrls::properties_collection*				_selected{ 0 };
	bool										_go_right{ false };

	void select_item(const std::string& key);
	void erase_item(const std::string& key);
	ctrls::properties_collection* find_item(const std::string& key);
	//*>
};

#endif //ITEMS_DIALOG_H

