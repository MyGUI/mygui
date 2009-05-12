/*!
	@file
	@author		Albert Semenov
	@date		05/2009
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
#ifndef __MYGUI_OGRE_CONVERT_VALUE_H__
#define __MYGUI_OGRE_CONVERT_VALUE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ConvertValue.h"

#include <OgreColourValue.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	template<> struct Convert<Colour, Ogre::ColourValue>
	{
		inline static Colour From(const Ogre::ColourValue& _value) { return Colour(_value.r, _value.g, _value.b, _value.a); }
		inline static Ogre::ColourValue To(const Colour& _value) { return Ogre::ColourValue(_value.red, _value.green, _value.blue, _value.alpha); }
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_CONVERT_VALUE_H__
