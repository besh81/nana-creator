/*
 *		ctrls::progress Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PROGRESS_H
#define NANA_CREATOR_PROGRESS_H

#include <nana/gui/widgets/progress.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class progress
		: public ctrl
	{
	public:
		progress(ctrl* parent, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::progress prg;

		unsigned int _def_amount;
		unsigned int _def_value;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_PROGRESS_H
