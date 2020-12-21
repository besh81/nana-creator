/*
 *		ctrls::treebox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_TREEBOX_H
#define NANA_CREATOR_TREEBOX_H

#include <nana/gui/widgets/treebox.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class treebox
		: public ctrl
	{
	public:
		treebox(ctrl* parent, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::treebox trb;

		nana::drawerbase::treebox::scheme def_scheme;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_TREEBOX_H
