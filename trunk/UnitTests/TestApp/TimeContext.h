/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __TIME_CONTEXT_H__
#define __TIME_CONTEXT_H__

#include <MyGUI.h>

namespace context
{

	namespace detail
	{
		class TimeHolder
		{
		public:
			TimeHolder()
			{
				mCurrentTime = 1;
				MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &TimeHolder::frameStart);
			}

			float getCurrentTime()
			{
				return mCurrentTime;
			}

		private:
			void frameStart(float _time)
			{
				mCurrentTime += _time;
			}

		private:
			float mCurrentTime;
		};
	}

	class TimeContext
	{
	public:
		static float getCurrentTime()
		{
			static detail::TimeHolder holder;
			return holder.getCurrentTime();
		}
	};

} // namespace context

#endif // __TIME_CONTEXT_H__
