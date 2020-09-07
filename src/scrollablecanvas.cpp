/*
 *		scrollablecanvas Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "scrollablecanvas.h"
#include <nana/gui/drawing.hpp>
#include "style.h"


scrollablecanvas::scrollablecanvas(nana::window parent, bool visible)
	: nana::panel<true>(parent, nana::rectangle(), visible)
{
	bgcolor(CREATOR_MAIN_BG);

	// drawing
	nana::drawing dw{ *this };
	dw.draw([this](nana::paint::graphics & graph)
		{
			// draw border
			graph.rectangle(false, static_cast<::nana::color_rgb>(0x9cb6c5));
		});


	events().resized([this](const nana::arg_resized & arg)
	{
		if(haschild())
			adjust_scroll_life();
	});
}


bool scrollablecanvas::add(nana::window child)
{
	if(haschild())
		return false; // already has a child

	_child = nana::API::get_widget(child);
	return true;
}


bool scrollablecanvas::remove()
{
	if(!haschild())
		return false; // no child to remove

	_child->move(nana::rectangle(0, 0, 5, 5));
	_child = 0;
	return true;
}


void scrollablecanvas::refresh()
{
	adjust_scroll_life();
	nana::API::refresh_window(*this);
}


#define SCROLL_SCALE		16

void scrollablecanvas::adjust_scroll_life()
{
	::nana::internal_scope_guard lock;

	//The area of the child widget
	const auto child_sz = haschild() ? nana::size({ _child->size().width + 10, _child->size().height + 10 }) : nana::size();
	//The area used to show the child widget
	auto inner_sz = nana::size(size().width - 2, size().height - 2);

	bool h = false, v = false;

	if(inner_sz.height < child_sz.height)
	{
		v = true;
		inner_sz.width -= SCROLL_SCALE;
		if(inner_sz.width < child_sz.width)
		{
			h = true;
			inner_sz.height -= SCROLL_SCALE;
		}
	}
	else if(inner_sz.width < child_sz.width)
	{
		h = true;
		inner_sz.height -= SCROLL_SCALE;
		if(inner_sz.height < child_sz.height)
		{
			v = true;
			inner_sz.width -= SCROLL_SCALE;
		}
	}

	if(h)
	{
		::nana::rectangle r(1, inner_sz.height + 1, inner_sz.width, SCROLL_SCALE);
		if(_scroll_h.empty())
		{
			_scroll_h.create(*this, r);
			::nana::API::take_active(_scroll_h.handle(), false, *this);

			_scroll_h.events().value_changed.connect_unignorable([this](const ::nana::arg_scroll & arg)
				{
					_scroll_pos.x = -static_cast<int>(_scroll_h.value());
					_child->move(nana::rectangle(_scroll_pos.x + 5, _scroll_pos.y + 5, _child->size().width, _child->size().height));
					::nana::API::refresh_window(*this);
				});
		}
		else
			_scroll_h.move(r);
	}
	else if(!_scroll_h.empty())
		_scroll_h.close();

	if(v)
	{
		::nana::rectangle r(inner_sz.width + 1, 1, SCROLL_SCALE, inner_sz.height);
		if(_scroll_v.empty())
		{
			_scroll_v.create(*this, r);
			::nana::API::take_active(_scroll_v.handle(), false, *this);

			_scroll_v.events().value_changed.connect_unignorable([this](const ::nana::arg_scroll & arg)
				{
					_scroll_pos.y = -static_cast<int>(_scroll_v.value());
					_child->move(nana::rectangle(_scroll_pos.x + 5, _scroll_pos.y + 5, _child->size().width, _child->size().height));
					::nana::API::refresh_window(*this);
				});
		}
		else
			_scroll_v.move(r);
	}
	else if(!_scroll_v.empty())
		_scroll_v.close();


	_inner.move(nana::rectangle(1, 1, inner_sz.width, inner_sz.height));


	// adjust scroll value
	auto graph = nana::API::dev::window_graphics(*this);

	if(!_scroll_h.empty())
	{
		const unsigned ext_px = _scroll_v.empty() ? 0 : SCROLL_SCALE;
		if(ext_px >= inner_sz.width)
			return;

		if(_scroll_pos.x + child_sz.width < inner_sz.width)
		{
			_scroll_pos.x = static_cast<int>(inner_sz.width) - static_cast<int>(child_sz.width);
			if(_scroll_pos.x > 0)
				_scroll_pos.x = 0;
		}

		_scroll_h.amount(child_sz.width);
		_scroll_h.range(inner_sz.width);
		_scroll_h.value(-_scroll_pos.x);
		_scroll_h.step(graph->text_extent_size(L"W").width);
	}
	else
		_scroll_pos.x = 0;

	if(!_scroll_v.empty())
	{
		const unsigned ext_px = _scroll_h.empty() ? 0 : SCROLL_SCALE;
		if(ext_px >= inner_sz.height)
			return;

		if(_scroll_pos.y + child_sz.height < inner_sz.height)
		{
			_scroll_pos.y = static_cast<int>(inner_sz.height) - static_cast<int>(child_sz.height);
			if(_scroll_pos.y > 0)
				_scroll_pos.y = 0;
		}

		_scroll_v.amount(child_sz.height);
		_scroll_v.range(inner_sz.height);
		_scroll_v.value(-_scroll_pos.y);
		_scroll_v.step(graph->text_extent_size(L"W").height);
	}
	else
		_scroll_pos.y = 0;


	_child->move(nana::rectangle(_scroll_pos.x + 5, _scroll_pos.y + 5, _child->size().width, _child->size().height));
}
