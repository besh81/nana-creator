/*
 *		ctrls::form Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_FORM_H
#define NANA_CREATOR_FORM_H

#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/place.hpp>
#include "ctrls/ctrl.h"
#include "ctrls/box_model.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class form
		: public ctrl
	{
	public:
		form(nana::window wd, const std::string& name, bool visible = true);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;

		void updatefield(nana::window child, const std::string& weight, const std::string& margin);

		bool children();
		bool append(nana::window child);
		bool remove(nana::window child);

		bool moveup(nana::window child);
		bool movedown(nana::window child);


	protected:
		nana::panel<true>	frm;
		box_model			boxmodel;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_FORM_H
