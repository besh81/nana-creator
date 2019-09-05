/*****************************************************
 *	C++ code generated with Nana Creator (0.20.0)
 *	GitHub repo: https://github.com/besh81/nana-creator
 *
 * PLEASE EDIT ONLY INSIDE THE TAGS:
 *		//<*tag
 *			user code
 *		//*>
*****************************************************/

#ifndef CREATOR_H
#define CREATOR_H

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/widgets/label.hpp>

//<*includes
#include <nana/gui/widgets/panel.hpp>
#include "nana_extra/adi_place.hpp"
//*>


class creator
	: public nana::form
{
public:
	creator(nana::window wd, const ::nana::size& sz = {800, 500}, const nana::appearance& apr = {true, true, false, false, true, true, true})
		: nana::form(wd, sz, apr)
	{
		init_();

		//<*ctor
		_init_ctrls();
		//*>
	}

	~creator()
	{
		//<*dtor
		_destroy_ctrls();
		//*>
	}


private:
	void init_()
	{
		//_place.div("vert <weight=30 toolbar><<vert weight=20% <assets>|<objects>>|<weight=55% canvas>|<properties>><weight=24 margin=3 statusbar>");
		_place.div("vert <weight=30 toolbar><adi_panel><weight=24 margin=3 statusbar>");
		caption("Nana Creator");
		// _tb
		_tb.create(*this);
		_place["toolbar"] << _tb;
		_tb.scale(21);
		_tb.append("New project", nana::paint::image("icons/new_prj.png"));
		_tb.append("Load project", nana::paint::image("icons/load_prj.png"));
		_tb.append("Save project", nana::paint::image("icons/save.png"));
		_tb.append("Save project as...", nana::paint::image("icons/save_as.png"));
		_tb.separate();
		_tb.append("Generate C++ code", nana::paint::image("icons/output_on.png"));
		_tb.separate();
		_tb.append("Delete current selection", nana::paint::image("icons/delete.png"));
		_tb.separate();
		_tb.append("Move up current selection", nana::paint::image("icons/up.png"));
		_tb.append("Move down current selection", nana::paint::image("icons/down.png"));
		_tb.separate();
		_tb.append("Cut current selection", nana::paint::image("icons/cut.png"));
		_tb.append("Copy current selection", nana::paint::image("icons/copy.png"));
		_tb.append("Paste into/after current selection", nana::paint::image("icons/paste.png"));
		_tb.go_right();
		_tb.append("About...", nana::paint::image("icons/info.png"));
		// _sb
		_sb.create(*this);
		_place["statusbar"] << _sb;
		_sb.caption("");

		_place.collocate();
	}


protected:
	nana::place _place{ *this };
	nana::toolbar _tb;
	nana::label _sb;


	//<*declarations
public:
	void enableGUI(bool state, bool new_load = false);
	bool load_xml(const std::string& filename);
	bool save_xml(const std::string& filename);

	bool generate_cpp();

	void sb_clear();
	void sb_set(const std::string& str);

private:
	void _init_ctrls();
	void _destroy_ctrls();

	nana::panel<true>	_adi_panel;
	nana::adi_place		_adi_place;
	//*>
};

#endif //CREATOR_H

