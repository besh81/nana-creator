/*****************************************************
 *	C++ code generated with Nana Creator (0.7.0)
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
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/combox.hpp>

//<*includes
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
		toolbar.append("Add item", nana::paint::image("icons/item_add_dark.png"));
		toolbar.append("Delete item", nana::paint::image("icons/item_delete_dark.png"));
		toolbar.separate();
		toolbar.append("Move up", nana::paint::image("icons/up_dark.png"));
		toolbar.append("Move down", nana::paint::image("icons/down_dark.png"));
		// prop_grp
		prop_grp.create(body_lyt_panel);
		body_lyt_place["field2"] << prop_grp;
		prop_grp.enabled(true);
		prop_grp.caption("Properties");
		prop_grp.div("field1");
		// prop_lyt
		prop_lyt_panel.create(prop_grp);
		prop_lyt_place.bind(prop_lyt_panel);
		prop_lyt_place.div("vert margin=5 <weight=35 field1><weight=25 field2><weight=25 field3><weight=25 field4><weight=25 field5><margin=5 field6><margin=5 field7>");
		prop_grp["field1"] << prop_lyt_panel;
		// type_lyt
		type_lyt_panel.create(prop_lyt_panel);
		type_lyt_place.bind(type_lyt_panel);
		type_lyt_place.div("margin=5 <field1><field2>");
		prop_lyt_place["field1"] << type_lyt_panel;
		// normal_chk
		normal_chk.create(type_lyt_panel);
		type_lyt_place["field1"] << normal_chk;
		normal_chk.enabled(true);
		normal_chk.caption("Normal");
		normal_chk.check(true);
		normal_chk.radio(true);
		rg_type.add(normal_chk);
		// separator_chk
		separator_chk.create(type_lyt_panel);
		type_lyt_place["field2"] << separator_chk;
		separator_chk.enabled(true);
		separator_chk.caption("Separator");
		separator_chk.check(false);
		separator_chk.radio(true);
		rg_type.add(separator_chk);
		// label1
		label1.create(prop_lyt_panel);
		prop_lyt_place["field2"] << label1;
		label1.enabled(true);
		label1.caption("Text:");
		label1.format(false);
		label1.transparent(false);
		label1.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// item_txt
		item_txt.create(prop_lyt_panel);
		prop_lyt_place["field3"] << item_txt;
		item_txt.enabled(true);
		item_txt.bgcolor(nana::color(255,255,255));
		item_txt.caption("");
		item_txt.tip_string("");
		item_txt.editable(true);
		item_txt.line_wrapped(false);
		item_txt.multi_lines(false);
		// label2
		label2.create(prop_lyt_panel);
		prop_lyt_place["field4"] << label2;
		label2.enabled(true);
		label2.caption("Image:");
		label2.format(false);
		label2.transparent(false);
		label2.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// layout1
		layout1_panel.create(prop_lyt_panel);
		layout1_place.bind(layout1_panel);
		layout1_place.div("<field1><weight=30 field2>");
		prop_lyt_place["field5"] << layout1_panel;
		// img_txt
		img_txt.create(layout1_panel);
		layout1_place["field1"] << img_txt;
		img_txt.enabled(true);
		img_txt.bgcolor(nana::color(255,255,255));
		img_txt.caption("");
		img_txt.tip_string("");
		img_txt.editable(true);
		img_txt.line_wrapped(false);
		img_txt.multi_lines(false);
		// img_btn
		img_btn.create(layout1_panel);
		layout1_place["field2"] << img_btn;
		img_btn.enabled(true);
		img_btn.caption("...");
		img_btn.transparent(false);
		// combox1
		combox1.create(prop_lyt_panel);
		prop_lyt_place["field6"] << combox1;
		combox1.enabled(true);
		combox1.push_back("UNO ");
		combox1.push_back("due");
		combox1.push_back("Tre ");
		combox1.option(1);
		combox1.editable(false);
		// toolbar1
		toolbar1.create(prop_lyt_panel);
		prop_lyt_place["field7"] << toolbar1;
		toolbar1.enabled(true);
		toolbar1.scale(16);
		toolbar1.append("New item", nana::paint::image("icons/combox_dark.png"));
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
	nana::panel<false> prop_lyt_panel;
	nana::place prop_lyt_place;
	nana::panel<false> type_lyt_panel;
	nana::place type_lyt_place;
	nana::checkbox normal_chk;
	nana::radio_group rg_type;
	nana::checkbox separator_chk;
	nana::label label1;
	nana::textbox item_txt;
	nana::label label2;
	nana::panel<false> layout1_panel;
	nana::place layout1_place;
	nana::textbox img_txt;
	nana::button img_btn;
	nana::combox combox1;
	nana::toolbar toolbar1;
	nana::panel<false> footer_lyt_panel;
	nana::place footer_lyt_place;
	nana::panel<false> spacer_panel;
	nana::place spacer_place;
	nana::button ok_btn;
	nana::button canc_btn;


	//<*declarations
public:
	void type(ctrls::pg_type ctrltype)
	{
		_type = ctrltype;
	}
	ctrls::pg_type type()
	{
		return _type;
	}

	void value(const std::string& items);
	void value(ctrls::pg_type ctrltype, const std::string& items)
	{
		type(ctrltype);
		value(items);
	}
	std::string value();

	nana::msgbox::pick_t return_val() { return _retval; }


protected:
	void init();

	void disable_editing();

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
		std::string text;
		std::string img;
	};
	std::vector<_data_struct>	_data;
	_data_struct*				_selected;

	namemanager					_node_mgr;
	//*>
};

