/*
 *		ctrls::button Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_BUTTON_H
#define NANA_CREATOR_BUTTON_H

#include <nana/gui/widgets/button.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator.h"


namespace ctrls
{

	class button
		: public ctrl
	{
	public:
		button(nana::window wd, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::button btn;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_BUTTON_H
