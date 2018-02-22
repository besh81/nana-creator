/*
 *		ctrls::toolbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_TOOLBAR_H
#define NANA_CREATOR_TOOLBAR_H

#include <nana/gui/widgets/toolbar.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class toolbar
		: public ctrl
	{
	public:
		toolbar(nana::window wd, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::toolbar tlb;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_TOOLBAR_H
