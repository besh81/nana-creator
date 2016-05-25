/*
 *		namemanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_NAMEMANAGER_H
#define NANA_CREATOR_NAMEMANAGER_H

#include <vector>
#include <string>


class namemanager
{
public:
	namemanager() {}
	~namemanager();

	bool add(const std::string& name);
	std::string add_numbered(const std::string& name);

	bool remove(const std::string& name);

	std::vector<std::string> getlist() { return _list; }

	void clear() { _list.clear(); }

	void print(std::basic_ostream<char, std::char_traits<char> >& stream) const;

private:
	std::vector<std::string> _list;	// unordered list of names
};

#endif //NANA_CREATOR_NAMEMANAGER_H
