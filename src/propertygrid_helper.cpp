/*
 *		propertygrid_helper Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "propertygrid_helper.h"
#include "pgitems_creator.h"


namespace propertygrid_helper
{

	void append(nana::propertygrid* propgrid, ctrls::properties_collection* properties, std::vector<ctrls::properties_collection>* items)
	{
		if(!propgrid || !properties)
			return;

		for(size_t i = 0; i < properties->count(); ++i)
		{
			auto prop = (*properties)[i];
			if(prop.label().empty())
				continue;

			auto cat_idx = propgrid->find(prop.category());
			auto cat = (cat_idx == nana::npos) ? propgrid->append(prop.category()) : propgrid->at(cat_idx);

			if(prop.type() == ctrls::pg_type::string_int)
			{
				cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string_int(prop.label(), prop.value())));
			}
			else if(prop.type() == ctrls::pg_type::string_uint || prop.type() == ctrls::pg_type::string_uint_0_100 || prop.type() == ctrls::pg_type::string_uint_1_100)
			{
				auto item = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string_uint(prop.label(), prop.value())));

				if(prop.type() == ctrls::pg_type::string_uint_0_100)
				{
					auto psui = static_cast<nana::pg_string_uint*>(item._m_pgitem());
					psui->range(0, 100);
				}
				else if(prop.type() == ctrls::pg_type::string_uint_1_100)
				{
					auto psui = static_cast<nana::pg_string_uint*>(item._m_pgitem());
					psui->range(1, 100);
				}
			}
			else if(prop.type() == ctrls::pg_type::string_weight)
			{
				cat.append(nana::propertygrid::pgitem_ptr(new pg_layout_weight(prop.label(), prop.value())));
			}
			else if(prop.type() == ctrls::pg_type::check)
			{
				cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_check(prop.label(), prop.value() == "true" ? true : false)));
			}
			else if(prop.type() == ctrls::pg_type::color)
			{
				cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_color(prop.label(), prop.value(), false)));
			}
			else if(prop.type() == ctrls::pg_type::color_inherited)
			{
				cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_color(prop.label(), prop.value(), true)));
			}
			else if(prop.type() == ctrls::pg_type::halign || prop.type() == ctrls::pg_type::valign || prop.type() == ctrls::pg_type::layout ||
				prop.type() == ctrls::pg_type::seekdir || prop.type() == ctrls::pg_type::check_style)
			{
				auto item = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_choice(prop.label())));
				auto pgc = static_cast<nana::pg_choice*>(item._m_pgitem());

				if(prop.type() == ctrls::pg_type::halign)
				{
					pgc->push_back(CITEM_LEFT);
					pgc->push_back(CITEM_CENTER);
					pgc->push_back(CITEM_RIGHT);
				}
				else if(prop.type() == ctrls::pg_type::valign)
				{
					pgc->push_back(CITEM_TOP);
					pgc->push_back(CITEM_CENTER);
					pgc->push_back(CITEM_BOTTOM);
				}
				else if(prop.type() == ctrls::pg_type::layout)
				{
					pgc->push_back(CITEM_HLAYOUT);
					pgc->push_back(CITEM_VLAYOUT);
				}
				else if(prop.type() == ctrls::pg_type::seekdir)
				{
					pgc->push_back(CITEM_BILATERAL);
					pgc->push_back(CITEM_FORWD);
					pgc->push_back(CITEM_BACKWD);
				}
				else if(prop.type() == ctrls::pg_type::check_style)
				{
					pgc->push_back(CITEM_NONE);
					pgc->push_back(CITEM_OPTION);
					pgc->push_back(CITEM_HIGHLIGHT);
				}
			
				pgc->option(std::atoi(prop.value().c_str()));
			}
			else if(prop.type() == ctrls::pg_type::image)
			{
				cat.append(nana::propertygrid::pgitem_ptr(new pg_image(prop.label(), prop.value())));
			}
			else if(prop.type() == ctrls::pg_type::folder)
			{
				cat.append(nana::propertygrid::pgitem_ptr(new pg_folder(prop.label(), prop.value())));
			}
			else if(prop.type() == ctrls::pg_type::collection_combox || prop.type() == ctrls::pg_type::collection_toolbar
				|| prop.type() == ctrls::pg_type::collection_listbox || prop.type() == ctrls::pg_type::collection_tabbar
				|| prop.type() == ctrls::pg_type::collection_menubar || prop.type() == ctrls::pg_type::collection_categorize
				|| prop.type() == ctrls::pg_type::collection_collapse)
			{
				cat.append(nana::propertygrid::pgitem_ptr(new pg_collection(prop.label(), prop.type(), items)));
			}
			else //nana::pg_type::string
			{
				cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string(prop.label(), prop.value())));
			}
		}

		// look for properties bonds
		for(size_t i = 0; i < properties->count(); ++i)
		{
			auto pi = (*properties)[i];
			enabled_bonds(propgrid, properties, pi.name(), pi.as_bool());
		}
	}


	void enabled_bonds(nana::propertygrid* propgrid, ctrls::properties_collection* properties, const std::string& name, bool value)
	{
		for(size_t i = 0; i < properties->count(); ++i)
		{
			auto prop = (*properties)[i];
			if(name == prop.enabled())
			{
				auto cat_idx = propgrid->find(prop.category());
				if(cat_idx == nana::npos)
					continue;

				auto cat = propgrid->at(cat_idx);
				for(auto c : cat)
				{
					if(c.label() == prop.label())
					{
						c._m_pgitem()->enabled(value == prop.enabled_value());
						c._m_pgitem()->update();
						break;
					}
				}
			}
		}
	}

}//end namespace propertygrid_helper
