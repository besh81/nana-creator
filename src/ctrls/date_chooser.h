/*
 *		ctrls::date_chooser Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_DATECHOOSER_H
#define NANA_CREATOR_DATECHOOSER_H

#include <nana/gui/widgets/date_chooser.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class date_chooser
		: public ctrl
	{
	public:
		date_chooser(ctrl* parent, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::date_chooser dtc;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_DATECHOOSER_H
