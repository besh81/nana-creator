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

	class ctrl
	{
	public:
		properties_collection	properties;
		nana::widget*			nanawdg;


		ctrl() = default;

		virtual void update();

		virtual void generatecode(code_data_struct* cd, code_info_struct* ci);
		void generatecode_colors(code_data_struct* cd, code_info_struct* ci, const std::string& name = "");

		virtual std::string get_divtext() { return ""; }
		virtual std::string get_weight();
		virtual void update_children_info(nana::window child, const std::string& divtext, const std::string& weight) { return; }

		virtual bool children() { return false; }
		virtual bool children_fields() { return false; }
		virtual bool append(nana::window ctrl) { return false; }
		virtual bool insert(nana::window pos, nana::window ctrl, bool after = true) { return false; }
		virtual bool remove(nana::window ctrl) { return false; }

		virtual bool moveup(nana::window ctrl) { return false; }
		virtual bool movedown(nana::window ctrl) { return false; }

		bool highlighted() { return _highlighted; }
		virtual void set_highlight() { _highlighted = true; }
		virtual void reset_highlight() { _highlighted = false; }

		bool selected() { return _selected; }
		virtual void set_select() { _selected = true; }
		virtual void reset_select() { _selected = false; }


	protected:
		void init(nana::widget* wdg, const std::string& type, const std::string& name);

		bool			_highlighted{ false };
		bool			_selected{ false };
	};

}//end namespace ctrls


typedef std::shared_ptr<ctrls::ctrl>	control_obj;
typedef std::weak_ptr<ctrls::ctrl>		control_obj_ptr;


#endif //NANA_CREATOR_CTRL_H
