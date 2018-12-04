/*
 *		ctrls::box_model Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "ctrls/box_model.h"
#include "config.h"


namespace ctrls
{

	//box_model
	void box_model::bind(nana::window wd)
	{
		_place.bind(wd);
	}

	void box_model::bind(nana::group* grp)
	{
		_place.bind(grp);
	}


	bool box_model::append(ctrl* child)
	{
		if(!child)
			return false;

		_children.push_back(child);

		_place.div(get(DEFAULT_FIELD, false));
		_place[DEFAULT_FIELD] << *child->nanawdg;

		_place.collocate();
		return true;
	}


	bool box_model::insert(ctrl* child, ctrl* pos, bool after)
	{
		if(!child)
			return false;

		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(*i == pos)
			{
				if(after)
					++i;

				// remove from place all ctrls after i
				for(auto r = i; r < _children.end(); ++r)
					_place.erase(*(*r)->nanawdg);

				i = _children.insert(i, child);

				_place.div(get(DEFAULT_FIELD, false));

				// add to place the previously removed controls using the new sequence
				for(auto r = i; r < _children.end(); ++r)
					_place[DEFAULT_FIELD] << *(*r)->nanawdg;

				_place.collocate();
				return true;
			}
		}

		return false;
	}


	bool box_model::remove(ctrl* child)
	{
		if(!child)
			return false;

		_place.erase(*child->nanawdg);

		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(*i == child)
			{
				_children.erase(i);

				update();
				return true;
			}
		}

		update();
		return false;
	}


	bool box_model::moveup(ctrl* child)
	{
		if(!child)
			return false;

		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(*i == child)
			{
				if(i == _children.begin())
					return false;

				std::iter_swap(i, i - 1);

				// remove from place all ctrls after i-1
				for(auto r = i - 1; r < _children.end(); ++r)
					_place.erase(*(*r)->nanawdg);

				// add to place the previously removed controls using the new sequence
				for(auto r = i - 1; r < _children.end(); ++r)
					_place[DEFAULT_FIELD] << *(*r)->nanawdg;

				update();
				return true;
			}
		}

		return false;
	}


	bool box_model::movedown(ctrl* child)
	{
		if(!child)
			return false;

		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(*i == child)
			{
				if(i + 1 == _children.end())
					return false;

				std::iter_swap(i, i + 1);

				// remove from place all ctrls after i
				for(auto r = i; r < _children.end(); ++r)
					_place.erase(*(*r)->nanawdg);

				// add to place the previously removed controls using the new sequence
				for(auto r = i; r < _children.end(); ++r)
					_place[DEFAULT_FIELD] << *(*r)->nanawdg;

				update();
				return true;
			}
		}

		return false;
	}


	void box_model::update()
	{
		_place.div(get(DEFAULT_FIELD, false));
		_place.collocate();
	}


	void box_model::set_type(layout_orientation orientation)
	{
		_type_str = (orientation == layout_orientation::vertical) ? "vert " : "";
		_is_grid = false;
	}
	
	
	void box_model::set_type(const std::string& cols, const std::string& rows)
	{
		if(cols.empty() || rows.empty())
			return;

		if(cols[0] == '0' || rows[0] == '0')
			return;

		_type_str = "grid=[" + cols + "," + rows + "] ";
		_is_grid = true;
	}


	void box_model::set_attributes(const std::string& weight, const std::string& margin, const std::string& gap)
	{
		_weight_str = "";
		if(!weight.empty())
			if(weight[0] != '-')
				_weight_str = "weight=" + weight + " ";

		_margin_str = "";
		if(!margin.empty())
			if(margin[0] != '0')
				_margin_str = "margin=" + margin + " ";

		_gap_str = "";
		if(!gap.empty())
			if(gap[0] != '0')
				_gap_str = "gap=" + gap + " ";
	}


	void box_model::add_collapse(const std::string& collapse)
	{
		if(!collapse.empty())
			_collapse.push_back(collapse);
	}


	void box_model::clear_collapse()
	{
		_collapse.clear();
	}


	std::string box_model::get(const std::string& field, bool generate_code)
	{
		std::string divtext;

		// children divtext
		//-----------------------------
		for(auto c : _children)
		{
			if(!c->get_divtext().empty())
				divtext.append(c->get_divtext());
		}

		bool _fields = divtext.empty() ? false : true;

		if(!generate_code)
			divtext = "";

		if(divtext.empty())
		{
			if(!_is_grid)
			{
				// arrange - children's weight
				//-----------------------------
				std::string arrange;
				bool all_variable = true;

				if(!_children.empty())
				{
					arrange = "arrange=[";
					for(auto c : _children)
					{
						if(!c->get_weight().empty())
						{
							std::string weight = c->get_weight();
							if(weight[0] == '-')
								weight = "variable";
							else
								all_variable = false;

							arrange.append(weight + ",");
						}
					}
					if(arrange[arrange.size() - 1] == ',')
						arrange.pop_back();
					arrange.append("] ");

					// if all ctrls have variable weight -> arrange not needed
					if(all_variable)
						arrange = "";
				}

				divtext += arrange;
			}
			else
			{
				// is grid

				for(auto & c : _collapse)
					divtext += "collapse(" + c + ") ";
			}

			divtext += field;
		}

		return _type_str + _weight_str + _margin_str + (_fields ? "" : _gap_str) + divtext;
	}


	bool box_model::children_fields()
	{
		if(!children())
			return false;

		std::string divtext;

		for(auto c : _children)
		{
			if(!c->get_divtext().empty())
				return true;
		}

		return false;
	}

}//end namespace ctrls
