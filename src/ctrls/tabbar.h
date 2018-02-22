/*
 *		ctrls::tabbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_TABBAR_H
#define NANA_CREATOR_TABBAR_H

#include <nana/gui/widgets/tabbar.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class tabbar
		: public ctrl
	{
	public:
		tabbar(nana::window wd, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::tabbar<int> tbb;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_TABBAR_H
