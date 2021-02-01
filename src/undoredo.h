/*
 *		Undo/Redo
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_UNDOREDO_H
#define NANA_CREATOR_UNDOREDO_H

#include <string>
#include "pugixml/pugixml.hpp"


namespace undoredo
{
	enum class action
	{
		add,
		remove,
		move_up,
		move_down,
		move_into_field,
		move_into_grid,
		move_into_panel,
		change_name,
		change_property,
		change_items,
		empty
	};

	struct state
	{
		undoredo::action action{ undoredo::action::empty };
		std::string name;
		undoredo::action item_action{ undoredo::action::empty };
		std::string item_name;
		pugi::xml_document snapshot;
	};
}


#endif //NANA_CREATOR_UNDOREDO_H
