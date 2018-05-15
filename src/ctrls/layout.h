/*
 *		ctrls::layout Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_LAYOUT_H
#define NANA_CREATOR_LAYOUT_H

#include <nana/gui/widgets/widget.hpp>
#include "ctrls/ctrl.h"
#include "ctrls/box_model.h"
#include "codegenerator_data.h"


namespace nana
{

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
		
		void updatefield(nana::window ctrl, const std::string& weight, const std::string& margin);

		bool children();
		bool append(nana::window ctrl);
		bool insert(nana::window pos, nana::window ctrl, bool after = true);
		bool remove(nana::window ctrl);

		bool moveup(nana::window ctrl);
		bool movedown(nana::window ctrl);


	protected:
		nana::layout		lyt;
		box_model			boxmodel;
	};

}//end namespace ctrls

#endif //NANA_CREATOR_LAYOUT_H
