/*****************************************************
 *	C++ code generated with Nana Creator (0.31.0)
 *	GitHub repo: https://github.com/besh81/nana-creator
 *
 * PLEASE EDIT ONLY INSIDE THE TAGS:
 *		//<*tag
 *			user code
 *		//*>
*****************************************************/

#ifndef OPTIONS_H
#define OPTIONS_H

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/button.hpp>

//<*includes

//*>


class options
	: public nana::form
{
public:
	options(nana::window wd, const ::nana::size& sz = {450, 260}, const nana::appearance& apr = {true, true, false, false, false, false, false})
		: nana::form(wd, sz, apr)
	{
		init_();

		//<*ctor
		init();
		//*>
	}

	~options()
	{
		//<*dtor

		//*>
	}


private:
	void init_()
	{
		place_.bind(*this);
		place_.div("vert margin=[5,5,5,5] <weight=34 margin=[5,5,5,5] gap=2 arrange=[180,60] field1><margin=[5,0,0,0] gap=2 field2><weight=36 margin=[5,5,5,5] gap=5 arrange=[variable,90,90] field3>");
		caption("Options");
		icon(nana::paint::image("icons/creator.ico"));
		bgcolor(nana::color(204,213,240));
		// label1
		label1.create(*this);
		place_["field1"] << label1;
		label1.bgcolor(nana::color(204,213,240));
		label1.caption("Undo/Redo queue length");
		label1.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// ur_spin
		ur_spin.create(*this);
		place_["field1"] << ur_spin;
		ur_spin.range(0, 99999, 1);
		ur_spin.value("30");
		// group1
		group1.create(*this);
		group1.div("vert margin=[5,5,5,5] gap=2 _field_");
		group1.caption("Directories (read only)");
		place_["field2"] << group1;
		// label2
		label2.create(group1);
		group1["_field_"] << label2;
		label2.caption("New projects");
		label2.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(2));
		// new_txt
		new_txt.create(group1);
		group1["_field_"] << new_txt;
		new_txt.editable(false);
		new_txt.multi_lines(false);
		// label21
		label21.create(group1);
		group1["_field_"] << label21;
		label21.caption("Load projects");
		label21.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(2));
		// load_txt
		load_txt.create(group1);
		group1["_field_"] << load_txt;
		load_txt.editable(false);
		load_txt.multi_lines(false);
		// label22
		label22.create(group1);
		group1["_field_"] << label22;
		label22.caption("Images");
		label22.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(2));
		// image_txt
		image_txt.create(group1);
		group1["_field_"] << image_txt;
		image_txt.editable(false);
		image_txt.multi_lines(false);
		// panel1
		panel1.create(*this);
		panel1.transparent(true);
		place_["field3"] << panel1;
		// ok_btn
		ok_btn.create(*this);
		place_["field3"] << ok_btn;
		ok_btn.caption("OK");
		// canc_btn
		canc_btn.create(*this);
		place_["field3"] << canc_btn;
		canc_btn.caption("Cancel");

		place_.collocate();
		group1.collocate();
	}


protected:
	nana::place place_;
	nana::label label1;
	nana::spinbox ur_spin;
	nana::group group1;
	nana::label label2;
	nana::textbox new_txt;
	nana::label label21;
	nana::textbox load_txt;
	nana::label label22;
	nana::textbox image_txt;
	nana::panel<true> panel1;
	nana::button ok_btn;
	nana::button canc_btn;


	//<*declarations
protected:
	void init();
	//*>
};

#endif //OPTIONS_H

