/*!
	@file
	@author		Albert Semenov
	@date		01/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_STACK_PANEL_H__
#define __MYGUI_STACK_PANEL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Panel.h"
#include "MyGUI_FlowDirection.h"

namespace MyGUI
{

	class MYGUI_EXPORT StackPanel :
		public Panel
	{
		MYGUI_RTTI_DERIVED( StackPanel )

	public:
		StackPanel();

		void setFlowDirection(FlowDirection _value);
		FlowDirection getFlowDirection() { return mFlowDirection; }

		void setUniform(bool _value);
		bool getUniform() { return mUniform; }

		void setSpacer(int _value);
		int getSpacer() { return mSpacer; }

		virtual void setProperty(const std::string& _key, const std::string& _value);

	protected:
		virtual IntSize overrideMeasure(const IntSize& _sizeAvailable);
		virtual void overrideArrange(const IntSize& _sizeOld);

	private:
		FlowDirection mFlowDirection;
		bool mUniform;
		IntSize mMaxItemSize;
		int mSpacer;
	};

} // namespace MyGUI

#endif // __MYGUI_STACK_PANEL_H__
