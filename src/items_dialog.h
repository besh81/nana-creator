/*****************************************************
 *	C++ code generated with Nana Creator (0.8.0)
 *	GitHub repo: https://github.com/besh81/nana-creator
 *
 * PLEASE EDIT ONLY INSIDE THE TAGS:
 *		//<*tag
 *			user code
 *		//*>
*****************************************************/


#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/button.hpp>

//<*includes
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include "ctrls/property.h"
#include "namemanager.h"
//*>


class items_dialog
	: public nana::form
{
public:
	items_dialog(nana::window wd, const ::nana::size& sz = {640, 480}, const nana::appearance& apr = {true, true, false, false, false, false, false})
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
		enabled(true);
		caption("Form");
		// main_lyt
		main_lyt_place.bind(*this);
		main_lyt_place.div("vert <field1><weight=40 field2>");
		// body_lyt
		body_lyt_panel.create(*this);
		body_lyt_place.bind(body_lyt_panel);
		body_lyt_place.div("margin=5 <field1><field2>");
		main_lyt_place["field1"] << body_lyt_panel;
		// left_lyt
		left_lyt_panel.create(body_lyt_panel);
		left_lyt_place.bind(left_lyt_panel);
		left_lyt_place.div("vert <field1><weight=30 field2>");
		body_lyt_place["field1"] << left_lyt_panel;
		// items_tree
		items_tree.create(left_lyt_panel);
		left_lyt_place["field1"] << items_tree;
		items_tree.enabled(true);
		items_tree.bgcolor(nana::color(255,255,255));
		// toolbar
		toolbar.create(left_lyt_panel);
		left_lyt_place["field2"] << toolbar;
		toolbar.enabled(true);
		toolbar.scale(21);
		toolbar.append("Add item", nana::paint::image("icons/item_add.png"));
		toolbar.append("Delete item", nana::paint::image("icons/item_delete.png"));
		toolbar.separate();
		toolbar.append("Move up", nana::paint::image("icons/up.png"));
		toolbar.append("Move down", nana::paint::image("icons/down.png"));
		toolbar.separate();
		toolbar.append("Move left", nana::paint::image("icons/left.png"));
		toolbar.append("Move right", nana::paint::image("icons/right.png"));
		// prop_grp
		prop_grp.create(body_lyt_panel);
		body_lyt_place["field2"] << prop_grp;
		prop_grp.enabled(true);
		prop_grp.caption("Properties");
		prop_grp.div("field1");
		// footer_lyt
		footer_lyt_panel.create(*this);
		footer_lyt_place.bind(footer_lyt_panel);
		footer_lyt_place.div("margin=2 <field1><weight=90 margin=3 field2><weight=90 margin=3 field3>");
		main_lyt_place["field2"] << footer_lyt_panel;
		// spacer
		spacer_panel.create(footer_lyt_panel);
		spacer_place.bind(spacer_panel);
		spacer_place.div("margin=5 ");
		footer_lyt_place["field1"] << spacer_panel;
		// ok_btn
		ok_btn.create(footer_lyt_panel);
		footer_lyt_place["field2"] << ok_btn;
		ok_btn.enabled(true);
		ok_btn.caption("OK");
		ok_btn.transparent(false);
		// canc_btn
		canc_btn.create(footer_lyt_panel);
		footer_lyt_place["field3"] << canc_btn;
		canc_btn.enabled(true);
		canc_btn.caption("Cancel");
		canc_btn.transparent(false);

		main_lyt_place.collocate();
	}


protected:
	nana::place main_lyt_place;
	nana::panel<false> body_lyt_panel;
	nana::place body_lyt_place;
	nana::panel<false> left_lyt_panel;
	nana::place left_lyt_place;
	nana::treebox items_tree;
	nana::toolbar toolbar;
	nana::group prop_grp;
	nana::panel<false> footer_lyt_panel;
	nana::place footer_lyt_place;
	nana::panel<false> spacer_panel;
	nana::place spacer_place;
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

	void value(const std::string& items);
	std::string value();

	nana::msgbox::pick_t return_val() { return _retval; }


protected:
	void init();

	void select_item(const std::string& key);
	void erase_item(const std::string& key);

	void _update_selected();

	ctrls::pg_type				_type;

	nana::treebox::item_proxy	_root;
	nana::msgbox::pick_t		_retval;

	struct _data_struct
	{
		bool		separator{ false };
		std::string key;
		std::string owner;
		std::string text;
		std::string img;
		std::string width;
	};
	std::vector<_data_struct>	_data;
	_data_struct*				_selected{ 0 };

	namemanager					_node_mgr;


	//ctrls
	nana::place type_lyt_place;
	nana::checkbox normal_chk;
	nana::radio_group rg_type;
	nana::checkbox separator_chk;
	nana::label label1;
	nana::textbox item_txt;
	nana::label label2;
	nana::panel<false> img_lyt_panel;
	nana::place img_lyt_place;
	nana::textbox img_txt;
	nana::button img_btn;
	nana::label label3;
	nana::textbox width_txt;
	//*>
};

