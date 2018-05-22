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
// equalize path characters (tolower) and slashs('/')
//---------------------------------------------
std::string equalize_path(const std::string& path);


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
// check file existance
//---------------------------------------------
bool file_exists(const std::string& name);


#endif //NANA_CREATOR_FILEMANAGER_H
