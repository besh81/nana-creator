/*
 *		ctrls::box_model Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "ctrls/box_model.h"


namespace ctrls
{

	//box_model
	box_model::box_model(nana::window wd)
	{
		bind(wd);
	}


	void box_model::bind(nana::window wd)
	{
		_place.bind(wd);
		_place.collocate();
	}

	void box_model::bind(nana::group* grp)
	{
		_p_grp = grp;
		_p_grp->collocate();
	}


	void box_model::update()
	{
		if(_p_grp)
		{
			_p_grp->div(getdiv().c_str());
			_p_grp->collocate();
		}
		else
		{
			_place.div(getdiv().c_str());
			_place.collocate();
		}
	}


	void box_model::orientation(layout_orientation orientation)
	{
		_orientation_str = (orientation == layout_orientation::vertical) ? "vert " : "";
	}


	void box_model::padding(int pixels)
	{
		// [css boxmodel]padding -> [nana]margin
		if(pixels > 0)
			_padding_str = "margin=" + std::to_string(pixels) + " ";
		else
			_padding_str = "";
	}


	void box_model::updatefield(nana::window ctrl, const std::string& weight, const std::string& margin)
	{
		for(auto& f : _children)
		{
			if(f.second == ctrl)
			{
				f.first.weight = weight;
				f.first.margin = margin;

				update();
				return;
			}
		}
	}


	bool box_model::append(nana::window ctrl)
	{
		field f;
		f.name = _name_mgr.add_numbered("field");

		_children.push_back(std::pair<field, nana::window>(f, ctrl));

		if(_p_grp)
		{
			_p_grp->div(getdiv().c_str());
			(*_p_grp)[f.name.c_str()] << ctrl;
		}
		else
		{
			_place.div(getdiv().c_str());
			_place[f.name.c_str()] << ctrl;
		}
		return true;
	}


	bool box_model::insert(nana::window pos, nana::window ctrl, bool after)
	{
		field f;
		f.name = _name_mgr.add_numbered("field");

		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(i->second == pos)
			{
				_children.insert(after ? ++i : i, std::pair<field, nana::window>(f, ctrl));

				if(_p_grp)
				{
					_p_grp->div(getdiv().c_str());
					(*_p_grp)[f.name.c_str()] << ctrl;
				}
				else
				{
					_place.div(getdiv().c_str());
					_place[f.name.c_str()] << ctrl;
				}
				return true;
			}
		}

		return false;
	}


	bool box_model::remove(nana::window ctrl)
	{
		if(_p_grp)
			_p_grp->erase(ctrl);
		else
			_place.erase(ctrl);

		for(auto i = _children.begin(); i < _children.end(); ++i)
		{
			if(i->second == ctrl)
			{
				_name_mgr.remove(i->first.name);
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

				update();
				return true;
			}
		}

		return false;
	}


	std::string box_model::getdiv()
	{
		std::string div;
		div.append(_orientation_str);
		div.append(_padding_str);
		//
		for(auto f : _children)
		{
			div.append("<");
			if(!f.first.weight.empty())
				div.append("weight=" + f.first.weight + " ");
			if(!f.first.margin.empty())
				if(f.first.margin != "0")
					div.append("margin=" + f.first.margin + " ");
			div.append(f.first.name);
			div.append(">");
		}

		return div;
	}

}//end namespace ctrls
