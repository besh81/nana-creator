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


	void box_model::update()
	{
		_place.div(get(DEFAULT_FIELD, false));
		_place.collocate();
	}


	void box_model::set(layout_orientation orientation, const std::string& weight, const std::string& margin)
	{
		_field_str = (orientation == layout_orientation::vertical) ? "vert " : "";
		_is_grid = false;

		if(!weight.empty())
			if(weight[0] != '-')
				_field_str.append("weight=" + weight + " ");

		if(!margin.empty())
			if(margin[0] != '0')
				_field_str.append("margin=" + margin + " ");
	}


	void box_model::set(const std::string& cols, const std::string& rows, const std::string& margin, const std::string& gap)
	{
		if(cols.empty() || rows.empty())
			return;

		if(cols[0] == '0' || rows[0] == '0')
			return;

		_field_str = "grid=[" + cols + "," + rows + "] ";
		_is_grid = true;

		if(!margin.empty())
			if(margin[0] != '0')
				_field_str.append("margin=" + margin + " ");

		if(!gap.empty())
			if(gap[0] != '0')
				_field_str.append("gap=" + gap + " ");
	}


	std::string box_model::get(const std::string& field, bool generate_code)
	{
		std::string divtext;

		// children divtext
		//-----------------------------
		if(generate_code)
		{
			for(auto f : _children)
			{
				if(!f.first.divtext.empty())
					divtext.append(f.first.divtext);
			}
		}

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
					for(auto f : _children)
					{
						if(!f.first.weight.empty())
						{
							arrange.append(f.first.weight + ",");

							if(f.first.weight[0] != 'v')
								all_variable = false;
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

			divtext += field;
		}


		return _field_str + divtext;
	}


	void box_model::update_children_info(nana::window child, const std::string& divtext, const std::string& weight)
	{
		for(auto& f : _children)
		{
			if(f.second == child)
			{
				f.first.divtext = divtext;

				if(weight[0] == '-')
					f.first.weight = "variable";
				else
					f.first.weight = weight;

				update();
				return;
			}
		}
	}


	bool box_model::children_fields()
	{
		if(!children())
			return false;

		std::string divtext;

		for(auto f : _children)
		{
			if(!f.first.divtext.empty())
				divtext.append(f.first.divtext);
		}

		return divtext.empty() ? false : true;
	}
	

	bool box_model::append(nana::window ctrl)
	{
		_children.push_back(std::pair<child_info, nana::window>({}, ctrl));

		_place.div(get(DEFAULT_FIELD, false));
		_place[DEFAULT_FIELD] << ctrl;

		_place.collocate();
		return true;
	}


	bool box_model::insert(nana::window pos, nana::window ctrl, bool after)
	{
		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(i->second == pos)
			{
				if(after)
					++i;

				// remove from place all ctrls after i
				for(auto r = i; r < _children.end(); ++r)
					_place.erase(r->second);

				i = _children.insert(i, std::pair<child_info, nana::window>({}, ctrl));

				_place.div(get(DEFAULT_FIELD, false));

				// add to place the previously removed controls using the new sequence
				for(auto r = i; r < _children.end(); ++r)
					_place[DEFAULT_FIELD] << r->second;

				_place.collocate();
				return true;
			}
		}

		return false;
	}


	bool box_model::remove(nana::window ctrl)
	{
		_place.erase(ctrl);

		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(i->second == ctrl)
			{
				_children.erase(i);

				update();
				return true;
			}
		}

		update();
		return false;
	}

	bool box_model::moveup(nana::window ctrl)
	{
		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(i->second == ctrl)
			{
				if(i == _children.begin())
					return false;

				std::iter_swap(i, i - 1);

				// remove from place all ctrls after i-1
				for(auto r = i-1; r < _children.end(); ++r)
					_place.erase(r->second);

				// add to place the previously removed controls using the new sequence
				for(auto r = i - 1; r < _children.end(); ++r)
					_place[DEFAULT_FIELD] << r->second;

				update();
				return true;
			}
		}

		return false;
	}

	bool box_model::movedown(nana::window ctrl)
	{
		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(i->second == ctrl)
			{
				if(i + 1 == _children.end())
					return false;

				std::iter_swap(i, i + 1);

				// remove from place all ctrls after i
				for(auto r = i; r < _children.end(); ++r)
					_place.erase(r->second);

				// add to place the previously removed controls using the new sequence
				for(auto r = i; r < _children.end(); ++r)
					_place[DEFAULT_FIELD] << r->second;

				update();
				return true;
			}
		}

		return false;
	}


}//end namespace ctrls
