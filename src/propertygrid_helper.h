/*
 *		propertygrid_helper Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PROPERTYGRID_HELPER_H
#define NANA_CREATOR_PROPERTYGRID_HELPER_H

#include "nana_extra/propertygrid.h"
#include "ctrls/property.h"


namespace propertygrid_helper
{

	void append(nana::propertygrid* propgrid, ctrls::properties_collection* properties, std::vector<ctrls::properties_collection>* items);

	void enabled_bonds(nana::propertygrid* propgrid, ctrls::properties_collection* properties, const std::string& name, bool value);

}//end namespace propertygrid_helper


#endif //NANA_CREATOR_PROPERTYGRID_HELPER_H
