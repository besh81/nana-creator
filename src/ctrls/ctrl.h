/*
 *		ctrls::ctrl_struct Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_CTRL_H
#define NANA_CREATOR_CTRL_H

#include "config.h"
#include <memory>
#include <nana/gui/wvl.hpp>
#include "ctrls/property.h"
#include "codegenerator_data.h"


namespace ctrls
{
	enum class highlight_mode
	{
		none,
		before_after,
		into
	};

	enum class mouse_position
	{
		up_left,
		up_right,
		down_left,
		down_right
	};


	class ctrl
	{
	public:
		ctrl(ctrl* parent)
			: _parent(parent)
		{ }


		// public vars
		properties_collection				properties;
		std::vector<properties_collection>	items;
		nana::widget*						nanawdg;

		// relationship management
		ctrl* parent() { return _parent; }
		//
		virtual bool append(ctrl* child) { return false; }
		virtual bool insert(ctrl* child, ctrl* pos, bool after) { return false; }
		virtual bool remove(ctrl* child) { return false; }
		//
		virtual bool moveup(ctrl* child) { return false; }
		virtual bool movedown(ctrl* child) { return false; }
		//
		virtual bool children() { return false; }
		virtual bool children_fields() { return false; }


		// items management
		virtual properties_collection& append_item();


		// creator modifiers
		virtual void update();
		virtual void refresh();
		//
		bool highlighted() { return _highlighted == highlight_mode::none ? false : true; }
		virtual void highlight(highlight_mode mode) { _highlighted = mode; }
		virtual highlight_mode highlight() { return _highlighted; }
		//
		bool selected() { return _selected; }
		virtual void select(bool state) { _selected = state; }
		//
		virtual std::string get_type();
		virtual std::string get_weight();
		virtual std::string get_divtext() { return ""; }
		//
		mouse_position		get_cursor_pos() { return _cursor_pos; }


		// mouse events
		virtual void connect_mouse_enter(std::function<void(const nana::arg_mouse&)> f);
		virtual void connect_mouse_leave(std::function<void(const nana::arg_mouse&)> f);
		virtual void connect_mouse_move(std::function<void(const nana::arg_mouse&)> f);
		virtual void connect_mouse_down(std::function<void(const nana::arg_mouse&)> f);


		// code generation
		virtual void generatecode(code_data_struct* cd, code_info_struct* ci);
		void generatecode_colors(code_data_struct* cd, code_info_struct* ci, const std::string& name = "");


	protected:
		void init(nana::widget* wdg, const std::string& type, const std::string& name, bool font = true);

		// drawing functions
		void draw_highlight(nana::paint::graphics& graph, highlight_mode mode, mouse_position pos);
		void draw_selection(nana::paint::graphics& graph);
		
		// mouse position inside the control
		mouse_position mouse_pos(nana::point pos, nana::size size);


		ctrl*				_parent{ 0 };

		highlight_mode		_highlighted{ highlight_mode::none };
		mouse_position		_cursor_pos{ mouse_position::up_left };
		bool				_selected{ false };

		struct def_values
		{
			nana::color			bgcolor;
			nana::color			fgcolor;
			nana::paint::font	font;
			bool				use_font{ true };
		};
		def_values			_defaults;
	};

}//end namespace ctrls


#endif //NANA_CREATOR_CTRL_H
