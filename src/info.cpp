/*
 *      Nana C++ Library - Creator
 *      Author: besh81
 */
#include "info.h"
#include "config.h"


void info::_init_ctrls()
{
	_ver.caption(CREATOR_VERSION);

	_thanks_txt.caption(
		"Jinhao:\t\tNana C++ Library and help\n"
		"szega83:\t\tbug fixing\n"
		"James Bremner:\t\tpropertygrid improvements\n");


	//------------------
	// events
	//------------------

	// ok button
	_ok_btn.events().click([this]()
	{
		nana::API::close_window(handle());
	});


	// set focus
	_ok_btn.focus();
}
