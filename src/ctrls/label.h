/*
 *		ctrls::label Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_LABEL_H
#define NANA_CREATOR_LABEL_H

#include <nana/gui/widgets/label.hpp>
#include "ctrls/property.h"


namespace ctrls
{

	class label
		: public nana::label
	{
	public:
		label(nana::window wd, properties_collection* properties, const std::string& name);

		void update(properties_collection* properties);

		void generatecode(properties_collection* properties, std::vector<std::string>* decl, std::vector<std::string>* init);


	protected:
		void _initproperties(properties_collection* properties, const std::string& name);
	};

}//end namespace ctrls

#endif //NANA_CREATOR_LABEL_H
