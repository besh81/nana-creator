/*
 *		ctrls::panel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PANEL_H
#define NANA_CREATOR_PANEL_H

#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/place.hpp>
#include "ctrls/property.h"


namespace ctrls
{

	class panel
		: public nana::panel<true>
	{
	public:
		panel(nana::window wd, properties_collection* properties, const std::string& name);

		void update(properties_collection* properties);

		void generatecode(properties_collection* properties, std::vector<std::string>* decl, std::vector<std::string>* init);

		void prepareforinsert();
		bool append(nana::window child);
		bool remove(nana::window child);

		bool haschild() { return _child; }


	protected:
		void _initproperties(properties_collection* properties, const std::string& name);

		bool			_child{ false };
		bool			_is_highlighted{ false };
		nana::color		_bgcolor;

		nana::place		_place{ *this };
	};

}//end namespace ctrls

#endif //NANA_CREATOR_PANEL_H
