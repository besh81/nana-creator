/*
 *		ctrls::ctrl_struct Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_CTRL_H
#define NANA_CREATOR_CTRL_H

#include "config.h"
#include <memory>
#include <nana/gui/wvl.hpp>
#include "ctrls/property.h"
#include "codegenerator_data.h"


namespace ctrls
{

	class ctrl
	{
	public:
		properties_collection	properties;
		nana::widget*			nanawdg;


		ctrl() = default;

		virtual void update();

		virtual void generatecode(code_data_struct* cd, code_info_struct* ci);
		
		bool highlighted() { return _ishighlighted; }

		void set_highlight(const nana::color& color)
		{
			// reset bgcolor highlight
			_ishighlighted = true;
			_bgcolor = nanawdg->bgcolor();
			nanawdg->bgcolor(color);
		}

		void reset_highlight()
		{
			// reset bgcolor highlight
			_ishighlighted = false;
			nanawdg->bgcolor(_bgcolor);
		}


	protected:
		void init(nana::widget* wdg, const std::string& type, const std::string& name);


	private:
		nana::color		_bgcolor;
		bool			_ishighlighted{ false };
	};

}//end namespace ctrls


typedef std::shared_ptr<ctrls::ctrl>	control_obj;
typedef std::weak_ptr<ctrls::ctrl>		control_obj_ptr;


#endif //NANA_CREATOR_CTRL_H
