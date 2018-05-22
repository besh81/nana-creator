/*****************************************************
 *	C++ code generated with Nana Creator (0.10.0)
 *	GitHub repo: https://github.com/besh81/nana-creator
 *
 * PLEASE EDIT ONLY INSIDE THE TAGS:
 *		//<*tag
 *			user code
 *		//*>
*****************************************************/

#ifndef NEW_PROJECT_H
#define NEW_PROJECT_H

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>

//<*includes

//*>


class new_project
	: public nana::form
{
public:
	new_project(nana::window wd, const ::nana::size& sz = {540, 350}, const nana::appearance& apr = {true, true, false, false, false, false, false})
		: nana::form(wd, sz, apr)
	{
		init_();

		//<*ctor
		init();
		//*>
	}

	~new_project()
	{
		//<*dtor

		//*>
	}


private:
	void init_()
	{
		_place.div("vert margin=5 <weight=30 margin=2 field1><field2><weight=40 margin=5 field3><weight=40 margin=5 field4><weight=40 field5>");
		enabled(true);
		caption("New project");
		// label3
		label3.create(*this);
		_place["field1"] << label3;
		label3.caption("   Type:");
		label3.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(2));
		// layout1
		layout1_panel.create(*this);
		layout1_place.bind(layout1_panel);
		layout1_place.div("margin=3 <weight=180 margin=3 field1><margin=3 field2>");
		_place["field2"] << layout1_panel;
		// type_lst
		type_lst.create(layout1_panel);
		layout1_place["field1"] << type_lst;
		type_lst.bgcolor(nana::color(255,255,255));
		type_lst.checkable(false);
		type_lst.enable_single(true, true);
		type_lst.show_header(false);
		type_lst.append_header("Widget type", 150);
		// type_txt
		type_txt.create(layout1_panel);
		layout1_place["field2"] << type_txt;
		type_txt.bgcolor(nana::color(255,255,255));
		type_txt.caption("");
		type_txt.editable(false);
		type_txt.line_wrapped(true);
		type_txt.multi_lines(true);
		// layout2
		layout2_panel.create(*this);
		layout2_place.bind(layout2_panel);
		layout2_place.div("<weight=100 margin=3 field1><margin=3 field2>");
		_place["field3"] << layout2_panel;
		// label1
		label1.create(layout2_panel);
		layout2_place["field1"] << label1;
		label1.caption("Name:");
		label1.text_align(static_cast<nana::align>(2), static_cast<nana::align_v>(1));
		// name_txt
		name_txt.create(layout2_panel);
		layout2_place["field2"] << name_txt;
		name_txt.bgcolor(nana::color(255,255,255));
		name_txt.caption("");
		name_txt.multi_lines(false);
		// layout21
		layout21_panel.create(*this);
		layout21_place.bind(layout21_panel);
		layout21_place.div("<weight=100 margin=3 field1><margin=3 field2><weight=50 margin=3 field3>");
		_place["field4"] << layout21_panel;
		// label2
		label2.create(layout21_panel);
		layout21_place["field1"] << label2;
		label2.caption("Create in:");
		label2.text_align(static_cast<nana::align>(2), static_cast<nana::align_v>(1));
		// folder_txt
		folder_txt.create(layout21_panel);
		layout21_place["field2"] << folder_txt;
		folder_txt.bgcolor(nana::color(255,255,255));
		folder_txt.caption("");
		folder_txt.editable(false);
		folder_txt.multi_lines(false);
		// folder_btn
		folder_btn.create(layout21_panel);
		layout21_place["field3"] << folder_btn;
		folder_btn.caption("...");
		// footer_lyt
		footer_lyt_panel.create(*this);
		footer_lyt_place.bind(footer_lyt_panel);
		footer_lyt_place.div("margin=2 <field1><weight=90 margin=3 field2><weight=90 margin=3 field3>");
		_place["field5"] << footer_lyt_panel;
		// spacer
		spacer_panel.create(footer_lyt_panel);
		spacer_place.bind(spacer_panel);
		spacer_place.div("margin=5 ");
		footer_lyt_place["field1"] << spacer_panel;
		// create_btn
		create_btn.create(footer_lyt_panel);
		footer_lyt_place["field2"] << create_btn;
		create_btn.caption("Create");
		// canc_btn
		canc_btn.create(footer_lyt_panel);
		footer_lyt_place["field3"] << canc_btn;
		canc_btn.caption("Cancel");

		_place.collocate();
	}


protected:
	nana::place _place{ *this };
	nana::label label3;
	nana::panel<false> layout1_panel;
	nana::place layout1_place;
	nana::listbox type_lst;
	nana::textbox type_txt;
	nana::panel<false> layout2_panel;
	nana::place layout2_place;
	nana::label label1;
	nana::textbox name_txt;
	nana::panel<false> layout21_panel;
	nana::place layout21_place;
	nana::label label2;
	nana::textbox folder_txt;
	nana::button folder_btn;
	nana::panel<false> footer_lyt_panel;
	nana::place footer_lyt_place;
	nana::panel<false> spacer_panel;
	nana::place spacer_place;
	nana::button create_btn;
	nana::button canc_btn;


	//<*declarations
public:
	std::string get_ctrl_type() { return _ctrltype; }
	std::string get_filename() { return _filename; }

	nana::msgbox::pick_t return_val() { return _retval; }


protected:
	void init();


	std::string				_ctrltype;
	std::string				_filename;
	nana::msgbox::pick_t	_retval;
	//*>
};

#endif //NEW_PROJECT_H

