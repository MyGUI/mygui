/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_TEXT_EDIT_OVERLAY_ELEMENT_H__
#define __MYGUI_TEXT_EDIT_OVERLAY_ELEMENT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Font.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_AlignInfo.h"
#include "MyGUI_EnumCharInfo.h"

#include <OgreOverlayElement.h>

namespace MyGUI
{

	class _MyGUIExport TextEditOverlayElement : public Ogre::OverlayElement
	{

	protected:
		enum {
			OVERLAY_POSITION_BINDING = 0,
			OVERLAY_DEFAULT_INITIAL_CHARS = 12
		};

		/// Flag indicating if this panel should be visual or just group things
        bool mTransparent;

        /// Render operation
        Ogre::RenderOperation mRenderOp;

        static Ogre::String msTypeName;

        Ogre::Real mCharHeight;
        Ogre::ushort mPixelCharHeight;
        size_t mAllocSize;
		Ogre::Real mViewportAspectCoef;

		IntRect mMargin; // перекрытие
		Align mAlign;

		bool mRenderGL;// для конвертирования цвета вершин
		Ogre::RGBA mDefaultColour; // цвет текста
		Ogre::RGBA mInverseColour; // инверсный цвет текста
		size_t mStartSelect, mEndSelect; // начало и конец выделения
		IntPoint mPointShift; // смещение текста
		FloatSize mContextSize; // размер всего текста
		VectorLineInfo mLinesInfo;
		bool mRawDataOutOfDate;
		float mOldViewportAspectCoef; // последний коээфициент
		float mTextureHeightOne, mTextureWidthOne; // размер одной текстурной единицы
        FontPtr mpFont;
		FloatPoint mBackgroundEmpty, mBackgroundFill, mBackgroundFillDeactive;
		Font::GlyphInfo * mSpaceGlyphInfo;
		Font::GlyphInfo * mTabGlyphInfo;
		bool mBackgroundNormal;

	public:
        /** Constructor. */
        TextEditOverlayElement(const Ogre::String& name);
        virtual ~TextEditOverlayElement();

        virtual void initialise(void);
		virtual void setCaption(const Ogre::DisplayString& text);

        void setCharHeight( Ogre::Real height );
        Ogre::Real getCharHeight() const;

        void setFontName( const Ogre::String& font );
        const Ogre::String& getFontName() const;

        /** See OverlayElement. */
        virtual const Ogre::String& getTypeName(void) const;
        /** See Renderable. */
        void getRenderOperation(Ogre::RenderOperation& op);
        /** Overridden from OverlayElement */
        void setMaterialName(const Ogre::String& matName);

        /** Sets the colour of the text. 
        @remarks
            This method establishes a constant colour for 
            the entire text. Also see setColourBottom and 
            setColourTop which allow you to set a colour gradient.
        */
        void setColour(const Ogre::ColourValue& col);

        /** Gets the colour of the text. */
        const Ogre::ColourValue& getColour(void) const;

        /** Overridden from OverlayElement */
        virtual void setMetricsMode(Ogre::GuiMetricsMode gmm);

        /** Overridden from OverlayElement */
        virtual void _update(void);

        /** Overridden from OverlayElement */
		virtual void _notifyViewport();

        /// Method for setting up base parameters for this class
		// переопределяем для пустоты
        void addBaseParameters(void);

        /// Internal method to allocate memory, only reallocates when necessary
        void checkMemoryAllocation( size_t numChars );
        /// Inherited function
        virtual void updatePositionGeometry();
		/// Inherited function
		virtual void updateTextureGeometry();

		/// Inherited function
        virtual void _updateFromParent(void);

		IntSize getTextSize(const Ogre::DisplayString& _text);

		void updateRawData();

		size_t getTextCursorFromPoint(IntPoint & _point);

		IntPoint getTextCursorFromPosition(size_t _position);

		inline size_t getSelectStart()
		{
			return mStartSelect;
		}

		inline size_t getSelectEnd()
		{
			return mEndSelect;
		}

		inline void setSelect(size_t _start, size_t _end)
		{
			mStartSelect=_start;
			mEndSelect=_end;
			mGeomPositionsOutOfDate = true;
		}

		inline bool getSelectBackground()
		{
			return mBackgroundNormal;
		}

		inline void setSelectBackground(bool _normal)
		{
			if (mBackgroundNormal == _normal) return;
			mBackgroundNormal = _normal;
			mGeomPositionsOutOfDate = true;
		}

		// необходимо обновить все что связанно с стекстом
		inline void setAlignment(Align _align)
		{
			// выравнивание бокса
			mAlign = _align;
			mGeomPositionsOutOfDate = true;
		}

		// устанавливет размеры по которым резать текст
		inline void setMargin(const IntRect& _margin)
		{
			mMargin = _margin;
			mDerivedOutOfDate = true;
		}

		// возвращает размер текста в пикселях
		inline IntSize getTextSize()
		{
			// если нуно обновить, или изменились пропорции экрана
			updateRawData();
			return IntSize( (int)(mContextSize.width / (mPixelScaleX * 2.0)), (int)(mLinesInfo.size() * mPixelCharHeight) );
		}

		// устанавливает смещение текста в пикселях
		inline void setTextShift(IntPoint _point)
		{
			mPointShift = _point;
			mRawDataOutOfDate = true;
			mGeomPositionsOutOfDate = true;
		}

		inline IntPoint getTextShift()
		{
			return mPointShift;
		}

	}; // class TextEditOverlayElement : public Ogre::OverlayElement

} // namespace MyGUI

#endif // __MYGUI_TEXT_EDIT_OVERLAY_ELEMENT_H__
