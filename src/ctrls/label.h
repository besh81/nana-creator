/*
 *		ctrls::label Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_LABEL_H
#define NANA_CREATOR_LABEL_H

#include <nana/gui/widgets/label.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class label
		: public ctrl
	{
	public:
		label(nana::window wd, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::label lbl;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_LABEL_H
