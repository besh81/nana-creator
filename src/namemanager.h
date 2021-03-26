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
#include <functional>


class namemanager
{
public:
	namemanager() = default;
	~namemanager();

	bool add(const std::string& name);
	std::string add_numbered(const std::string& name);

	bool remove(const std::string& name);

	std::vector<std::string> getlist() { return list_; }

	void clear() { list_.clear(); }

	void print(std::basic_ostream<char, std::char_traits<char> >& stream) const;

	void set_validation_rule(std::function<bool(const std::string& str)> f) { validation_rule_f_ = f; }  ///< Sets user defined rule to validate new name

private:
	std::vector<std::string> list_;	///< unordered list of names

	std::function<bool(const std::string& str)> validation_rule_f_; ///< user defined rule to validate new name
};

#endif //NANA_CREATOR_NAMEMANAGER_H
