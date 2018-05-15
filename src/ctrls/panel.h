/*
 *		ctrls::panel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PANEL_H
#define NANA_CREATOR_PANEL_H

#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/place.hpp>
#include "ctrls/ctrl.h"
#include "ctrls/box_model.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class panel
		: public ctrl
	{
	public:
		panel(nana::window wd, const std::string& name, bool mainclass = false, bool visible = true);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;

		void updatefield(nana::window ctrl, const std::string& weight, const std::string& margin);

		bool children();
		bool append(nana::window ctrl);
		bool insert(nana::window pos, nana::window ctrl, bool after = true);
		bool remove(nana::window ctrl);

		bool moveup(nana::window ctrl);
		bool movedown(nana::window ctrl);


	protected:
		nana::panel<true>	pnl;
		box_model			boxmodel;
	
		bool				_mainclass{ false };
	};

}//end namespace ctrls

#endif //NANA_CREATOR_PANEL_H
