/*
 *		resizablecanvas Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_RESIZABLECANVAS_H
#define NANA_CREATOR_RESIZABLECANVAS_H

#include <nana/gui/widgets/widget.hpp>


namespace drawerbase
{
	namespace resizablecanvas
	{
		struct essence_t;

		class trigger : public nana::drawer_trigger
		{
		public:
			trigger();
			~trigger();

			essence_t& essence() const;

		private:
			void attached(widget_reference, graph_reference) override;
			void detached() override;
			void refresh(graph_reference) override;
			void resized(graph_reference, const ::nana::arg_resized&) override;

			void _m_draw_border();

			nana::window	window_{ nullptr };
			essence_t*		essence_{ 0 };
		};
	}// end namespace resizablecanvas
}//end namespace drawerbase


class resizablecanvas
	: public nana::widget_object<typename nana::category::widget_tag, drawerbase::resizablecanvas::trigger>
{
public:
	resizablecanvas(nana::window wd);

	bool haschild() { return _child ? true : false; }
	bool add(nana::window child);
	bool remove(nana::window child);
	::nana::widget* child();

private:
	::nana::widget*		_child{ 0 };

	drawerbase::resizablecanvas::essence_t& _m_ess() const;
};

#endif //NANA_CREATOR_RESIZABLECANVAS_H
