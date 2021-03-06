/*****************************************************
 *	C++ code generated with Nana Creator (0.31.0)
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
		place_.bind(*this);
		place_.div("vert margin=5 <weight=30 gap=2 field1><margin=5 gap=5 arrange=[180,variable] field2><weight=36 margin=5 gap=3 arrange=[100,variable] field3><weight=36 margin=5 gap=3 arrange=[100,variable,50] field4><weight=36 margin=[5,5,5,5] gap=5 arrange=[variable,90,90] field5>");
		caption("New project");
		icon(nana::paint::image("icons/creator.ico"));
		bgcolor(nana::color(204,213,240));
		// label3
		label3.create(*this);
		place_["field1"] << label3;
		label3.caption("   Type:");
		label3.transparent(true);
		label3.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(2));
		// type_lst
		type_lst.create(*this);
		place_["field2"] << type_lst;
		type_lst.enable_single(true, true);
		type_lst.show_header(false);
		type_lst.append_header("Widget", 150);
		// type_txt
		type_txt.create(*this);
		place_["field2"] << type_txt;
		type_txt.editable(false);
		type_txt.line_wrapped(true);
		// label1
		label1.create(*this);
		place_["field3"] << label1;
		label1.bgcolor(nana::color(212,208,200));
		label1.caption("Name:");
		label1.transparent(true);
		label1.text_align(static_cast<nana::align>(2), static_cast<nana::align_v>(1));
		// name_txt
		name_txt.create(*this);
		place_["field3"] << name_txt;
		name_txt.multi_lines(false);
		// label2
		label2.create(*this);
		place_["field4"] << label2;
		label2.bgcolor(nana::color(212,208,200));
		label2.caption("Create in:");
		label2.transparent(true);
		label2.text_align(static_cast<nana::align>(2), static_cast<nana::align_v>(1));
		// folder_txt
		folder_txt.create(*this);
		place_["field4"] << folder_txt;
		folder_txt.editable(false);
		folder_txt.multi_lines(false);
		// folder_btn
		folder_btn.create(*this);
		place_["field4"] << folder_btn;
		folder_btn.bgcolor(this->bgcolor());
		folder_btn.caption("...");
		// panel1
		panel1.create(*this);
		panel1.bgcolor(nana::color(212,208,200));
		panel1.transparent(true);
		place_["field5"] << panel1;
		// create_btn
		create_btn.create(*this);
		place_["field5"] << create_btn;
		create_btn.bgcolor(this->bgcolor());
		create_btn.caption("Create");
		// canc_btn
		canc_btn.create(*this);
		place_["field5"] << canc_btn;
		canc_btn.bgcolor(this->bgcolor());
		canc_btn.caption("Cancel");

		place_.collocate();
	}


protected:
	nana::place place_;
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

