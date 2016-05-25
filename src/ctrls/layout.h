/*
 *		ctrls::layout Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_LAYOUT_H
#define NANA_CREATOR_LAYOUT_H

#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/place.hpp>
#include "ctrls/property.h"
#include "namemanager.h"


namespace ctrls
{

	enum class layout_orientation
	{
		horizontal,
		vertical
	};


	namespace drawerbase
	{
		namespace layout
		{
			class drawer : public nana::drawer_trigger
			{
				void attached(widget_reference, graph_reference) override;
				void refresh(graph_reference) override;

			private:
				nana::window window_{ nullptr };
			};
		}// end namespace layout
	}//end namespace drawerbase


	class layout
		: public nana::widget_object<typename nana::category::widget_tag, drawerbase::layout::drawer>
	{
	public:
		layout(nana::window wd, properties_collection* properties, const std::string& name, layout_orientation lo);

		void update(properties_collection* properties);
		void updatefield(nana::window child, const std::string& weight, const std::string& margin);

		void generatecode(properties_collection* properties, std::vector<std::string>* decl, std::vector<std::string>* init);

		std::string getdiv();

		void prepareforinsert();
		bool append(nana::window child);
		bool remove(nana::window child);
		bool moveup(nana::window child);
		bool movedown(nana::window child);


	protected:
		void _orientation(layout_orientation orientation);
		void _padding(int pixels);

		void _initproperties(properties_collection* properties, const std::string& name);

		bool			_is_highlighted{ false };
		nana::color		_bgcolor;

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
	};

}//end namespace ctrls

#endif //NANA_CREATOR_LAYOUT_H
