/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _05ab6d18_ce9a_43df_93dc_98653055410f_
#define _05ab6d18_ce9a_43df_93dc_98653055410f_

#include "Control.h"

namespace tools
{

	class MYGUI_EXPORT_DLL MessageBoxFadeControl :
		public Control
	{
	public:
		MessageBoxFadeControl();
		virtual ~MessageBoxFadeControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyFrameStart(float _time);

	private:
		float mMaxAlpha;
	};

}

#endif
