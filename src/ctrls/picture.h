/*
 *		ctrls::picture Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PICTURE_H
#define NANA_CREATOR_PICTURE_H

#include <nana/gui/widgets/picture.hpp>
#include "ctrls/property.h"
#include "codegenerator.h"


namespace ctrls
{

	class picture
		: public nana::picture
	{
	public:
		picture(nana::window wd, properties_collection* properties, const std::string& name);

		void update(properties_collection* properties);

		void generatecode(properties_collection* properties, code_data_struct* cd, code_info_struct* ci);


	protected:
		void _initproperties(properties_collection* properties, const std::string& name);
	};

}//end namespace ctrls

#endif //NANA_CREATOR_PICTURE_H
