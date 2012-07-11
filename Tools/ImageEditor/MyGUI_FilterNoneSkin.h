/*!
	@file
	@author		Albert Semenov
	@date		08/2010
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
#ifndef __MYGUI_FILTER_NONE_SKIN_H__
#define __MYGUI_FILTER_NONE_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_SubSkin.h"

namespace MyGUI
{

	class /*MYGUI_EXPORT */FilterNone :
		public SubSkin
	{
		MYGUI_RTTI_DERIVED( FilterNone )

	public:
		FilterNone();
		virtual ~FilterNone();

		virtual void doManualRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

	/*internal:*/
		virtual void _setAlign(const IntSize& _oldsize);
	};

} // namespace MyGUI

#endif // __MYGUI_FILTER_NONE_SKIN_H__
