/*****************************************************
 *	C++ code generated with Nana Creator (0.31.0)
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
		place_.bind(*this);
		place_.div("vert arrange=[36,variable,24] _field_");
		caption("Nana Creator");
		icon(nana::paint::image("icons/creator.ico"));
		bgcolor(nana::color(93,107,153));
		// _tb
		_tb.create(*this);
		place_["_field_"] << _tb;
		_tb.bgcolor(nana::color(204,213,240));
		_tb.tools_height(26);
		_tb.append(nana::toolbar::tools::button, "New project", nana::paint::image("icons/new_prj.png"));
		_tb.append(nana::toolbar::tools::button, "Load project", nana::paint::image("icons/load_prj.png"));
		_tb.append(nana::toolbar::tools::button, "Save project", nana::paint::image("icons/save.png"));
		_tb.append(nana::toolbar::tools::button, "Save project as...", nana::paint::image("icons/save_as.png"));
		_tb.append_separator();
		_tb.append(nana::toolbar::tools::button, "Undo", nana::paint::image("icons/undo.png"));
		_tb.append(nana::toolbar::tools::button, "Redo", nana::paint::image("icons/redo.png"));
		_tb.append_separator();
		_tb.append(nana::toolbar::tools::button, "Generate code", nana::paint::image("icons/output.png")).textout(true);
		_tb.append_separator();
		_tb.append(nana::toolbar::tools::button, "Delete current selection", nana::paint::image("icons/delete.png"));
		_tb.append_separator();
		_tb.append(nana::toolbar::tools::button, "Move up current selection", nana::paint::image("icons/up.png"));
		_tb.append(nana::toolbar::tools::button, "Move down current selection", nana::paint::image("icons/down.png"));
		auto ip__tb_14 = _tb.append(nana::toolbar::tools::dropdown, "Move into", nana::paint::image("icons/into.png")).textout(true);
		ip__tb_14.dropdown_append("field", nana::paint::image("icons/horizontal_layout.png"));
		ip__tb_14.dropdown_append("grid", nana::paint::image("icons/grid_layout.png"));
		ip__tb_14.dropdown_append("panel", nana::paint::image("icons/panel.png"));
		_tb.append_separator();
		_tb.append(nana::toolbar::tools::button, "Cut current selection", nana::paint::image("icons/cut.png"));
		_tb.append(nana::toolbar::tools::button, "Copy current selection", nana::paint::image("icons/copy.png"));
		_tb.append(nana::toolbar::tools::button, "Paste into/after current selection", nana::paint::image("icons/paste.png"));
		_tb.go_right();
		_tb.append(nana::toolbar::tools::button, "Options", nana::paint::image("icons/settings.png"));
		_tb.append(nana::toolbar::tools::button, "Info", nana::paint::image("icons/info.png"));
		// _adi_panel_margin
		_adi_panel_margin.create(*this);
		_adi_panel_margin_place_.bind(_adi_panel_margin);
		_adi_panel_margin_place_.div("margin=5 gap=2 _field_");
		_adi_panel_margin.bgcolor(this->bgcolor());
		place_["_field_"] << _adi_panel_margin;
		// _adi_panel
		_adi_panel.create(_adi_panel_margin);
		_adi_panel.bgcolor(_adi_panel_margin.bgcolor());
		_adi_panel_margin_place_["_field_"] << _adi_panel;
		// _sb_panel
		_sb_panel.create(*this);
		_sb_panel_place_.bind(_sb_panel);
		_sb_panel_place_.div("weight=24 margin=3 gap=2 _field_");
		_sb_panel.bgcolor(nana::color(64,86,141));
		place_["_field_"] << _sb_panel;
		// _sb
		_sb.create(_sb_panel);
		_sb_panel_place_["_field_"] << _sb;
		_sb.bgcolor(_sb_panel.bgcolor());
		_sb.fgcolor(nana::color(255,255,255));
		_sb.caption("");

		place_.collocate();
		_adi_panel_margin_place_.collocate();
		_sb_panel_place_.collocate();
	}


protected:
	nana::place place_;
	nana::toolbar _tb;
	nana::panel<true> _adi_panel_margin;
	nana::place _adi_panel_margin_place_;
	nana::panel<true> _adi_panel;
	nana::panel<true> _sb_panel;
	nana::place _sb_panel_place_;
	nana::label _sb;


	//<*declarations
public:
	void enable_gui(bool state, bool new_load, bool undo, bool redo);
	bool load_xml(const std::string& filename);
	bool save_xml(const std::string& filename);

	bool generate_cpp();

	void sb_clear();
	void sb_set(const std::string& str);

	void update_window_caption();

private:
	void _init_ctrls();
	void _destroy_ctrls();

	nana::adi_place		_adi_place;
	//*>
};

#endif //CREATOR_H

