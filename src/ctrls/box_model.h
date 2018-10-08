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

		void update();

		void set(layout_orientation orientation, const std::string& weight, const std::string& margin);
		void set(const std::string& cols, const std::string& rows, const std::string& margin, const std::string& gap);
		std::string get(const std::string& field, bool generate_code);

		void update_children_info(nana::window child, const std::string& divtext, const std::string& weight);

		bool children() { return !_children.empty(); }
		bool children_fields();
		bool append(nana::window ctrl);
		bool insert(nana::window pos, nana::window ctrl, bool after = true);
		bool remove(nana::window ctrl);

		bool moveup(nana::window ctrl);
		bool movedown(nana::window ctrl);


	protected:
		std::string		_field_str;
		bool			_is_grid{ false };

		struct child_info
		{
			std::string divtext;
			std::string weight{ "variable" };
		};
		std::vector<std::pair<child_info, nana::window>>	_children;

		boxmodel_place	_place;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_BOX_MODEL_H
