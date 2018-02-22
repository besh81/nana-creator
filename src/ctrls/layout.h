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
#include "ctrls/ctrl.h"
#include "namemanager.h"
#include "codegenerator.h"


namespace nana
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
		layout() = default;
		layout(nana::window wd, const nana::rectangle& r = {}, bool visible = true);

		bool create(nana::window wd, const nana::rectangle& r = {}, bool visible = true);

		void update();
		void orientation(layout_orientation orientation);
		void padding(int pixels);

		void updatefield(nana::window child, const std::string& weight, const std::string& margin);

		std::string getdiv();

		bool append(nana::window child);
		bool remove(nana::window child);
		bool moveup(nana::window child);
		bool movedown(nana::window child);


	protected:
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

}//end namespace nana


namespace ctrls
{

	class layout
		: public ctrl
	{
	public:
		layout(nana::window wd, const std::string& name);

		void update() override;

		void generatecode(code_data_struct* cd, code_info_struct* ci) override;
		
		void updatefield(nana::window child, const std::string& weight, const std::string& margin);

		bool append(nana::window child);
		bool remove(nana::window child);

		bool moveup(nana::window child);
		bool movedown(nana::window child);


	protected:
		nana::layout	lyt;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_LAYOUT_H
