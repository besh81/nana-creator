/*
 *		filemanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "filemanager.h"
#include "relativepath.h"


//filemanager
std::string filemanager::to_relative(const std::string& abspath)
{
	if(abspath.empty())
		return "";

	if(!relative_)
		return equalize_path(abspath);

	return get_relative_path(equalize_path(base_), equalize_path(abspath));
}
