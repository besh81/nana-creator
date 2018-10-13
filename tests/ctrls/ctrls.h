/*****************************************************
 *	C++ code generated with Nana Creator (0.13.0)
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
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/date_chooser.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/toolbar.hpp>

//<*includes

//*>


class form1
	: public nana::form
{
public:
	form1(nana::window wd, const ::nana::size& sz = {640, 480}, const nana::appearance& apr = {true, true, false, false, true, true, true})
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
		_place.div("margin=3 <vert weight=300 margin=5 gap=2 arrange=[25,variable,variable,24,variable,26,variable,25,30,26,26] field1><margin=2 gap=5 field2>");
		caption("Nana-Creator test");
		// label1
		label1.create(*this);
		_place["field1"] << label1;
		label1.fgcolor(nana::color(255,0,0));
		label1.caption("Nana-Creator test");
		label1.text_align(static_cast<nana::align>(1), static_cast<nana::align_v>(1));
		// button1
		button1.create(*this);
		_place["field1"] << button1;
		button1.caption("button");
		// categorize1
		categorize1.create(*this);
		_place["field1"] << categorize1;
		categorize1.bgcolor(nana::color(255,255,255));
		// checkbox1
		checkbox1.create(*this);
		_place["field1"] << checkbox1;
		checkbox1.caption("checkbox");
		checkbox1.check(false);
		checkbox1.radio(false);
		// group1
		group1.create(*this);
		group1.caption_align(static_cast<nana::align>(0));
		group1.div("margin=5 gap=2 _field_");
		group1.caption("group");
		group1.bgcolor(nana::color(201,197,190));
		_place["field1"] << group1;
		// checkbox2
		checkbox2.create(group1);
		group1["_field_"] << checkbox2;
		checkbox2.caption("radio1");
		checkbox2.check(false);
		checkbox2.radio(true);
		rg_gruppo1.add(checkbox2);
		// checkbox3
		checkbox3.create(group1);
		group1["_field_"] << checkbox3;
		checkbox3.caption("radio2");
		checkbox3.check(true);
		checkbox3.radio(true);
		rg_gruppo1.add(checkbox3);
		// combox1
		combox1.create(*this);
		_place["field1"] << combox1;
		combox1.push_back("UNO");
		combox1.push_back("tre");
		combox1.image(1, nana::paint::image("icons/up.png"));
		combox1.push_back("DUE");
		combox1.option(0);
		combox1.editable(false);
		// picture1
		picture1.create(*this);
		_place["field1"] << picture1;
		picture1.load(nana::paint::image("icons/save.png"));
		picture1.transparent(false);
		picture1.align(static_cast<nana::align>(0), static_cast<nana::align_v>(0));
		picture1.stretchable(false);
		// progress1
		progress1.create(*this);
		_place["field1"] << progress1;
		progress1.fgcolor(nana::color(16,117,21));
		progress1.value(25);
		// slider1
		slider1.create(*this);
		_place["field1"] << slider1;
		slider1.maximum(10);
		slider1.value(4);
		slider1.seek(static_cast<nana::drawerbase::slider::seekdir>(0));
		slider1.vertical(false);
		slider1.transparent(false);
		// spinbox1
		spinbox1.create(*this);
		_place["field1"] << spinbox1;
		spinbox1.bgcolor(nana::color(255,255,255));
		spinbox1.range(0, 100, 5);
		spinbox1.value("55");
		spinbox1.editable(true);
		spinbox1.modifier("", "");
		// textbox1
		textbox1.create(*this);
		_place["field1"] << textbox1;
		textbox1.bgcolor(nana::color(255,255,255));
		textbox1.caption("");
		textbox1.tip_string("insert text");
		textbox1.multi_lines(false);
		// panel1
		panel1.create(*this);
		panel1_place.bind(panel1);
		panel1_place.div("vert margin=5 gap=5 arrange=[40%,variable,variable,30] _field_");
		panel1.bgcolor(nana::color(120,208,200));
		_place["field2"] << panel1;
		// datechooser1
		datechooser1.create(panel1);
		panel1_place["_field_"] << datechooser1;
		// listbox1
		listbox1.create(panel1);
		panel1_place["_field_"] << listbox1;
		listbox1.bgcolor(nana::color(255,255,255));
		listbox1.checkable(false);
		listbox1.show_header(true);
		// treebox1
		treebox1.create(panel1);
		panel1_place["_field_"] << treebox1;
		treebox1.bgcolor(nana::color(255,255,255));
		// toolbar1
		toolbar1.create(panel1);
		panel1_place["_field_"] << toolbar1;
		toolbar1.scale(21);
		toolbar1.append("Add", nana::paint::image("icons/item_add.png"));
		toolbar1.separate();
		toolbar1.append("Delete", nana::paint::image("icons/item_delete.png"));

		_place.collocate();
		group1.collocate();
		panel1_place.collocate();
	}


protected:
	nana::place _place{ *this };
	nana::label label1;
	nana::button button1;
	nana::categorize<int> categorize1;
	nana::checkbox checkbox1;
	nana::group group1;
	nana::checkbox checkbox2;
	nana::radio_group rg_gruppo1;
	nana::checkbox checkbox3;
	nana::combox combox1;
	nana::picture picture1;
	nana::progress progress1;
	nana::slider slider1;
	nana::spinbox spinbox1;
	nana::textbox textbox1;
	nana::panel<true> panel1;
	nana::place panel1_place;
	nana::date_chooser datechooser1;
	nana::listbox listbox1;
	nana::treebox treebox1;
	nana::toolbar toolbar1;


	//<*declarations

	//*>
};

#endif //CTRLS_H

