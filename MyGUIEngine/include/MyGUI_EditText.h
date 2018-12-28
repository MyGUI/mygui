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

		void setVisible(bool _value) override;

		// обновляет все данные связанные с тектом
		virtual void updateRawData();

		// метод для отрисовки себя
		void doRender() override;

		void setCaption(const UString& _value) override;
		const UString& getCaption() const override;

		void setTextColour(const Colour& _value)override;
		const Colour& getTextColour() const override;

		void setAlpha(float _value) override;

		void setFontName(const std::string& _value) override;
		const std::string& getFontName() const override;

		void setFontHeight(int _value) override;
		int getFontHeight() const override;

		void createDrawItem(ITexture* _texture, ILayerNode* _node) override;
		void destroyDrawItem() override;

		void setTextAlign(Align _value) override;
		Align getTextAlign() const override;

		size_t getTextSelectionStart() const override;
		size_t getTextSelectionEnd() const override;
		void setTextSelection(size_t _start, size_t _end) override;

		bool getSelectBackground() const override;
		void setSelectBackground(bool _normal) override;

		bool isVisibleCursor() const override;
		void setVisibleCursor(bool _value) override;

		/** Get invert selected text color property. */
		bool getInvertSelected() const override;
		/** Enable or disable inverting color of selected text.\n
			Enabled (true) by default.
		*/
		void setInvertSelected(bool _value) override;

		size_t getCursorPosition() const override;
		void setCursorPosition(size_t _index) override;

		IntSize getTextSize() override;

		// устанавливает смещение текста в пикселях
		void setViewOffset(const IntPoint& _point) override;
		IntPoint getViewOffset() const override;

		// возвращает положение курсора по произвольному положению
		size_t getCursorPosition(const IntPoint& _point) override;

		// возвращает положение курсора в обсолютных координатах
		IntCoord getCursorCoord(size_t _position) override;

		bool getShadow() const override;
		void setShadow(bool _value) override;

		void setShiftText(bool _shift) override;

		void setWordWrap(bool _value) override;

		void setStateData(IStateInfo* _data) override;

		void setShadowColour(const Colour& _value) override;
		const Colour& getShadowColour() const override;

		/*internal:*/
		void _updateView() override;
		void _correctView() override;

		void _setAlign(const IntSize& _oldsize) override;

		virtual const VectorLineInfo& getLineInfo() const;

	private:
		void _setTextColour(const Colour& _value);
		void checkVertexSize();
		unsigned int getMixedNativeAlpha(float secondAlpha);

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
