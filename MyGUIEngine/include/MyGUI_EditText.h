/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_EDIT_TEXT_H_
#define MYGUI_EDIT_TEXT_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_IFont.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_TextView.h"
#include "MyGUI_VertexData.h"

namespace MyGUI
{

	class RenderItem;

	class MYGUI_EXPORT EditText :
		public ISubWidgetText
	{
		MYGUI_RTTI_DERIVED( EditText )

	public:
		EditText();
		virtual ~EditText();

		virtual void setVisible(bool _value);

		// обновляет все данные связанные с тектом
		virtual void updateRawData();

		// метод для отрисовки себя
		virtual void doRender();

		void setCaption(const UString& _value);
		const UString& getCaption() const;

		void setTextColour(const Colour& _value);
		const Colour& getTextColour() const;

		void setAlpha(float _value);
		float getAlpha() const;

		virtual void setFontName(const std::string& _value);
		virtual const std::string& getFontName() const;

		virtual void setFontHeight(int _value);
		virtual int getFontHeight() const;

		virtual void createDrawItem(ITexture* _texture, ILayerNode* _node);
		virtual void destroyDrawItem();

		virtual void setTextAlign(Align _value);
		virtual Align getTextAlign() const;

		virtual size_t getTextSelectionStart() const;
		virtual size_t getTextSelectionEnd() const;
		virtual void setTextSelection(size_t _start, size_t _end);

		virtual bool getSelectBackground() const;
		virtual void setSelectBackground(bool _normal);

		virtual bool isVisibleCursor() const;
		virtual void setVisibleCursor(bool _value);

		/** Get invert selected text color property. */
		virtual bool getInvertSelected() const;
		/** Enable or disable inverting color of selected text.\n
			Enabled (true) by default.
		*/
		virtual void setInvertSelected(bool _value);

		virtual size_t getCursorPosition() const;
		virtual void setCursorPosition(size_t _index);

		virtual IntSize getTextSize();

		// устанавливает смещение текста в пикселях
		virtual void setViewOffset(const IntPoint& _point);
		virtual IntPoint getViewOffset() const;

		// возвращает положение курсора по произвольному положению
		virtual size_t getCursorPosition(const IntPoint& _point);

		// возвращает положение курсора в обсолютных координатах
		virtual IntCoord getCursorCoord(size_t _position);

		virtual bool getShadow() const;
		virtual void setShadow(bool _value);

		virtual void setShiftText(bool _shift);

		virtual void setWordWrap(bool _value);

		virtual void setStateData(IStateInfo* _data);

		virtual void setShadowColour(const Colour& _value);
		virtual const Colour& getShadowColour() const;

		/*internal:*/
		virtual void _updateView();
		virtual void _correctView();

		virtual void _setAlign(const IntSize& _oldsize);

		virtual const VectorLineInfo& getLineInfo() const;

	private:
		void _setTextColour(const Colour& _value);
		void checkVertexSize();

		void drawQuad(
			Vertex*& _vertex,
			size_t& _vertexCount,
			const FloatRect& _vertexRect,
			float _vertexZ,
			const FloatRect& _textureRect,
			uint32 _colour) const;

		void drawGlyph(
			const RenderTargetInfo& renderTargetInfo,
			Vertex*& _vertex,
			size_t& _vertexCount,
			FloatRect _vertexRect,
			FloatRect _textureRect,
			uint32 _colour) const;

	protected:
		bool mEmptyView;
		uint32 mCurrentColourNative;
		uint32 mInverseColourNative;
		uint32 mCurrentAlphaNative;
		uint32 mShadowColourNative;
		IntCoord mCurrentCoord;

		UString mCaption;
		bool mTextOutDate;
		Align mTextAlign;

		Colour mColour;
		Colour mShadowColour;
		float mAlpha;
		VertexColourType mVertexFormat;

		IFont* mFont;
		ITexture* mTexture;
		int mFontHeight;

		bool mBackgroundNormal;
		size_t mStartSelect;
		size_t mEndSelect;
		size_t mCursorPosition;
		bool mVisibleCursor;
		bool mInvertSelect;
		bool mShadow;

		IntPoint mViewOffset; // смещение текста

		ILayerNode* mNode;
		RenderItem* mRenderItem;
		size_t mCountVertex;
		bool mIsAddCursorWidth;

		bool mShiftText;
		bool mWordWrap;
		bool mManualColour;
		int mOldWidth;

		TextView mTextView;
	};

} // namespace MyGUI

#endif // MYGUI_EDIT_TEXT_H_
