/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _05ab6d18_ce9a_43df_93dc_98653055410f_
#define _05ab6d18_ce9a_43df_93dc_98653055410f_

#include "BaseLayout/BaseLayout.h"

namespace tools
{

	class MessageBoxFadeControl :
		public wraps::BaseLayout
	{
	public:
		MessageBoxFadeControl();
		virtual ~MessageBoxFadeControl();

	private:
		void notifyFrameStart(float _time);

	private:
		float mMaxAlpha;
	};

}

#endif
