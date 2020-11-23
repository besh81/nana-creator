/*****************************************************
 *	C++ code generated with Nana Creator (0.26.0)
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
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/label.hpp>

//<*includes
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
		_place.div("vert arrange=[30,variable,24] _field_");
		caption("Nana Creator");
		bgcolor(nana::color(93,107,153));
		// _tb
		_tb.create(*this);
		_place["_field_"] << _tb;
		_tb.bgcolor(nana::color(204,213,240));
		_tb.scale(21);
		_tb.append("New project", nana::paint::image("icons/new_prj.png"));
		_tb.append("Load project", nana::paint::image("icons/load_prj.png"));
		_tb.append("Save project", nana::paint::image("icons/save.png"));
		_tb.append("Save project as...", nana::paint::image("icons/save_as.png"));
		_tb.separate();
		_tb.append("Generate code", nana::paint::image("icons/output.png")).textout(true);
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
		_tb.append("Info...", nana::paint::image("icons/info.png")).textout(true);
		// _adi_panel_margin
		_adi_panel_margin.create(*this);
		_adi_panel_margin_place.bind(_adi_panel_margin);
		_adi_panel_margin_place.div("margin=5 gap=2 _field_");
		_adi_panel_margin.bgcolor(this->bgcolor());
		_place["_field_"] << _adi_panel_margin;
		// _adi_panel
		_adi_panel.create(_adi_panel_margin);
		_adi_panel.bgcolor(_adi_panel_margin.bgcolor());
		_adi_panel_margin_place["_field_"] << _adi_panel;
		// _sb_panel
		_sb_panel.create(*this);
		_sb_panel_place.bind(_sb_panel);
		_sb_panel_place.div("weight=24 margin=3 gap=2 _field_");
		_sb_panel.bgcolor(nana::color(64,86,141));
		_place["_field_"] << _sb_panel;
		// _sb
		_sb.create(_sb_panel);
		_sb_panel_place["_field_"] << _sb;
		_sb.bgcolor(_sb_panel.bgcolor());
		_sb.fgcolor(nana::color(255,255,255));
		_sb.caption("");

		_place.collocate();
		_adi_panel_margin_place.collocate();
		_sb_panel_place.collocate();
	}


protected:
	nana::place _place{ *this };
	nana::toolbar _tb;
	nana::panel<true> _adi_panel_margin;
	nana::place _adi_panel_margin_place;
	nana::panel<true> _adi_panel;
	nana::panel<true> _sb_panel;
	nana::place _sb_panel_place;
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

	nana::adi_place		_adi_place;
	//*>
};

#endif //CREATOR_H

