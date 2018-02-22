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
#include "codegenerator.h"


namespace ctrls
{

	class form
		: public ctrl
	{
	public:
		form(nana::window wd, const std::string& name, bool visible = true);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;

		bool append(nana::window child);
		bool remove(nana::window child);

		bool haschild() { return _child; }


	protected:
		nana::panel<true>	frm;
		nana::place			_place;

		bool				_child{ false };
	};

}//end namespace ctrls

#endif //NANA_CREATOR_FORM_H
