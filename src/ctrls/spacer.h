/*
 *		ctrls::spacer Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_SPACER_H
#define NANA_CREATOR_SPACER_H

#include <nana/gui/widgets/panel.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class spacer
		: public ctrl
	{
	public:
		spacer(nana::window wd, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::panel<true>	pnl;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_SPACER_H
