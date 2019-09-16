/*
 *		ctrls::field Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <algorithm>
#include "ctrls/field.h"
#include "style.h"


namespace ctrls
{

	//field
	field::field(ctrl* parent, const std::string& name, bool grid, bool visible)
		: ctrl(parent), _grid(grid)
	{
		fld.create(*parent->nanawdg, visible);
		boxmodel.bind(fld);

		fld.transparent(true);

		nana::drawing dw{ fld };
		dw.draw([this](nana::paint::graphics& graph)
		{
			graph.rectangle(false, FIELD_COL);
		});


		ctrl::init(&fld, _grid ? CTRL_GRID : CTRL_FIELD, name, false);

		// common
		properties.remove("enabled");
		// appearance
		properties.remove("bgcolor");
		properties.remove("fgcolor");
		// layout
		if(_grid)
		{
			properties.append("columns").label("Columns").category(CAT_LAYOUT).type(pg_type::string_uint_1_100) = 3;
			properties.append("rows").label("Rows").category(CAT_LAYOUT).type(pg_type::string_uint_1_100) = 3;
		}
		else
			properties.append("layout").label("Layout").category(CAT_LAYOUT).type(pg_type::layout) = static_cast<int>(layout_orientation::horizontal);
		
		properties.append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 5;
		properties.append("gap").label("Gap").category(CAT_LAYOUT).type(pg_type::string_uint) = 2;

		if(_grid)
			properties.append("collapse").label("collapse").category(CAT_LAYOUT).type(pg_type::collection_collapse);
	}


	void field::init_item(properties_collection& item)
	{
		ctrl::init_item(item);
		item.property("type") = "collapse";
		//
		item.append("left").label("Left").category(CAT_COMMON).type(pg_type::string_uint) = 0;
		item.append("top").label("Top").category(CAT_COMMON).type(pg_type::string_uint) = 0;
		item.append("cols").label("Columns").category(CAT_COMMON).type(pg_type::string_uint) = 0;
		item.append("rows").label("Rows").category(CAT_COMMON).type(pg_type::string_uint) = 0;
	}


	void field::update()
	{
		//ctrl::update();
		auto pw = nana::API::get_widget(nanawdg->parent());
		fld.bgcolor(pw->bgcolor());
		fld.fgcolor(pw->fgcolor());

		// update boxmodel
		if(_grid)
			boxmodel.set_type(properties.property("columns").as_string(), properties.property("rows").as_string());
		else
			boxmodel.set_type(static_cast<layout_orientation>(properties.property("layout").as_int()));

		boxmodel.set_attributes(properties.property("weight").as_string(), properties.property("margin").as_string(), properties.property("gap").as_string());

		if(_grid)
		{
			// collapse - START
			boxmodel.clear_collapse();
			for(auto& i : items)
			{
				boxmodel.add_collapse(i.property("left").as_string() + "," + i.property("top").as_string() + "," + i.property("cols").as_string() + "," + i.property("rows").as_string());
			}
			// collapse - END
		}

		boxmodel.update();
	}


	void field::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		//ctrl::generatecode(cd, ci);

		auto name = properties.property("name").as_string();

		// headers
		// ...
		// declaration
		// ...
		// init
		// ...
		// placement
		// ...
		// collocate
		// ...
		// children
		ci->field = name;
	}


	std::string field::get_divtext()
	{
		return "<" + boxmodel.get(properties.property("name").as_string(), true) + ">";
	}

}//end namespace ctrls
