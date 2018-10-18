/*
 *		ctrls::splitterbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_SPLITTERBAR_H
#define NANA_CREATOR_SPLITTERBAR_H

#include <nana/gui/widgets/panel.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class splitterbar
		: public ctrl
	{
	public:
		splitterbar(nana::window wd, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;

		std::string get_weight();
		std::string get_divtext();

	protected:
		nana::panel<true>	pnl;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_SPLITTERBAR_H
