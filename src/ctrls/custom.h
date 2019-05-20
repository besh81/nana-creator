/*
 *		ctrls::custom Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_CUSTOM_H
#define NANA_CREATOR_CUSTOM_H

#include <nana/gui/widgets/panel.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class custom
		: public ctrl
	{
	public:
		custom(ctrl* parent, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


	protected:
		nana::panel<true> cst;

		std::string parse_code(const std::string& code, const std::string& name, const std::string& parent);
	};

}//end namespace ctrls

#endif //NANA_CREATOR_CUSTOM_H
