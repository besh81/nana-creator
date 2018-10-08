/*
 *		ctrls::spacer Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/spacer.h"
#include "style.h"


namespace ctrls
{

	//spacer
	spacer::spacer(nana::window wd, const std::string& name)
		: ctrl()
	{
		pnl.create(wd, true);

		
		nana::drawing dw{ pnl };
		dw.draw([this](nana::paint::graphics& graph)
		{
			graph.rectangle(false, SPACER_COL);
			graph.line(nana::point(0, 0), nana::point(static_cast<int>(graph.width()) - 1, static_cast<int>(graph.height()) - 1), SPACER_COL);
			graph.line(nana::point(0, static_cast<int>(graph.height()) - 1), nana::point(static_cast<int>(graph.width()) - 1, 0), SPACER_COL);
		});


		ctrl::init(&pnl, CTRL_SPACER, name);

		// common
		properties.remove("enabled");
		// appearance
		properties.remove("bgcolor");
		properties.remove("fgcolor");
		// layout
		// ...
	}


	void spacer::update()
	{
		//ctrl::update();

		auto pw = nana::API::get_widget(nanawdg->parent());
		nanawdg->bgcolor(pw->bgcolor());
	}


	void spacer::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);
	}

}//end namespace ctrls
