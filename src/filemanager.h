/*
 *		filemanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_FILEMANAGER_H
#define NANA_CREATOR_FILEMANAGER_H

#include <string>



//---------------------------------------------
// filemanager
// manage file paths
//---------------------------------------------
class filemanager
{
public:
	filemanager(bool relative = true)
	{
		enable_relative(relative);
	}

	void enable_relative(bool state)
	{
		relative_ = state;
	}
	bool is_relative()
	{
		return relative_;
	}

	void basedir(const std::string& path)
	{
		base_ = path;
	}
	std::string basedir()
	{
		return base_;
	}

	std::string to_relative(const std::string& abspath);

private:
	std::string base_;			// base directory
	bool		relative_;
};



//---------------------------------------------
// equalize path characters (tolower) and and dir separators
//---------------------------------------------
std::string equalize_path(const std::string& path, char div_from = '\\', char div_to = '/');


//---------------------------------------------
// given an absolute directory (basedir) and an 
// absolute path (abspath), returns the relative path
//---------------------------------------------
std::string get_relative_path(const std::string& basedir, const std::string& abspath);


//---------------------------------------------
// get current working directory
//---------------------------------------------
std::string get_working_dir();


//---------------------------------------------
// set working directory
//---------------------------------------------
bool set_working_dir(const std::string& path);


//---------------------------------------------
// check file existance
//---------------------------------------------
bool file_exists(const std::string& name);


//---------------------------------------------
// check directory existance
//---------------------------------------------
bool dir_exists(const std::string& path);


#endif //NANA_CREATOR_FILEMANAGER_H
