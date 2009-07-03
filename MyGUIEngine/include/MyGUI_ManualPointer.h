/*!
	@file
	@author		Albert Semenov
	@date		06/2009
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
#ifndef __MYGUI_MANUAL_POINTER_H__
#define __MYGUI_MANUAL_POINTER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IPointer.h"

namespace MyGUI
{

	class MYGUI_EXPORT ManualPointer : public IPointer
    {
		MYGUI_RTTI_DERIVED( ManualPointer );

	public:
		ManualPointer() { }
		virtual ~ManualPointer() { }

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		virtual void setImage(StaticImagePtr _image);
		virtual void setPosition(StaticImagePtr _image, const IntPoint& _point);

	private:
		IntPoint mPoint;
		IntSize mSize;
		FloatRect mOffset;
		std::string mTexture;
    };

} // namespace MyGUI

#endif // __MYGUI_MANUAL_POINTER_H__
