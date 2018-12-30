/*
 *		ctrls::combox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_COMBOX_H
#define NANA_CREATOR_COMBOX_H

#include <nana/gui/widgets/combox.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class combox
		: public ctrl
	{
	public:
		combox(ctrl* parent, const std::string& name);

		static void init_item(properties_collection& item);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::combox cmb;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_COMBOX_H
