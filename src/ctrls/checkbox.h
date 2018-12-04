/*
 *		ctrls::checkbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_CHECKBOX_H
#define NANA_CREATOR_CHECKBOX_H

#include <nana/gui/widgets/checkbox.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class checkbox
		: public ctrl
	{
	public:
		checkbox(ctrl* parent, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::checkbox chk;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_CHECKBOX_H
