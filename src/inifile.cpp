/*
 *		inifile Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "version.h"
#include "inifile.h"
#include "pugixml/pugixml.hpp"


#define INIT_UNDO_QUEUE_LEN		30


//inifile
inifile::inifile()
{
	_undo_queue_len = INIT_UNDO_QUEUE_LEN;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(INI_FILE);

	bool create = false;

	if(!result)
		create = true;
	else
	{
		pugi::xml_node root = doc.child(NODE_ROOT);
		if(root.empty())
			create = true;
		else
		{
			// read values
			_new_prj_dir = root.attribute("new_prj_dir").as_string();
			_load_prj_dir = root.attribute("load_prj_dir").as_string();
			_img_dir = root.attribute("img_dir").as_string();
			//
			_undo_queue_len = root.attribute("undo_queue_len").as_uint(INIT_UNDO_QUEUE_LEN);
		}
	}

	if(create)
		save();
}


void inifile::new_project_dir(const std::string& dir, bool save_to_file)
{
	_new_prj_dir = dir;

	if(save_to_file)
		save();
}


void inifile::load_project_dir(const std::string& dir, bool save_to_file)
{
	_load_prj_dir = dir;

	if(save_to_file)
		save();
}


void inifile::image_dir(const std::string& dir, bool save_to_file)
{
	_img_dir = dir;

	if(save_to_file)
		save();
}


void inifile::undo_queue_len(unsigned int len, bool save_to_file)
{
	_undo_queue_len = len;

	if(save_to_file)
		save();
}


bool inifile::save()
{
	pugi::xml_document doc;

	// append root node
	pugi::xml_node root = doc.append_child(NODE_ROOT);
	root.append_attribute("version") = CREATOR_VERSION;

	root.append_attribute("new_prj_dir") = _new_prj_dir.c_str();
	root.append_attribute("load_prj_dir") = _load_prj_dir.c_str();
	root.append_attribute("img_dir") = _img_dir.c_str();
	//
	root.append_attribute("undo_queue_len") = _undo_queue_len;

	return doc.save_file(INI_FILE);
}

