/*
 *		ctrls::ctrl_struct Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_CTRL_H
#define NANA_CREATOR_CTRL_H

#include "config.h"
#include <memory>
#include <nana/gui/wvl.hpp>
#include "ctrls/property.h"


namespace ctrls
{

	struct ctrl_struct
	{
		properties_collection		properties;
		std::unique_ptr<nana::widget>	nanawdg;
	};

}//end namespace ctrls


typedef std::shared_ptr<ctrls::ctrl_struct>	control_struct;
typedef std::weak_ptr<ctrls::ctrl_struct>	control_struct_ptr;


#endif //NANA_CREATOR_CTRL_H
