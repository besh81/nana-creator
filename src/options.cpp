/*
 *		options Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "options.h"
#include "inifile.h"


extern inifile			g_inifile;


void options::init()
{
	ur_spin.value(std::to_string(g_inifile.undo_queue_len()));

	new_txt.caption(g_inifile.new_project_dir());
	load_txt.caption(g_inifile.load_project_dir());
	image_txt.caption(g_inifile.image_dir());


	//------------------
	// events
	//------------------

	// ok button
	ok_btn.events().click([this]()
		{
			g_inifile.undo_queue_len(ur_spin.to_int(), true);
			g_inifile.save();

			nana::API::close_window(handle());
		});

	// cancel button
	canc_btn.events().click([this]()
		{
			nana::API::close_window(handle());
		});
}
