/*
 *		ctrls::picture Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PICTURE_H
#define NANA_CREATOR_PICTURE_H

#include <nana/gui/widgets/picture.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class picture
		: public ctrl
	{
	public:
		picture(ctrl* parent, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::picture pct;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_PICTURE_H
