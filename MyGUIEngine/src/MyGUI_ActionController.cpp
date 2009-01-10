/*!
	@file
	@author		Albert Semenov
	@date		03/2008
	@module
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_ActionController.h"
#include "MyGUI_Widget.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	namespace action
	{

		void actionWidgetHide(WidgetPtr _widget)
		{
			_widget->hide();
		}

		void actionWidgetDestroy(WidgetPtr _widget)
		{
			WidgetManager::getInstance().destroyWidget(_widget);
		}

		void linearMoveFunction(const IntCoord & _startRect, const IntCoord & _destRect, IntCoord & _result, float _k){
			_result.set(_startRect.left   - int( float(_startRect.left   - _destRect.left)   * _k ),
						_startRect.top    - int( float(_startRect.top    - _destRect.top)    * _k ),
						_startRect.width  - int( float(_startRect.width  - _destRect.width)  * _k ),
						_startRect.height - int( float(_startRect.height - _destRect.height) * _k )
			);
		}

		void inertionalMoveFunction(const IntCoord & _startRect, const IntCoord & _destRect, IntCoord & _result, float _current_time)
		{
			#ifndef M_PI
			const float M_PI = 3.141593;
			#endif
			float k = sin(M_PI * _current_time - M_PI/2);
			if (k<0) k = (-pow((-k), (float)0.7) + 1)/2;
			else k = (pow((k), (float)0.7) + 1)/2;
			linearMoveFunction(_startRect, _destRect, _result, k);
		}

	} // namespace action

} // namespace MyGUI
