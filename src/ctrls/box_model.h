/*
 *		ctrls::box_model Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_BOX_MODEL_H
#define NANA_CREATOR_BOX_MODEL_H

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/group.hpp>
#include "namemanager.h"


namespace ctrls
{

	enum class layout_orientation
	{
		horizontal,
		vertical
	};


	class box_model
	{
	public:
		box_model() = default;
		box_model(nana::window wd);

		void bind(nana::window wd);
		void bind(nana::group* grp);

		void update();
		void orientation(layout_orientation orientation);
		void padding(int pixels);

		void updatefield(nana::window ctrl, const std::string& weight, const std::string& margin);

		std::string getdiv();

		bool children() { return !_children.empty(); }
		bool append(nana::window ctrl);
		bool insert(nana::window pos, nana::window ctrl, bool after = true);
		bool remove(nana::window ctrl);

		bool moveup(nana::window ctrl);
		bool movedown(nana::window ctrl);


	protected:
		std::string		_orientation_str;
		std::string		_padding_str;

		namemanager		_name_mgr;
		struct field
		{
			std::string name;
			std::string weight;
			std::string margin;
		};
		std::vector<std::pair<field, nana::window>>	_children;

		nana::place		_place;
		nana::group*	_p_grp{ 0 };
	};

}//end namespace ctrls

#endif //NANA_CREATOR_BOX_MODEL_H
