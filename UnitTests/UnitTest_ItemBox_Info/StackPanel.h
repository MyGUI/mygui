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
#ifndef __MYGUI_STACK_PANEL_H__
#define __MYGUI_STACK_PANEL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class /*MYGUI_EXPORT */StackPanel :
		public Widget
	{
		MYGUI_RTTI_DERIVED( StackPanel );

	public:
		StackPanel();

		void setOrientation(bool _vert);

		virtual const IntSize& updateMeasure(const IntSize& _sizeAvailable);
		virtual void updateArrange(const IntSize& _sizeFinal);

		/** Set content align */
		//void setContentAlign(Align _value);
		/** Get content align */
		//Align getContentAlign() { return mContentAlign; }

		void setUniform(bool _value);
		bool getUniform() { return mUniform; }

		void setSpacer(int _value);
		int getSpacer() { return mSpacer; }

	protected:
		//virtual Widget* baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name);

	private:
		bool mVertOrientation;
		bool mUniform;
		//Align mContentAlign;
		IntSize mMaxItemSize;
		int mSpacer;
	};

} // namespace MyGUI

#endif // __MYGUI_STACK_PANEL_H__
