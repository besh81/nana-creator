/*
 *		filemanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_FILEMANAGER_H
#define NANA_CREATOR_FILEMANAGER_H

#include <string>


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

#endif //NANA_CREATOR_FILEMANAGER_H
