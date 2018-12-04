/*
 *		ctrls::notebook Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_NOTEBOOK_H
#define NANA_CREATOR_NOTEBOOK_H

#include <mutex>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include "ctrls/ctrl.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class notebook
		: public ctrl
	{
	public:
		notebook(ctrl* parent, const std::string& name);

		// relationship management
		bool append(ctrl* child) override;
		bool remove(ctrl* child) override;
		//
		bool moveup(ctrl* child) override;
		bool movedown(ctrl* child) override;
		//
		bool children() override { return !pages.empty(); }
		//
		ctrl* get_page(size_t pos);
		void show_page(ctrl* page);
		
		
		void update() override;
		void refresh() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;


		// mouse events
		void connect_mouse_enter(std::function<void(const nana::arg_mouse&)> f) override;
		void connect_mouse_leave(std::function<void(const nana::arg_mouse&)> f) override;
		void connect_mouse_down(std::function<void(const nana::arg_mouse&)> f) override;
		//
		void connect_tab_click(std::function<void(const nana::arg_tabbar_mouse<size_t>&)> f);


	protected:
		nana::panel<true>		pnl;
		nana::place				plc;
		nana::tabbar<size_t>	tbb;

		std::vector<ctrl*>		pages;

		std::mutex				mouse_mtx; // needs to discriminate tabbar page click from other tabbar click

		bool internal_use{ false };
	};

}//end namespace ctrls

#endif //NANA_CREATOR_NOTEBOOK_H
