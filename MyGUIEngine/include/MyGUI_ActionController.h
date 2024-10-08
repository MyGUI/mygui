/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_ACTION_CONTROLLER_H_
#define MYGUI_ACTION_CONTROLLER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include <cmath>

namespace MyGUI
{
	class ControllerItem;

	namespace action
	{

		/** Function used for hiding widget with one of controller event */
		void MYGUI_EXPORT actionWidgetHide(Widget* _widget, ControllerItem* _controller);

		/** Function used for showing widget with one of controller event */
		void MYGUI_EXPORT actionWidgetShow(Widget* _widget, ControllerItem* _controller);

		/** Function used for destroying widget with one of controller event */
		void MYGUI_EXPORT actionWidgetDestroy(Widget* _widget, ControllerItem* _controller);

		/** Function for ControllerPosition per frame action : Move with constant speed */
		void MYGUI_EXPORT
		linearMoveFunction(const IntCoord& _startRect, const IntCoord& _destRect, IntCoord& _result, float _k);

		/** Function for ControllerPosition per frame action : Move with accelerated speed
			if N == 10 then this function is same as linearMoveFunction
			if N > 10 speed will be increasing
			if N < 10 speed will be decreasing
		*/
		template<int N>
		inline void acceleratedMoveFunction(
			const IntCoord& _startRect,
			const IntCoord& _destRect,
			IntCoord& _result,
			float _current_time)
		{
			float k = std::pow(_current_time, N / 10.f /*3 by default as Accelerated and 0.4 by default as Slowed*/);
			linearMoveFunction(_startRect, _destRect, _result, k);
		}

		/** Function for ControllerPosition per frame action : Move with accelerated speed a bit farther than necessary and then return it back */
		template<int N>
		inline void jumpMoveFunction(
			const IntCoord& _startRect,
			const IntCoord& _destRect,
			IntCoord& _result,
			float _current_time)
		{
			float k = std::pow(_current_time, 2.0f) * (-2 - N / 10.f) + _current_time * (3 + N / 10.f);
			linearMoveFunction(_startRect, _destRect, _result, k);
		}

		/** Function for ControllerPosition per frame action : Start with zero speed increasing half time and then decreasing to zero */
		void MYGUI_EXPORT inertionalMoveFunction(
			const IntCoord& _startRect,
			const IntCoord& _destRect,
			IntCoord& _result,
			float _current_time);

	} // namespace action

} // namespace MyGUI

#endif // MYGUI_ACTION_CONTROLLER_H_
