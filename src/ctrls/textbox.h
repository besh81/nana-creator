/*
 *		ctrls::textbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_TEXTBOX_H
#define NANA_CREATOR_TEXTBOX_H

#include <nana/gui/widgets/textbox.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class textbox
		: public ctrl
	{
	public:
		textbox(ctrl* parent, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::textbox txt;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_TEXTBOX_H
