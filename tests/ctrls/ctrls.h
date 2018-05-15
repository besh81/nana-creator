/*****************************************************
 *	C++ code generated with Nana Creator (0.9.0)
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
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/categorize.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/widgets/date_chooser.hpp>
#include <nana/gui/widgets/listbox.hpp>

//<*includes

//*>


class form1
	: public nana::form
{
public:
	form1(nana::window wd, const ::nana::size& sz = {650, 550}, const nana::appearance& apr = {true, true, false, false, false, false, false})
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
		_place.div("margin=5 <field1><margin=5 field2>");
		enabled(true);
		caption("Nana-Creator Test");
		// layout1
		layout1_panel.create(*this);
		layout1_place.bind(layout1_panel);
		layout1_place.div("vert margin=5 <weight=30 field1><weight=40 margin=3 field2><weight=30 margin=3 field3><weight=30 margin=3 field4><weight=40 field5><weight=30 margin=3 field6><field7><field8><weight=30 margin=3 field9><weight=30 margin=3 field10><weight=30 margin=3 field11><weight=30 margin=3 field12>");
		_place["field1"] << layout1_panel;
		// label1
		label1.create(layout1_panel);
		layout1_place["field1"] << label1;
		label1.caption("Nana-Creator test controls");
		label1.format(false);
		label1.transparent(false);
		label1.text_align(static_cast<nana::align>(1), static_cast<nana::align_v>(1));
		// button1
		button1.create(layout1_panel);
		layout1_place["field2"] << button1;
		button1.bgcolor(nana::color(212,208,200));
		button1.fgcolor(nana::color(0,0,0));
		button1.caption("Button");
		// categorize1
		categorize1.create(layout1_panel);
		layout1_place["field3"] << categorize1;
		categorize1.bgcolor(nana::color(255,255,255));
		categorize1.fgcolor(nana::color(0,0,0));
		// checkbox1
		checkbox1.create(layout1_panel);
		layout1_place["field4"] << checkbox1;
		checkbox1.bgcolor(nana::color(212,208,200));
		checkbox1.fgcolor(nana::color(0,0,0));
		checkbox1.caption("checkbox");
		checkbox1.check(false);
		checkbox1.radio(false);
		// panel2
		panel2.create(layout1_panel);
		panel2_place.bind(panel2);
		panel2_place.div("margin=5 <field1><field2>");
		panel2.bgcolor(nana::color(112,255,20));
		layout1_place["field5"] << panel2;
		// checkbox2
		checkbox2.create(panel2);
		panel2_place["field1"] << checkbox2;
		checkbox2.bgcolor(nana::color(112,255,20));
		checkbox2.fgcolor(nana::color(0,0,0));
		checkbox2.caption("radio 1");
		checkbox2.check(false);
		checkbox2.radio(true);
		rg_radio.add(checkbox2);
		// checkbox3
		checkbox3.create(panel2);
		panel2_place["field2"] << checkbox3;
		checkbox3.bgcolor(nana::color(112,255,20));
		checkbox3.fgcolor(nana::color(0,0,0));
		checkbox3.caption("radio 2");
		checkbox3.check(false);
		checkbox3.radio(true);
		rg_radio.add(checkbox3);
		// combox1
		combox1.create(layout1_panel);
		layout1_place["field6"] << combox1;
		combox1.bgcolor(nana::color(255,255,255));
		combox1.fgcolor(nana::color(0,0,0));
		combox1.push_back("A");
		combox1.push_back("B");
		combox1.push_back("C");
		combox1.option(0);
		combox1.editable(false);
		// group1
		group1.create(layout1_panel);
		group1.caption("Group");
		group1.bgcolor(nana::color(201,197,190));
		group1.fgcolor(nana::color(0,0,0));
		layout1_place["field7"] << group1;
		// picture1
		picture1.create(layout1_panel);
		layout1_place["field8"] << picture1;
		picture1.bgcolor(nana::color(212,208,200));
		picture1.fgcolor(nana::color(0,0,0));
		picture1.load(nana::paint::image("icons/save.png"));
		picture1.transparent(false);
		picture1.align(static_cast<nana::align>(0), static_cast<nana::align_v>(0));
		picture1.stretchable(false);
		// progress1
		progress1.create(layout1_panel);
		layout1_place["field9"] << progress1;
		progress1.bgcolor(nana::color(212,208,200));
		progress1.fgcolor(nana::color(16,117,21));
		progress1.value(60);
		// slider1
		slider1.create(layout1_panel);
		layout1_place["field10"] << slider1;
		slider1.bgcolor(nana::color(212,208,200));
		slider1.fgcolor(nana::color(0,0,0));
		slider1.maximum(10);
		slider1.value(2);
		slider1.seek(static_cast<nana::drawerbase::slider::seekdir>(0));
		slider1.vertical(false);
		slider1.transparent(false);
		// spinbox1
		spinbox1.create(layout1_panel);
		layout1_place["field11"] << spinbox1;
		spinbox1.bgcolor(nana::color(255,255,255));
		spinbox1.fgcolor(nana::color(0,0,0));
		spinbox1.range(0, 100, 1);
		spinbox1.value("0");
		spinbox1.editable(true);
		spinbox1.modifier("", "");
		// textbox1
		textbox1.create(layout1_panel);
		layout1_place["field12"] << textbox1;
		textbox1.bgcolor(nana::color(255,255,255));
		textbox1.fgcolor(nana::color(0,0,0));
		textbox1.caption("");
		textbox1.tip_string("");
		textbox1.editable(true);
		textbox1.line_wrapped(false);
		textbox1.multi_lines(true);
		// panel1
		panel1.create(*this);
		panel1_place.bind(panel1);
		panel1_place.div("vert margin=5 <weight=30 field1><weight=40 field2><margin=3 field3><margin=3 field4>");
		panel1.bgcolor(nana::color(22,128,250));
		_place["field2"] << panel1;
		// menubar1
		menubar1.create(panel1);
		panel1_place["field1"] << menubar1;
		auto* menubar1_0 = &menubar1.push_back("&File");
		menubar1_0->append("E&xit");
		auto* menubar1_1 = &menubar1.push_back("menu_1");
		menubar1_1->append("menu_1_1");
		auto* menubar1_1_0 = menubar1_1->create_sub_menu(0);
		menubar1_1_0->append("menu_1_1_1");
		menubar1.push_back("&Help");
		// toolbar1
		toolbar1.create(panel1);
		panel1_place["field2"] << toolbar1;
		toolbar1.bgcolor(nana::color(212,208,200));
		toolbar1.fgcolor(nana::color(0,0,0));
		toolbar1.scale(21);
		toolbar1.append("Uno", nana::paint::image("icons/up.png"));
		toolbar1.separate();
		toolbar1.append("Due", nana::paint::image("icons/down.png"));
		// datechooser1
		datechooser1.create(panel1);
		panel1_place["field3"] << datechooser1;
		datechooser1.bgcolor(nana::color(212,208,200));
		datechooser1.fgcolor(nana::color(0,0,0));
		// listbox1
		listbox1.create(panel1);
		panel1_place["field4"] << listbox1;
		listbox1.bgcolor(nana::color(255,255,255));
		listbox1.fgcolor(nana::color(0,0,0));
		listbox1.checkable(false);
		listbox1.show_header(true);
		listbox1.append_header("Col1", 150);
		listbox1.append_header("Col2");

		_place.collocate();
		panel2_place.collocate();
		panel1_place.collocate();
	}


protected:
	nana::place _place{ *this };
	nana::panel<false> layout1_panel;
	nana::place layout1_place;
	nana::label label1;
	nana::button button1;
	nana::categorize<int> categorize1;
	nana::checkbox checkbox1;
	nana::panel<true> panel2;
	nana::place panel2_place;
	nana::checkbox checkbox2;
	nana::radio_group rg_radio;
	nana::checkbox checkbox3;
	nana::combox combox1;
	nana::group group1;
	nana::picture picture1;
	nana::progress progress1;
	nana::slider slider1;
	nana::spinbox spinbox1;
	nana::textbox textbox1;
	nana::panel<true> panel1;
	nana::place panel1_place;
	nana::menubar menubar1;
	nana::toolbar toolbar1;
	nana::date_chooser datechooser1;
	nana::listbox listbox1;


	//<*declarations

	//*>
};

