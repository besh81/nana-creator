/*
 *		ctrls::group Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_GROUP_H
#define NANA_CREATOR_GROUP_H

#include <nana/gui/widgets/group.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class group
		: public ctrl
	{
	public:
		group(nana::window wd, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::group grp;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_GROUP_H
