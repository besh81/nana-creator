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
		tabbar(ctrl* parent, const std::string& name);

		static void init_item(properties_collection& item);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::tabbar<size_t> tbb;

		bool internal_use{ false };
	};

}//end namespace ctrls

#endif //NANA_CREATOR_TABBAR_H
