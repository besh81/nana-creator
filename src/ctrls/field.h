/*
 *		ctrls::field Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_FIELD_H
#define NANA_CREATOR_FIELD_H

#include <nana/gui/widgets/panel.hpp>
#include "ctrls/ctrl.h"
#include "ctrls/box_model.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class field
		: public ctrl
	{
	public:
		field(nana::window wd, const std::string& name, bool grid = false);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;

		std::string get_divtext();
		void update_children_info(nana::window child, const std::string& divtext, const std::string& weight);

		bool children();
		bool children_fields();
		bool append(nana::window ctrl);
		bool insert(nana::window pos, nana::window ctrl, bool after = true);
		bool remove(nana::window ctrl);

		bool moveup(nana::window ctrl);
		bool movedown(nana::window ctrl);


	protected:
		nana::panel<true>	fld;
		box_model			boxmodel;

		bool				_grid;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_FIELD_H
