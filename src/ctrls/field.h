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
		field(ctrl* parent, const std::string& name, bool grid = false);

		// relationship management
		bool append(ctrl* child) override { return boxmodel.append(child); }
		bool insert(ctrl* child, ctrl* pos, bool after) override { return boxmodel.insert(child, pos, after); }
		bool remove(ctrl* child) override { return boxmodel.remove(child); }
		//
		bool moveup(ctrl* child) override { return boxmodel.moveup(child); }
		bool movedown(ctrl* child) override { return boxmodel.movedown(child); }
		//
		bool children() override { return boxmodel.children(); }
		bool children_fields() override { return boxmodel.children_fields(); }


		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;

		std::string get_divtext() override;


	protected:
		nana::panel<true>	fld;
		box_model			boxmodel;

		bool				_grid;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_FIELD_H
