/*
 *		ctrls::spinbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_SPINBOX_H
#define NANA_CREATOR_SPINBOX_H

#include <nana/gui/widgets/spinbox.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class spinbox
		: public ctrl
	{
	public:
		spinbox(ctrl* parent, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::spinbox spn;

		int _def_begin;
		int _def_last;
		int _def_value;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_SPINBOX_H
