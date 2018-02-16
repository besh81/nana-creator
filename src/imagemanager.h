/*
 *		imagemanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_IMAGEMANAGER_H
#define NANA_CREATOR_IMAGEMANAGER_H

#include <vector>
#include <string>


class imagemanager
{
public:
	imagemanager() {}
	~imagemanager();

	bool add(const std::string& name, const std::string& path);
	bool remove(const std::string& name);

	std::string name(unsigned pos);
	std::string path(unsigned pos);
	std::string path(const std::string& name);

	void clear() { _list.clear(); }

private:
	struct _img
	{
		std::string name;
		std::string path;
	};

	std::vector<_img> _list;	// unordered list of images
};

#endif //NANA_CREATOR_IMAGEMANAGER_H
