/*
 *		objectspanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_OBJECTSPANEL_H
#define NANA_CREATOR_OBJECTSPANEL_H

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/menu.hpp>


class objectspanel
	: public nana::panel<true>
{
public:
	objectspanel(nana::window wd, bool visible = true);
	~objectspanel() {}

	bool append(const std::string& parent, const std::string& name, const std::string& type);
	bool clear();

	bool select(const std::string& name);

	void emit_events(bool state)
	{
		_evt_emit = state;
	}

	void auto_draw(bool state)
	{
		_objects.auto_draw(state);
	}

	void refresh()
	{
		nana::API::refresh_window(_objects);
	}

private:
	nana::place		_place{ *this };

	nana::label		_caption{ *this };
	nana::treebox	_objects{ *this };

	nana::menu		_ctxmenu;

	std::vector<nana::treebox::item_proxy> _roots;

	bool			_evt_emit{ true };
};

#endif //NANA_CREATOR_OBJECTSPANEL_H
