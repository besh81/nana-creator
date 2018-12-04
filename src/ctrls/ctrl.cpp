/*
 *		ctrls::ctrl Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/ctrl.h"
#include "nana_extra/color_helper.h"
#include "style.h"


namespace ctrls
{

	//ctrl
	void ctrl::init(nana::widget* wdg, const std::string& type, const std::string& name, bool font)
	{
		nanawdg = wdg;
		_defaults.use_font = font;

		nana::API::ignore_mouse_focus(*nanawdg, false);
		nana::API::effects_edge_nimbus(*nanawdg, nana::effects::edge_nimbus::none);
		//nana::API::effects_edge_nimbus(*nanawdg, nana::effects::edge_nimbus::active);


		// mouse move
		nanawdg->events().mouse_move.connect_front([this](const nana::arg_mouse& arg)
		{
			if(_highlighted == highlight_mode::before_after)
			{
				auto pos = mouse_pos(arg.pos, nanawdg->size());
				if(pos != _cursor_pos)
				{
					_cursor_pos = pos;
					refresh();
				}
			}
		});

		// drawing
		nana::drawing dw{ *nanawdg };
		dw.draw([this](nana::paint::graphics& graph)
		{
			if(selected())
				draw_selection(graph);

			if(highlighted())
				draw_highlight(graph, _highlighted, _cursor_pos);
		});


		// save defaults values
		_defaults.bgcolor = nanawdg->bgcolor();
		_defaults.fgcolor = nanawdg->fgcolor();


		// properties - main
		properties.append("type") = type;
		properties.append("name") = name;
		// common
		properties.append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = nanawdg->enabled();
		// appearance
		properties.append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(nanawdg->bgcolor(), false);
		properties.append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(nanawdg->fgcolor(), false);
		// font
		if(_defaults.use_font)
		{
			_defaults.font = nanawdg->typeface();

			properties.append("f_size").label("Size").category(CAT_FONT).type(pg_type::string_uint) = static_cast<unsigned int>(_defaults.font.size(true));
			properties.append("f_weight").label("Weight").category(CAT_FONT).type(pg_type::string_uint) = _defaults.font.weight();
			properties.append("f_italic").label("Italic").category(CAT_FONT).type(pg_type::check) = _defaults.font.italic();
			properties.append("f_underline").label("Underline").category(CAT_FONT).type(pg_type::check) = _defaults.font.underline();
			properties.append("f_strike_out").label("Strike out").category(CAT_FONT).type(pg_type::check) = _defaults.font.strikeout();
		}
		// layout
		properties.append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_weight) = -1;
	}


	void ctrl::update()
	{
		auto pw = nana::API::get_widget(nanawdg->parent());
		bool inherited;
		nana::color col;

		nanawdg->enabled(properties.property("enabled").as_bool());

		// colors
		col = nana::to_color(properties.property("bgcolor").as_string(), inherited);
		nanawdg->bgcolor(inherited ? pw->bgcolor() : col);

		col = nana::to_color(properties.property("fgcolor").as_string(), inherited);
		nanawdg->fgcolor(inherited ? pw->fgcolor() : col);

		// font
		if(_defaults.use_font)
		{
			nana::paint::font::font_style f_s;
			f_s.weight = properties.property("f_weight").as_uint();
			f_s.italic = properties.property("f_italic").as_bool();
			f_s.underline = properties.property("f_underline").as_bool();
			f_s.strike_out = properties.property("f_strike_out").as_bool();

			nanawdg->typeface(nana::paint::font("", properties.property("f_size").as_uint(), f_s));
		}
	}


	void ctrl::refresh()
	{
		nana::API::refresh_window(*nanawdg);
	}


	std::string ctrl::get_type()
	{
		return properties.property("type").as_string();
	}


	std::string ctrl::get_weight()
	{
		return properties.property("weight").as_string();
	}


	void ctrl::connect_mouse_enter(std::function<void(const nana::arg_mouse&)> f)
	{
		if(nanawdg)
			nanawdg->events().mouse_enter.connect_front(f);
	}


	void ctrl::connect_mouse_leave(std::function<void(const nana::arg_mouse&)> f)
	{
		if(nanawdg)
			nanawdg->events().mouse_leave.connect_front(f);
	}


	void ctrl::connect_mouse_move(std::function<void(const nana::arg_mouse&)> f)
	{
		if(nanawdg)
			nanawdg->events().mouse_move.connect_front(f);
	}


	void ctrl::connect_mouse_down(std::function<void(const nana::arg_mouse&)> f)
	{
		if(nanawdg)
			nanawdg->events().mouse_down.connect_front(f);
	}


	void ctrl::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		std::string name = properties.property("name").as_string();
		// create
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");
		// placement
		cd->init.push_back(ci->place + "[\"" + ci->field + "\"] << " + name + ";");
		// init
		if(!properties.property("enabled").as_bool())
			cd->init.push_back(name + ".enabled(" + properties.property("enabled").as_string() + ");");

		generatecode_colors(cd, ci, name);

		// font
		if(_defaults.use_font)
		{
			auto f = nanawdg->typeface();

			if(_defaults.font.size(true) != f.size(true) || _defaults.font.weight() != f.weight()
				|| _defaults.font.italic() != f.italic() || _defaults.font.underline() != f.underline()
				|| _defaults.font.strikeout() != f.strikeout())
			{
				std::string font = name + ".typeface(nana::paint::font(\"\", " + properties.property("f_size").as_string() + ", {";
				font += properties.property("f_weight").as_string() + ", ";
				font += properties.property("f_italic").as_string() + ", ";
				font += properties.property("f_underline").as_string() + ", ";
				font += properties.property("f_strike_out").as_string() + "}));";

				cd->init.push_back(font);
			}
		}
	}


	void ctrl::generatecode_colors(code_data_struct* cd, code_info_struct* ci, const std::string& name)
	{
		auto pw = nana::API::get_widget(nanawdg->parent());
		bool inherited;
		std::string color_txt;
		nana::color color;

		// bg
		color_txt = properties.property("bgcolor").as_string();
		color = nana::to_color(color_txt, inherited);
		if(inherited)
		{
			// is inherited: set parent color
			if(name != "")
			{
				if(ci->create == "*this")
					cd->init.push_back(name + ".bgcolor(this->bgcolor());");
				else
					cd->init.push_back(name + ".bgcolor(" + ci->create + ".bgcolor());");
			}
		}
		else
		{
			// not inherited: color_txt is in the form R,G,B
			if(color != _defaults.bgcolor)
			{
				if(name == "")
					cd->init.push_back("bgcolor(nana::color(" + color_txt + "));");
				else
					cd->init.push_back(name + ".bgcolor(nana::color(" + color_txt + "));");
			}
		}

		// fg
		color_txt = properties.property("fgcolor").as_string();
		color = nana::to_color(color_txt, inherited);
		if(inherited)
		{
			// is inherited: set parent color
			if(name != "")
			{
				if(ci->create == "*this")
					cd->init.push_back(name + ".fgcolor(this->fgcolor());");
				else
					cd->init.push_back(name + ".fgcolor(" + ci->create + ".fgcolor());");
			}
		}
		else
		{
			// not inherited: color_txt is in the form R,G,B
			if(color != _defaults.fgcolor)
			{
				if(name == "")
					cd->init.push_back("fgcolor(nana::color(" + color_txt + "));");
				else
					cd->init.push_back(name + ".fgcolor(nana::color(" + color_txt + "));");
			}
		}
	}


	void ctrl::draw_highlight(nana::paint::graphics& graph, highlight_mode mode, mouse_position pos)
	{
		if(mode == highlight_mode::into)
			graph.blend({ 0, 0, graph.width(), graph.height() }, HIGHLIGHT_CTRL_COL, 0.5);
		else if(mode == highlight_mode::before_after)
		{
			if(pos == mouse_position::up_left || pos == mouse_position::down_left) //left
				graph.blend({ 0, 0, graph.width() / 2, graph.height() }, HIGHLIGHT_CTRL_COL, 0.5);
			else //right
				graph.blend({ static_cast<int>(graph.width()) / 2, 0, graph.width() / 2, graph.height() }, HIGHLIGHT_CTRL_COL, 0.5);
		}
	}


	void ctrl::draw_selection(nana::paint::graphics& graph)
	{
		graph.rectangle(false, SELECT_CTRL_COL);
		graph.rectangle({ 1, 1, SELECT_CTRL_SIZE, SELECT_CTRL_SIZE }, true, SELECT_CTRL_COL);
		graph.rectangle({ 1, static_cast<int>(graph.height()) - SELECT_CTRL_SIZE - 1, SELECT_CTRL_SIZE, SELECT_CTRL_SIZE }, true, SELECT_CTRL_COL);
		graph.rectangle({ static_cast<int>(graph.width()) - SELECT_CTRL_SIZE - 1, 1, SELECT_CTRL_SIZE, SELECT_CTRL_SIZE }, true, SELECT_CTRL_COL);
		graph.rectangle({ static_cast<int>(graph.width()) - SELECT_CTRL_SIZE - 1, static_cast<int>(graph.height()) - SELECT_CTRL_SIZE - 1, SELECT_CTRL_SIZE, SELECT_CTRL_SIZE }, true, SELECT_CTRL_COL);
	}
	
	
	mouse_position ctrl::mouse_pos(nana::point pos, nana::size size)
	{
		if(pos.y > size.height / 2)
		{
			if(pos.x > size.width / 2)
				return  mouse_position::up_right;
			else
				return  mouse_position::up_left;
		}
		else
		{
			if(pos.x > size.width / 2)
				return  mouse_position::down_right;
			else
				return  mouse_position::down_left;
		}
	}

}//end namespace ctrls
