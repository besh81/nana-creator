/*
 *		nana::relativepath Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_RELATIVEPATH_H
#define NANA_CREATOR_RELATIVEPATH_H

#include <string>

std::string equalize_path(const std::string& path);

std::string get_relative_path(const std::string& basedir, const std::string& abspath);

#endif //NANA_CREATOR_RELATIVEPATH_H
