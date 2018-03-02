/*****************************************************
 *	C++ code generated with Nana Creator (0.6.0)
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
	form1(const nana::rectangle& rect = nana::API::make_center(650, 550), const nana::appearance& apr = {true, true, false, false, false, false, false})
		: nana::form(rect, apr)
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
		enabled(true);
		caption("Nana-Creator Test");
		// layout1
		layout1_place.bind(*this);
		layout1_place.div("margin=5 <margin=0 field1><margin=5 field2>");
		// layout2
		layout2_panel.create(*this);
		layout2_place.bind(layout2_panel);
		layout2_place.div("vert margin=5 <weight=30 margin=0 field1><weight=40 margin=3 field2><weight=30 margin=3 field3><weight=30 margin=3 field4><weight=40 margin=0 field5><weight=30 margin=3 field6><margin=0 field7><margin=0 field8><weight=30 margin=3 field9><weight=30 margin=3 field10><weight=30 margin=3 field11><weight=30 margin=3 field12>");
		layout1_place["field1"] << layout2_panel;
		// label1
		label1.create(layout2_panel);
		layout2_place["field1"] << label1;
		label1.enabled(true);
		label1.caption("Nana-Creator test controls");
		label1.format(false);
		label1.transparent(false);
		label1.text_align(static_cast<nana::align>(1), static_cast<nana::align_v>(1));
		// button1
		button1.create(layout2_panel);
		layout2_place["field2"] << button1;
		button1.enabled(true);
		button1.bgcolor(nana::color(212,208,200));
		button1.fgcolor(nana::color(0,0,0));
		button1.caption("Button");
		button1.transparent(false);
		// categorize1
		categorize1.create(layout2_panel);
		layout2_place["field3"] << categorize1;
		categorize1.enabled(true);
		categorize1.bgcolor(nana::color(255,255,255));
		categorize1.fgcolor(nana::color(0,0,0));
		// checkbox1
		checkbox1.create(layout2_panel);
		layout2_place["field4"] << checkbox1;
		checkbox1.enabled(true);
		checkbox1.bgcolor(nana::color(212,208,200));
		checkbox1.fgcolor(nana::color(0,0,0));
		checkbox1.caption("checkbox");
		checkbox1.check(false);
		checkbox1.radio(false);
		// panel2
		panel2.create(layout2_panel);
		panel2.enabled(true);
		panel2.bgcolor(nana::color(112,255,20));
		layout2_place["field5"] << panel2;
		// layout4
		layout4_place.bind(panel2);
		layout4_place.div("margin=5 <margin=0 field1><margin=0 field2>");
		// checkbox2
		checkbox2.create(panel2);
		layout4_place["field1"] << checkbox2;
		checkbox2.enabled(true);
		checkbox2.bgcolor(nana::color(112,255,20));
		checkbox2.fgcolor(nana::color(0,0,0));
		checkbox2.caption("radio 1");
		checkbox2.check(false);
		checkbox2.radio(true);
		rg_radio.add(checkbox2);
		// checkbox3
		checkbox3.create(panel2);
		layout4_place["field2"] << checkbox3;
		checkbox3.enabled(true);
		checkbox3.bgcolor(nana::color(112,255,20));
		checkbox3.fgcolor(nana::color(0,0,0));
		checkbox3.caption("radio 2");
		checkbox3.check(false);
		checkbox3.radio(true);
		rg_radio.add(checkbox3);
		// combox1
		combox1.create(layout2_panel);
		layout2_place["field6"] << combox1;
		combox1.enabled(true);
		combox1.bgcolor(nana::color(255,255,255));
		combox1.fgcolor(nana::color(0,0,0));
		combox1.editable(false);
		combox1.push_back("A");
		combox1.push_back("B");
		combox1.push_back("C");
		// group1
		group1.create(layout2_panel);
		layout2_place["field7"] << group1;
		group1.enabled(true);
		group1.bgcolor(nana::color(201,197,190));
		group1.fgcolor(nana::color(0,0,0));
		group1.caption("Group");
		// picture1
		picture1.create(layout2_panel);
		layout2_place["field8"] << picture1;
		picture1.enabled(true);
		picture1.bgcolor(nana::color(212,208,200));
		picture1.fgcolor(nana::color(0,0,0));
		picture1.load(nana::paint::image("icons/delete_dark.png"));
		picture1.transparent(false);
		picture1.align(static_cast<nana::align>(0), static_cast<nana::align_v>(0));
		picture1.stretchable(false);
		// progress1
		progress1.create(layout2_panel);
		layout2_place["field9"] << progress1;
		progress1.enabled(true);
		progress1.bgcolor(nana::color(212,208,200));
		progress1.fgcolor(nana::color(16,117,21));
		progress1.value(60);
		// slider1
		slider1.create(layout2_panel);
		layout2_place["field10"] << slider1;
		slider1.enabled(true);
		slider1.bgcolor(nana::color(212,208,200));
		slider1.fgcolor(nana::color(0,0,0));
		slider1.maximum(10);
		slider1.value(2);
		slider1.seek(static_cast<nana::drawerbase::slider::seekdir>(0));
		slider1.vertical(false);
		slider1.transparent(false);
		// spinbox1
		spinbox1.create(layout2_panel);
		layout2_place["field11"] << spinbox1;
		spinbox1.enabled(true);
		spinbox1.bgcolor(nana::color(255,255,255));
		spinbox1.fgcolor(nana::color(0,0,0));
		spinbox1.editable(true);
		// textbox1
		textbox1.create(layout2_panel);
		layout2_place["field12"] << textbox1;
		textbox1.enabled(true);
		textbox1.bgcolor(nana::color(255,255,255));
		textbox1.fgcolor(nana::color(0,0,0));
		textbox1.caption("");
		textbox1.tip_string("");
		textbox1.editable(true);
		textbox1.line_wrapped(false);
		textbox1.multi_lines(true);
		// panel1
		panel1.create(*this);
		panel1.enabled(true);
		panel1.bgcolor(nana::color(22,128,250));
		layout1_place["field2"] << panel1;
		// layout3
		layout3_place.bind(panel1);
		layout3_place.div("vert margin=0 <weight=30 margin=0 field1><weight=40 margin=0 field2><margin=3 field3><margin=3 field4>");
		// menubar1
		menubar1.create(panel1);
		layout3_place["field1"] << menubar1;
		menubar1.enabled(true);
		menubar1.bgcolor(nana::color(212,208,200));
		menubar1.fgcolor(nana::color(0,0,0));
		menubar1.push_back("&Entry1");
		menubar1.push_back("E&ntry2");
		// toolbar1
		toolbar1.create(panel1);
		layout3_place["field2"] << toolbar1;
		toolbar1.enabled(true);
		toolbar1.bgcolor(nana::color(212,208,200));
		toolbar1.fgcolor(nana::color(0,0,0));
		toolbar1.scale(16);
		// datechooser1
		datechooser1.create(panel1);
		layout3_place["field3"] << datechooser1;
		datechooser1.enabled(true);
		datechooser1.bgcolor(nana::color(212,208,200));
		datechooser1.fgcolor(nana::color(0,0,0));
		// listbox1
		listbox1.create(panel1);
		layout3_place["field4"] << listbox1;
		listbox1.enabled(true);
		listbox1.bgcolor(nana::color(255,255,255));
		listbox1.fgcolor(nana::color(0,0,0));
		listbox1.checkable(false);
		listbox1.show_header(true);
		listbox1.append_header("col1");
		listbox1.append_header("col2");

		layout1_place.collocate();
		layout4_place.collocate();
		layout3_place.collocate();
	}


protected:
	nana::place layout1_place;
	nana::panel<false> layout2_panel;
	nana::place layout2_place;
	nana::label label1;
	nana::button button1;
	nana::categorize<int> categorize1;
	nana::checkbox checkbox1;
	nana::panel<true> panel2;
	nana::place layout4_place;
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
	nana::place layout3_place;
	nana::menubar menubar1;
	nana::toolbar toolbar1;
	nana::date_chooser datechooser1;
	nana::listbox listbox1;


	//<*declarations

	//*>
};

