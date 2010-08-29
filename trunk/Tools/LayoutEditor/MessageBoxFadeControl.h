/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MESSAGE_BOX_FADE_CONTROL_H__
#define __MESSAGE_BOX_FADE_CONTROL_H__

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

} // namespace tools

#endif // __MESSAGE_BOX_FADE_CONTROL_H__
