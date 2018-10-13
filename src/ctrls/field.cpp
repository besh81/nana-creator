/*
 *		ctrls::field Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include <algorithm>
#include "ctrls/field.h"
#include "tokenizer/Tokenizer.h"
#include "style.h"


namespace ctrls
{

	//field
	field::field(nana::window wd, const std::string& name, bool grid)
		: ctrl(), _grid(grid)
	{
		fld.create(wd, true);
		boxmodel.bind(fld);


		nana::drawing dw{ fld };
		dw.draw([this](nana::paint::graphics& graph)
		{
			graph.rectangle(false, FIELD_COL);
		});


		ctrl::init(&fld, _grid ? CTRL_GRID : CTRL_FIELD, name);

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


	void field::update()
	{
		//ctrl::update();

		if(_grid)
			boxmodel.set_type(properties.property("columns").as_string(), properties.property("rows").as_string());
		else
			boxmodel.set_type(static_cast<layout_orientation>(properties.property("layout").as_int()));

		boxmodel.set_attributes(properties.property("weight").as_string(), properties.property("margin").as_string(), properties.property("gap").as_string());

		if(_grid)
		{
			// collapse - START
			boxmodel.clear_collapse();
			// split columns into item (delimiter = CITEM_TKN)
			Tokenizer items_tkn(properties.property("collapse").as_string());
			items_tkn.setDelimiter(CITEM_TKN);

			std::string item;
			while((item = items_tkn.next()) != "")
			{
				// split item into properties (delimiter = CITEM_INNER_TKN)
				Tokenizer item_tkn(item);
				item_tkn.setDelimiter(CITEM_INNER_TKN);

				auto left = item_tkn.next();
				if(left == CITEM_EMPTY) left = "0";
				auto top = item_tkn.next();
				if(top == CITEM_EMPTY) top = "0";
				auto cols = item_tkn.next();
				if(cols == CITEM_EMPTY) cols = "0";
				auto rows = item_tkn.next();
				if(rows == CITEM_EMPTY) rows = "0";

				boxmodel.add_collapse(left + "," + top + "," + cols + "," + rows);
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


	void field::update_children_info(nana::window child, const std::string& divtext, const std::string& weight)
	{
		boxmodel.update_children_info(child, divtext, weight);
	}


	std::string field::get_divtext()
	{
		return "<" + boxmodel.get(properties.property("name").as_string(), true) + ">";
	}


	bool field::children()
	{
		return boxmodel.children();
	}


	bool field::children_fields()
	{
		return boxmodel.children_fields();
	}


	bool field::append(nana::window ctrl)
	{
		return boxmodel.append(ctrl);
	}
	

	bool field::insert(nana::window pos, nana::window ctrl, bool after)
	{
		return boxmodel.insert(pos, ctrl, after);
	}


	bool field::remove(nana::window ctrl)
	{
		return boxmodel.remove(ctrl);
	}


	bool field::moveup(nana::window ctrl)
	{
		return boxmodel.moveup(ctrl);
	}


	bool field::movedown(nana::window ctrl)
	{
		return boxmodel.movedown(ctrl);
	}

}//end namespace ctrls
