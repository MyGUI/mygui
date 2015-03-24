/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TEXT_VIEW_H_
#define MYGUI_TEXT_VIEW_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_TextureUtility.h"
#include "MyGUI_TextViewData.h"
#include "MyGUI_IFont.h"

namespace MyGUI
{

	class TextView
	{
	public:
		TextView();

		void update(const UString& _text, IFont* _font, int _height, Align _align, VertexColourType _format, int _maxWidth = -1);

		size_t getCursorPosition(const IntPoint& _value);

		IntPoint getCursorPoint(size_t _position);

		const IntSize& getViewSize() const;

		size_t getTextLength() const;

		const VectorLineInfo& getData() const;

	private:
		IntSize mViewSize;
		size_t mLength;
		VectorLineInfo mLineInfo;
		int mFontHeight;
	};

} // namespace MyGUI

#endif // MYGUI_TEXT_VIEW_H_
