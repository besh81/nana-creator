/*****************************************************
 *	C++ code generated with Nana Creator (0.17.0)
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
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/panel.hpp>

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
		_place.div("vert margin=5 <weight=30 gap=2 field1><margin=5 gap=5 arrange=[180,variable] field2><weight=36 margin=5 gap=3 arrange=[100,variable] field3><weight=36 margin=5 gap=3 arrange=[100,variable,50] field4><weight=40 margin=5 gap=3 arrange=[variable,90,90] field5>");
		caption("New project");
		// label3
		label3.create(*this);
		_place["field1"] << label3;
		label3.caption("   Type:");
		label3.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(2));
		// type_lst
		type_lst.create(*this);
		_place["field2"] << type_lst;
		type_lst.enable_single(true, true);
		type_lst.show_header(false);
		type_lst.append_header("Widget", 150);
		// type_txt
		type_txt.create(*this);
		_place["field2"] << type_txt;
		type_txt.caption("");
		type_txt.editable(false);
		type_txt.line_wrapped(true);
		type_txt.multi_lines(true);
		// label1
		label1.create(*this);
		_place["field3"] << label1;
		label1.caption("Name:");
		label1.text_align(static_cast<nana::align>(2), static_cast<nana::align_v>(1));
		// name_txt
		name_txt.create(*this);
		_place["field3"] << name_txt;
		name_txt.caption("");
		name_txt.multi_lines(false);
		// label2
		label2.create(*this);
		_place["field4"] << label2;
		label2.caption("Create in:");
		label2.text_align(static_cast<nana::align>(2), static_cast<nana::align_v>(1));
		// folder_txt
		folder_txt.create(*this);
		_place["field4"] << folder_txt;
		folder_txt.caption("");
		folder_txt.editable(false);
		folder_txt.multi_lines(false);
		// folder_btn
		folder_btn.create(*this);
		_place["field4"] << folder_btn;
		folder_btn.caption("...");
		// panel1
		panel1.create(*this);
		panel1.transparent(true);
		_place["field5"] << panel1;
		// create_btn
		create_btn.create(*this);
		_place["field5"] << create_btn;
		create_btn.caption("Create");
		// canc_btn
		canc_btn.create(*this);
		_place["field5"] << canc_btn;
		canc_btn.caption("Cancel");

		_place.collocate();
	}


protected:
	nana::place _place{ *this };
	nana::label label3;
	nana::listbox type_lst;
	nana::textbox type_txt;
	nana::label label1;
	nana::textbox name_txt;
	nana::label label2;
	nana::textbox folder_txt;
	nana::button folder_btn;
	nana::panel<true> panel1;
	nana::button create_btn;
	nana::button canc_btn;


	//<*declarations
public:
	std::string get_ctrl_type() { return _ctrltype; }
	std::string get_filename() { return _filename; }
	std::string get_projectname() { return _projectname; }

	nana::msgbox::pick_t return_val() { return _retval; }


protected:
	void init();


	std::string				_ctrltype;
	std::string				_filename;
	std::string				_projectname;
	nana::msgbox::pick_t	_retval;
	//*>
};

#endif //NEW_PROJECT_H

