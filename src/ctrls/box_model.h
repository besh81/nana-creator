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
#include "ctrls/ctrl.h"


namespace ctrls
{

	enum class layout_orientation
	{
		horizontal,
		vertical
	};


	class boxmodel_place
	{
	public:
		boxmodel_place() {}

		void bind(nana::window handle)
		{
			place.bind(handle);
			place.collocate();
		}

		void bind(nana::group* handle)
		{
			p_grp = handle;
			p_grp->collocate();
		}

		void div(const std::string& div_text)
		{
			if(p_grp)
				p_grp->div(div_text.c_str());
			else
				place.div(div_text);
		}

		void collocate()
		{
			if(p_grp)
				p_grp->collocate();
			else
				place.collocate();
		}

		nana::place::field_reference operator[](const char* field)
		{
			if(p_grp)
				return (*p_grp)[field];
			else
				return place[field];
		}

		void erase(nana::window handle)
		{
			if(p_grp)
				p_grp->erase(handle);
			else
				place.erase(handle);
		}

	protected:
		nana::place		place;
		nana::group*	p_grp{ 0 };

	};


	class box_model
	{
	public:
		box_model() = default;

		void bind(nana::window handle);
		void bind(nana::group* handle);

		bool append(ctrl* child);
		bool insert(ctrl* child, ctrl* pos, bool after);
		bool remove(ctrl* child);
		bool moveup(ctrl* child);
		bool movedown(ctrl* child);
		//
		bool children() { return !_children.empty(); }
		bool children_fields();

		void update();

		void set_type(layout_orientation orientation);
		void set_type(const std::string& cols, const std::string& rows);
		void set_attributes(const std::string& weight, const std::string& margin, const std::string& gap);
		void add_collapse(const std::string& collapse);
		void clear_collapse();
		std::string get(const std::string& field, bool generate_code);

	protected:
		std::string		_type_str;
		std::string		_weight_str;
		std::string		_margin_str;
		std::string		_gap_str;
		bool			_is_grid{ false };

		std::vector<std::string> _collapse;
		
		std::vector<ctrl*>	_children;

		boxmodel_place	_place;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_BOX_MODEL_H
