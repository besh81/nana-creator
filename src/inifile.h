/*
 *		inifile Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_INIFILE_H
#define NANA_INIFILE_H

#include <string>



//---------------------------------------------
// inifile
// manage program configuration file
//---------------------------------------------
class inifile
{
public:
	inifile();

	std::string new_project_dir() { return _new_prj_dir; }
	void new_project_dir(const std::string& dir, bool save_to_file = true);

	std::string load_project_dir() { return _load_prj_dir; }
	void load_project_dir(const std::string& dir, bool save_to_file = true);

	std::string image_dir() { return _img_dir; }
	void image_dir(const std::string& dir, bool save_to_file = true);

	unsigned int undo_queue_len() { return _undo_queue_len; }
	void undo_queue_len(unsigned int len, bool save_to_file = true);

	bool save();

protected:
	std::string		_new_prj_dir;
	std::string		_load_prj_dir;
	std::string		_img_dir;

	unsigned int	_undo_queue_len;
};

#endif //NANA_INIFILE_H
