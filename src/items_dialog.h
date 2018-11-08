/*****************************************************
 *	C++ code generated with Nana Creator (0.13.0)
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
#include "nana_extra/propgrid.h"
#include "namemanager.h"
//*>


class items_dialog
	: public nana::form
{
public:
	items_dialog(nana::window wd, const ::nana::size& sz = {600, 480}, const nana::appearance& apr = {true, true, false, false, false, false, false})
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
		_place.div("vert margin=5 <<vert margin=3 gap=2 arrange=[variable,30] left_lyt><margin=3 gap=2 right_lyt>><weight=40 margin=5 gap=3 arrange=[variable,90,90] footer_lyt>");
		caption("Form");
		// items_tree
		items_tree.create(*this);
		_place["left_lyt"] << items_tree;
		items_tree.bgcolor(nana::color(255,255,255));
		// toolbar
		toolbar.create(*this);
		_place["left_lyt"] << toolbar;
		toolbar.scale(21);
		toolbar.append("Add item", nana::paint::image("icons/item_add.png"));
		toolbar.append("Delete item", nana::paint::image("icons/item_delete.png"));
		toolbar.separate();
		toolbar.append("Move up", nana::paint::image("icons/up.png"));
		toolbar.append("Move down", nana::paint::image("icons/down.png"));
		toolbar.separate();
		toolbar.append("Move left", nana::paint::image("icons/left.png"));
		toolbar.append("Move right", nana::paint::image("icons/right.png"));
		// panel1
		panel1.create(*this);
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

	void value(const std::string& items);
	std::string value();

	nana::msgbox::pick_t return_val() { return _retval; }


protected:
	void init();

	void select_item(const std::string& key);
	void erase_item(const std::string& key);

	void update_selected();
	void update_text(nana::drawerbase::treebox::item_proxy& ip, const std::string& text);
	void update_image(nana::drawerbase::treebox::item_proxy& ip, const std::string& filename);

	ctrls::pg_type				_type;

	nana::treebox::item_proxy	_root;
	nana::msgbox::pick_t		_retval;

	struct _data_struct
	{
		std::string key;
		std::string owner;

		std::string text;
		std::string img;
		bool		separator{ false };
		std::string width;
		std::string left{ "0" };
		std::string top{ "0" };
		std::string cols{ "0" };
		std::string rows{ "0" };
		std::string bgcolor;
		std::string fgcolor;
	};
	std::vector<_data_struct>	_data;
	_data_struct*				_selected{ 0 };

	namemanager					_node_mgr;

	nana::propertygrid			_propgrid{ *this };
	//*>
};

#endif //ITEMS_DIALOG_H

