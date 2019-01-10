/*
 *		ctrls::slider Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_SLIDER_H
#define NANA_CREATOR_SLIDER_H

#include <nana/gui/widgets/slider.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class slider
		: public ctrl
	{
	public:
		slider(ctrl* parent, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::slider sld;

		unsigned int _def_maximum;
		unsigned int _def_value;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_SLIDER_H
