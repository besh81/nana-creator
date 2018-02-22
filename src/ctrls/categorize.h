/*
 *		ctrls::categorize Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_CATEGORIZE_H
#define NANA_CREATOR_CATEGORIZE_H

#include <nana/gui/widgets/categorize.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class categorize
		: public ctrl
	{
	public:
		categorize(nana::window wd, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::categorize<int> ctg;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_CATEGORIZE_H
