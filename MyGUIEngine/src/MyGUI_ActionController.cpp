/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ActionController.h"
#include "MyGUI_Widget.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	namespace action
	{

		void actionWidgetHide(Widget* _widget, ControllerItem* _controller)
		{
			_widget->setVisible(false);
		}

		void actionWidgetShow(Widget* _widget, ControllerItem* _controller)
		{
			_widget->setVisible(true);
		}

		void actionWidgetDestroy(Widget* _widget, ControllerItem* _controller)
		{
			WidgetManager::getInstance().destroyWidget(_widget);
		}

		void linearMoveFunction(const IntCoord& _startRect, const IntCoord& _destRect, IntCoord& _result, float _k)
		{
			_result.set(
				_startRect.left   - int( float(_startRect.left   - _destRect.left)   * _k ),
				_startRect.top    - int( float(_startRect.top    - _destRect.top)    * _k ),
				_startRect.width  - int( float(_startRect.width  - _destRect.width)  * _k ),
				_startRect.height - int( float(_startRect.height - _destRect.height) * _k ));
		}

		void inertionalMoveFunction(const IntCoord& _startRect, const IntCoord& _destRect, IntCoord& _result, float _current_time)
		{
#ifndef M_PI
			const float M_PI = 3.141593f;
#endif
			float k = sin(M_PI * _current_time - M_PI / 2.0f);
			if (k < 0) k = (-pow(-k, 0.7f) + 1) / 2;
			else k = (pow(k, 0.7f) + 1) / 2;
			linearMoveFunction(_startRect, _destRect, _result, k);
		}

	} // namespace action

} // namespace MyGUI
