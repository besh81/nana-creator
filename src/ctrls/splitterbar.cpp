/*
 *		ctrls::splitterbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/splitterbar.h"
#include "style.h"


namespace ctrls
{

	//splitterbar
	splitterbar::splitterbar(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		pnl.create(*parent->nanawdg, true);

		
		nana::drawing dw{ pnl };
		dw.draw([this](nana::paint::graphics& graph)
		{
			graph.rectangle(false, SPLITTERBAR_COL);
		});


		ctrl::init(&pnl, CTRL_SPLITTERBAR, name, false);

		// common
		properties.remove("enabled");
		// appearance
		properties.remove("bgcolor");
		properties.remove("fgcolor");
		// layout
		properties.remove("weight");
	}


	void splitterbar::update()
	{
		//ctrl::update();

		// has the same bg color of parent control
		auto pw = nana::API::get_widget(nanawdg->parent());
		nanawdg->bgcolor(pw->bgcolor());
	}


	void splitterbar::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);
	}


	std::string splitterbar::get_weight()
	{
		return std::to_string(SPLITTERBAR_WEIGHT);
	}


	std::string splitterbar::get_divtext()
	{
		return "|";
	}

}//end namespace ctrls
