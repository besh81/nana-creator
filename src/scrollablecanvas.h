/*
 *		scrollablecanvas Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_SCROLLABLECANVAS_H
#define NANA_CREATOR_SCROLLABLECANVAS_H

#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/scroll.hpp>


class scrollablecanvas
	: public nana::panel<true>
{
public:
	scrollablecanvas(nana::window parent, bool visible = true);

	nana::window get_handle() const { return _inner.handle(); }

	bool haschild() const { return _child ? true : false; }
	nana::widget* child() { return _child; }

	bool add(nana::window child);
	bool remove();

	void refresh();

protected:
	nana::panel<false>	_inner{ *this };
	nana::scroll<true>	_scroll_v;
	nana::scroll<false>	_scroll_h;
	nana::point			_scroll_pos;

	nana::widget*		_child{ 0 };

	void adjust_scroll_life();
};


#endif //NANA_CREATOR_SCROLLABLECANVAS_H
