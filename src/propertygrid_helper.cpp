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
			nana::propertygrid::item_proxy ip(nullptr);

			if(prop.type() == ctrls::pg_type::string_int)
			{
				ip = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string_int(prop.label(), prop.value())));
			}
			else if(prop.type() == ctrls::pg_type::string_uint || prop.type() == ctrls::pg_type::string_uint_0_100 || prop.type() == ctrls::pg_type::string_uint_1_100)
			{
				auto psui = new nana::pg_string_uint(prop.label(), prop.value());
				ip = cat.append(nana::propertygrid::pgitem_ptr(psui));

				if(prop.type() == ctrls::pg_type::string_uint_0_100)
					psui->range(0, 100);
				else if(prop.type() == ctrls::pg_type::string_uint_1_100)
					psui->range(1, 100);
			}
			else if(prop.type() == ctrls::pg_type::string_weight)
			{
				ip = cat.append(nana::propertygrid::pgitem_ptr(new pg_layout_weight(prop.label(), prop.value())));
			}
			else if(prop.type() == ctrls::pg_type::check)
			{
				ip = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_check(prop.label(), prop.value() == "true" ? true : false)));
			}
			else if(prop.type() == ctrls::pg_type::color)
			{
				ip = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_color(prop.label(), prop.value())));
			}
			else if(prop.type() == ctrls::pg_type::color_inherited)
			{
				ip = cat.append(nana::propertygrid::pgitem_ptr(new pg_color_inherited(prop.label(), prop.value())));
			}
			else if(prop.type() == ctrls::pg_type::halign || prop.type() == ctrls::pg_type::valign || prop.type() == ctrls::pg_type::layout ||
				prop.type() == ctrls::pg_type::seekdir || prop.type() == ctrls::pg_type::check_style || prop.type() == ctrls::pg_type::include_style ||
				prop.type() == ctrls::pg_type::focus_behavior)
			{
				auto pgc = new nana::pg_choice(prop.label());
				ip = cat.append(nana::propertygrid::pgitem_ptr(pgc));

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
				else if(prop.type() == ctrls::pg_type::include_style)
				{
					pgc->push_back(CITEM_INCLUDE_1);
					pgc->push_back(CITEM_INCLUDE_2);
				}
				else if(prop.type() == ctrls::pg_type::focus_behavior)
				{
					pgc->push_back(CITEM_FOCUS_NONE);
					pgc->push_back(CITEM_FOCUS_SEL);
					pgc->push_back(CITEM_FOCUS_TAB);
					pgc->push_back(CITEM_FOCUS_CLICK);
					pgc->push_back(CITEM_FOCUS_TAB_CLICK);
				}
			
				pgc->option(std::atoi(prop.value().c_str()));
			}
			else if(prop.type() == ctrls::pg_type::image)
			{
				ip = cat.append(nana::propertygrid::pgitem_ptr(new pg_image(prop.label(), prop.value())));
			}
			else if(prop.type() == ctrls::pg_type::folder)
			{
				ip = cat.append(nana::propertygrid::pgitem_ptr(new pg_folder(prop.label(), prop.value())));
			}
			else if(prop.type() == ctrls::pg_type::collection_combox || prop.type() == ctrls::pg_type::collection_toolbar
				|| prop.type() == ctrls::pg_type::collection_listbox || prop.type() == ctrls::pg_type::collection_tabbar
				|| prop.type() == ctrls::pg_type::collection_menubar || prop.type() == ctrls::pg_type::collection_categorize
				|| prop.type() == ctrls::pg_type::collection_collapse)
			{
				ip = cat.append(nana::propertygrid::pgitem_ptr(new pg_collection(prop.label(), prop.type(), items)));
			}
			else if(prop.type() == ctrls::pg_type::margin)
			{
				ip = cat.append(nana::propertygrid::pgitem_ptr(new pg_margin(prop.label(), prop.value())));
			}
			else //nana::pg_type::string
			{
				ip = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string(prop.label(), prop.value())));
			}


			// set default value
			ip.defvalue(prop.defvalue());
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
