/*!
	@file
	@author		Albert Semenov
	@date		01/2010
	@module
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
#ifndef __MYGUI_WRAP_PANEL_H__
#define __MYGUI_WRAP_PANEL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Panel.h"
#include "MyGUI_FlowDirection.h"

namespace MyGUI
{

	class MYGUI_EXPORT WrapPanel :
		public Panel
	{
		MYGUI_RTTI_DERIVED( WrapPanel );

	public:
		WrapPanel();

		void setFlowDirection(FlowDirection _value);
		FlowDirection getFlowDirection() { return mFlowDirection; }

		void setHorizontalSpacer(int _value);
		int getHorizontalSpacer() { return mHorizontalSpacer; }

		void setVerticalSpacer(int _value);
		int getVerticalSpacer() { return mVerticalSpacer; }

		void setItemWidth(int _value);
		int getItemWidth() { return mItemWidth; }

		void setItemHeight(int _value);
		int getItemHeight() { return mItemHeight; }

		virtual void setProperty(const std::string& _key, const std::string& _value);

	protected:
		virtual void overrideMeasure(const IntSize& _sizeAvailable);
		virtual void overrideArrange(const IntSize& _sizeOld);

	private:
		IntSize getMaxDistance(EnumeratorWidgetPtr _child, const IntSize& _max);

	private:
		FlowDirection mFlowDirection;
		int mHorizontalSpacer;
		int mVerticalSpacer;
		int mItemWidth;
		int mItemHeight;
	};

} // namespace MyGUI

#endif // __MYGUI_STACK_PANEL_H__
