/*****************************************************
 *	C++ code generated with Nana Creator (0.31.0)
 *	GitHub repo: https://github.com/besh81/nana-creator
 *
 * PLEASE EDIT ONLY INSIDE THE TAGS:
 *		//<*tag
 *			user code
 *		//*>
*****************************************************/

#ifndef CTRLS_H
#define CTRLS_H

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/categorize.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/date_chooser.hpp>

//<*includes

//*>


class form1
	: public nana::form
{
public:
	form1(nana::window wd, const ::nana::size& sz = {640, 500}, const nana::appearance& apr = {true, true, false, false, true, true, true})
		: nana::form(wd, sz, apr)
	{
		init_();

		//<*ctor

		//*>
	}

	~form1()
	{
		//<*dtor

		//*>
	}


private:
	void init_()
	{
		place_.bind(*this);
		place_.div("vert <vert weight=56 arrange=[variable,32] menuf><margin=5 <vert weight=300 margin=5 gap=2 arrange=[25,variable,26,24,variable,26,variable,25,30,26,26,30] field1>|<margin=2 gap=2 field2>>");
		caption("Nana-Creator test");
		icon(nana::paint::image("icons/creator.ico"));
		// menubar1
		menubar1.create(*this);
		place_["menuf"] << menubar1;
		menubar1.push_back("M&enu1");
		auto* menubar1_1_submenu_ = &menubar1.push_back("Me&nu2");
		menubar1_1_submenu_->append("Disabled");
		menubar1_1_submenu_->image(0, nana::paint::image("icons/grid_layout.png"));
		menubar1_1_submenu_->enabled(0, false);
		menubar1_1_submenu_->append_splitter();
		menubar1_1_submenu_->append("Options");
		menubar1_1_submenu_->image(2, nana::paint::image("icons/categorize.png"));
		auto* menubar1_1_submenu__2 = menubar1_1_submenu_->create_sub_menu(2);
		menubar1_1_submenu__2->append("Option1");
		menubar1_1_submenu__2->check_style(0, static_cast<nana::menu::checks>(1));
		menubar1_1_submenu__2->append("Option2");
		menubar1_1_submenu__2->check_style(1, static_cast<nana::menu::checks>(1));
		menubar1_1_submenu__2->checked(1, true);
		menubar1_1_submenu_->append("Highlights");
		menubar1_1_submenu_->image(3, nana::paint::image("icons/checkbox.png"));
		auto* menubar1_1_submenu__3 = menubar1_1_submenu_->create_sub_menu(3);
		menubar1_1_submenu__3->append("Highlight1");
		menubar1_1_submenu__3->check_style(0, static_cast<nana::menu::checks>(2));
		menubar1_1_submenu__3->checked(0, true);
		menubar1_1_submenu__3->append("Highlight2");
		menubar1_1_submenu__3->check_style(1, static_cast<nana::menu::checks>(2));
		menubar1.push_back("Men&u3");
		// toolbar1
		toolbar1.create(*this);
		place_["menuf"] << toolbar1;
		toolbar1.bgcolor(this->bgcolor());
		toolbar1.tools_height(22);
		toolbar1.append(nana::toolbar::tools::button, "Add", nana::paint::image("icons/save.png"));
		toolbar1.append(nana::toolbar::tools::button, "Delete", nana::paint::image("icons/delete.png")).enable(false);
		toolbar1.append_separator();
		toolbar1.append(nana::toolbar::tools::toggle, "Toggle A").textout(true).toggle_group("Gruppo1").toggle(true);
		toolbar1.append(nana::toolbar::tools::toggle, "Toggle B").textout(true).toggle_group("Gruppo1");
		toolbar1.append_separator();
		auto ip_toolbar1_6 = toolbar1.append(nana::toolbar::tools::dropdown, "Dropdown", nana::paint::image("icons/panel.png"));
		ip_toolbar1_6.dropdown_append("ZERO", nana::paint::image("icons/up.png"));
		ip_toolbar1_6.dropdown_append("UNO", nana::paint::image("icons/delete.png"));
		ip_toolbar1_6.dropdown_enable(1, false);
		ip_toolbar1_6.dropdown_append("DUE", nana::paint::image("icons/down.png"));
		toolbar1.go_right();
		toolbar1.append(nana::toolbar::tools::button, "Right", nana::paint::image("icons/info.png"));
		// label1
		label1.create(*this);
		place_["field1"] << label1;
		label1.bgcolor(this->bgcolor());
		label1.fgcolor(nana::color(255,0,0));
		label1.caption("Nana-Creator test");
		label1.text_align(static_cast<nana::align>(1), static_cast<nana::align_v>(1));
		// button1
		button1.create(*this);
		place_["field1"] << button1;
		button1.caption("button");
		// categorize1
		categorize1.create(*this);
		place_["field1"] << categorize1;
		// checkbox1
		checkbox1.create(*this);
		place_["field1"] << checkbox1;
		checkbox1.typeface(nana::paint::font("", 12, {600, true, false, false}));
		checkbox1.caption("checkbox");
		// group1
		group1.create(*this);
		group1.div("margin=5 gap=2 _field_");
		group1.caption("group");
		place_["field1"] << group1;
		// checkbox2
		checkbox2.create(group1);
		group1["_field_"] << checkbox2;
		checkbox2.bgcolor(nana::color(212,208,200));
		checkbox2.caption("radio1");
		group1_radiogroup_.add(checkbox2);
		checkbox2.radio(true);
		checkbox2.transparent(true);
		// checkbox3
		checkbox3.create(group1);
		group1["_field_"] << checkbox3;
		checkbox3.bgcolor(nana::color(212,208,200));
		checkbox3.caption("radio2");
		group1_radiogroup_.add(checkbox3);
		checkbox3.check(true);
		checkbox3.radio(true);
		checkbox3.transparent(true);
		// combox1
		combox1.create(*this);
		place_["field1"] << combox1;
		combox1.push_back("UNO");
		combox1.push_back("Tre");
		combox1.image(1, nana::paint::image("icons/up.png"));
		combox1.push_back("due");
		combox1.option(0);
		combox1.image_pixels(21);
		// picture1
		picture1.create(*this);
		place_["field1"] << picture1;
		picture1.bgcolor(this->bgcolor());
		picture1.load(nana::paint::image("icons/save.png"));
		picture1.align(static_cast<nana::align>(1), static_cast<nana::align_v>(1));
		picture1.set_gradual_background(nana::color(255,255,0), nana::color(0,155,0), true);
		// progress1
		progress1.create(*this);
		place_["field1"] << progress1;
		progress1.amount(30);
		progress1.value(10);
		// slider1
		slider1.create(*this);
		place_["field1"] << slider1;
		slider1.value(4);
		// spinbox1
		spinbox1.create(*this);
		place_["field1"] << spinbox1;
		spinbox1.range(-20, 20, 5);
		spinbox1.value("-5");
		spinbox1.editable(false);
		spinbox1.modifier("", " Km");
		// textbox1
		textbox1.create(*this);
		place_["field1"] << textbox1;
		textbox1.tip_string("insert text");
		textbox1.multi_lines(false);
		textbox1.focus_behavior(static_cast<nana::textbox::text_focus_behavior>(3));
		textbox1.scheme().selection_text = nana::color(255,0,0);
		// tabbar1
		tabbar1.create(*this);
		place_["field1"] << tabbar1;
		tabbar1.push_back("tab1");
		tabbar1.tab_bgcolor(0, nana::color(212,208,200));
		tabbar1.tab_fgcolor(0, nana::color(220,0,0));
		tabbar1.push_back("tab2");
		tabbar1.tab_image(1, nana::paint::image("icons/down.png"));
		tabbar1.tab_bgcolor(1, nana::color(212,208,200));
		tabbar1.tab_fgcolor(1, nana::color(0,0,0));
		tabbar1.push_back("tab3");
		tabbar1.tab_bgcolor(2, nana::color(212,208,200));
		tabbar1.tab_fgcolor(2, nana::color(0,0,0));
		tabbar1.activated(1);
		// panel1
		panel1.create(*this);
		panel1_place_.bind(panel1);
		panel1_place_.div("vert margin=5 gap=2 _field_");
		panel1.bgcolor(nana::color(120,208,200));
		panel1.fgcolor(this->fgcolor());
		place_["field2"] << panel1;
		// notebook1
		notebook1_panel_.create(panel1);
		notebook1_tabbar_.create(notebook1_panel_);
		notebook1_place_.bind(notebook1_panel_);
		notebook1_place_.div("vert margin=1 <vert weight=30 tabs><pages>");
		notebook1_place_["tabs"] << notebook1_tabbar_;
		notebook1_tabbar_.toolbox(nana::drawerbase::tabbar::trigger::kits::close, true);
		notebook1_tabbar_.close_fly(true);
		panel1_place_["_field_"] << notebook1_panel_;
		// page1
		page1.create(notebook1_panel_);
		page1_place_.bind(page1);
		page1_place_.div("vert margin=5 <grid=[2,2] margin=[5,5,5,5] gap=2 collapse(0,0,2,1) grid1>");
		page1.bgcolor(nana::color(212,208,20));
		page1.fgcolor(nana::color(0,0,255));
		notebook1_tabbar_.push_back("page");
		notebook1_tabbar_.tab_image(notebook1_tabbar_.length()-1, nana::paint::image("icons/grid_layout.png"));
		notebook1_tabbar_.tab_bgcolor(notebook1_tabbar_.length()-1, page1.bgcolor());
		notebook1_tabbar_.tab_fgcolor(notebook1_tabbar_.length()-1, page1.fgcolor());
		notebook1_tabbar_.attach(notebook1_tabbar_.length()-1, page1);
		notebook1_place_["pages"].fasten(page1);
		// listbox1
		listbox1.create(page1);
		page1_place_["grid1"] << listbox1;
		listbox1.append_header("col 1", 55);
		listbox1.append_header("col 2");
		// treebox1
		treebox1.create(page1);
		page1_place_["grid1"] << treebox1;
		// page2
		page2.create(notebook1_panel_);
		page2_place_.bind(page2);
		page2_place_.div("margin=5 gap=2 _field_");
		notebook1_tabbar_.push_back("page");
		notebook1_tabbar_.tab_image(notebook1_tabbar_.length()-1, nana::paint::image("icons/datechooser.png"));
		notebook1_tabbar_.tab_bgcolor(notebook1_tabbar_.length()-1, page2.bgcolor());
		notebook1_tabbar_.tab_fgcolor(notebook1_tabbar_.length()-1, page2.fgcolor());
		notebook1_tabbar_.attach(notebook1_tabbar_.length()-1, page2);
		notebook1_place_["pages"].fasten(page2);
		// datechooser1
		datechooser1.create(page2);
		page2_place_["_field_"] << datechooser1;

		place_.collocate();
		group1.collocate();
		panel1_place_.collocate();
		notebook1_tabbar_.activated(0);
		notebook1_place_.collocate();
		page1_place_.collocate();
		page2_place_.collocate();
	}


protected:
	nana::place place_;
	nana::menubar menubar1;
	nana::toolbar toolbar1;
	nana::label label1;
	nana::button button1;
	nana::categorize<int> categorize1;
	nana::checkbox checkbox1;
	nana::group group1;
	nana::checkbox checkbox2;
	nana::radio_group group1_radiogroup_;
	nana::checkbox checkbox3;
	nana::combox combox1;
	nana::picture picture1;
	nana::progress progress1;
	nana::slider slider1;
	nana::spinbox spinbox1;
	nana::textbox textbox1;
	nana::tabbar<size_t> tabbar1;
	nana::panel<true> panel1;
	nana::place panel1_place_;
	nana::panel<true> notebook1_panel_;
	nana::tabbar<size_t> notebook1_tabbar_;
	nana::place notebook1_place_;
	nana::panel<true> page1;
	nana::place page1_place_;
	nana::listbox listbox1;
	nana::treebox treebox1;
	nana::panel<true> page2;
	nana::place page2_place_;
	nana::date_chooser datechooser1;


	//<*declarations

	//*>
};

#endif //CTRLS_H

