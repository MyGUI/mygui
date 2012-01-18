/*!
	@file
	@author		Albert Semenov
	@date		09/2009
*/
#ifndef __MYGUI_TEXT_VIEW_H__
#define __MYGUI_TEXT_VIEW_H__

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

#endif // __MYGUI_TEXT_VIEW_H__
