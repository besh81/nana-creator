/*****************************************************
 *	C++ code generated with Nana Creator (0.5.0)
 *	GitHub repo: https://github.com/besh81/nana-creator
 *
 * PLEASE EDIT ONLY INSIDE THE TAGS:
 *		//<*tag
 *			user code
 *		//*>
*****************************************************/


#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/button.hpp>

//<*includes

//*>


class items_dialog
	: public nana::form
{
public:
	items_dialog(const nana::rectangle& rect = nana::API::make_center(300, 400), const nana::appearance& apr = {true, true, false, false, false, false, false})
		: nana::form(rect, apr)
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
		caption("Items");
		enabled(true);
		// main_lyt
		main_lyt_place.bind(*this);
		main_lyt_place.div("vert margin=5 <weight=40 margin=0 field1><margin=0 field2><weight=40 margin=0 field3>");
		// _toolbar
		_toolbar.create(*this);
		_toolbar.enabled(true);
		_toolbar.bgcolor(nana::color(212,208,200));
		_toolbar.fgcolor(nana::color(0,0,0));
		_toolbar.scale(21);
		main_lyt_place["field1"] << _toolbar;
		// _list
		_list.create(*this);
		_list.append_header("items");
		_list.checkable(false);
		_list.enable_single(true, true);
		_list.enabled(true);
		_list.bgcolor(nana::color(255,255,255));
		_list.fgcolor(nana::color(0,0,0));
		_list.show_header(false);
		main_lyt_place["field2"] << _list;
		// footer_lyt
		footer_lyt_panel.create(*this);
		footer_lyt_place.bind(footer_lyt_panel);
		footer_lyt_place.div("margin=3 <margin=0 field1><weight=90 margin=3 field2><weight=90 margin=3 field3>");
		main_lyt_place["field3"] << footer_lyt_panel;
		// layout1
		layout1_panel.create(footer_lyt_panel);
		layout1_place.bind(layout1_panel);
		layout1_place.div("margin=5 ");
		footer_lyt_place["field1"] << layout1_panel;
		// ok_btn
		ok_btn.create(footer_lyt_panel);
		ok_btn.caption("OK");
		ok_btn.enabled(true);
		ok_btn.bgcolor(nana::color(212,208,200));
		ok_btn.fgcolor(nana::color(0,0,0));
		ok_btn.transparent(false);
		footer_lyt_place["field2"] << ok_btn;
		// canc_btn
		canc_btn.create(footer_lyt_panel);
		canc_btn.caption("Cancel");
		canc_btn.enabled(true);
		canc_btn.bgcolor(nana::color(212,208,200));
		canc_btn.fgcolor(nana::color(0,0,0));
		canc_btn.transparent(false);
		footer_lyt_place["field3"] << canc_btn;

		main_lyt_place.collocate();
	}


protected:
	nana::place main_lyt_place;
	nana::toolbar _toolbar;
	nana::listbox _list;
	nana::panel<false> footer_lyt_panel;
	nana::place footer_lyt_place;
	nana::panel<false> layout1_panel;
	nana::place layout1_place;
	nana::button ok_btn;
	nana::button canc_btn;


	//<*declarations
public:
	items_dialog(nana::window wd, const ::nana::size& sz = { 300, 400 }, const nana::appearance& apr = { true, true, false, false, false, false, false })
		: nana::form(wd, sz, apr)
	{
		init_();
		init();
	}

	void value(const std::string& items);
	std::string value();
	nana::msgbox::pick_t return_val() { return ret_val; }


protected:
	void init();

	nana::msgbox::pick_t ret_val;
	//*>
};

