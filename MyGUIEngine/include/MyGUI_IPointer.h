/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_POINTER_H_
#define MYGUI_I_POINTER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IResource.h"

namespace MyGUI
{

	class MYGUI_EXPORT IPointer :
		public IResource
	{
		MYGUI_RTTI_DERIVED( IPointer )

	public:
		IPointer() { }
		virtual ~IPointer() { }

		virtual void setImage(ImageBox* _image) = 0;
		virtual void setPosition(ImageBox* _image, const IntPoint& _point) = 0;
	};

} // namespace MyGUI

#endif // MYGUI_I_POINTER_H_
