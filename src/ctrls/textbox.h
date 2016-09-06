/*
 *		ctrls::textbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_TEXTBOX_H
#define NANA_CREATOR_TEXTBOX_H

#include <nana/gui/widgets/textbox.hpp>
#include "ctrls/property.h"
#include "codegenerator.h"


namespace ctrls
{

	class textbox
		: public nana::textbox
	{
	public:
		textbox(nana::window wd, properties_collection* properties, const std::string& name);

		void update(properties_collection* properties);

		void generatecode(properties_collection* properties, code_struct* cc);


	protected:
		void _initproperties(properties_collection* properties, const std::string& name);
	};

}//end namespace ctrls

#endif //NANA_CREATOR_TEXTBOX_H
