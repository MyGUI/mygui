/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_FONT_H__
#define __MYGUI_FONT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

#include <OgreResource.h>
#include <OgreTexture.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class _MyGUIExport Font : public Ogre::Resource, public Ogre::ManualResourceLoader
    {

	public:
		typedef Ogre::uint32 CodePoint;
		typedef Ogre::FloatRect UVRect;
		typedef std::pair<CodePoint, CodePoint> PairCodePoint;
		typedef std::vector<PairCodePoint> VectorPairCodePoint;

		// информация об одном символе
		struct GlyphInfo
		{
			CodePoint codePoint;
			UVRect uvRect;
			Ogre::Real aspectRatio;

			GlyphInfo() : codePoint(0), uvRect(UVRect(0, 0, 0, 0)), aspectRatio(1) { }
			GlyphInfo(CodePoint _code, const UVRect& _rect, Ogre::Real _aspect) : codePoint(_code), uvRect(_rect), aspectRatio(_aspect) { }
		};

		typedef std::vector<GlyphInfo> VectorGlyphInfo;

		// инфомация о диапазоне символов
		struct RangeInfo
		{
			CodePoint first;
			CodePoint second;
			VectorGlyphInfo range;

			RangeInfo(CodePoint _first, CodePoint _second) : first(_first), second(_second) { }
		};

		typedef std::vector<RangeInfo> VectorRangeInfo;

		// constants
		enum constCodePoints
		{
			FONT_CODE_SELECT = 0,
			FONT_CODE_TAB = 0x0009,
			FONT_CODE_LF = 0x000A,
			FONT_CODE_CR = 0x000D,
			FONT_CODE_SPACE = 0x0020,
			FONT_CODE_LATIN_START = 0x0021,
			FONT_CODE_NEL = 0x0085,
			FONT_CODE_LATIN_END = 0x00A6,
		};

	protected:

        /// Source of the font (either an image name or a truetype font)
		Ogre::String mSource;
        /// Size of the truetype font, in points
		Ogre::Real mTtfSize;
        /// Resolution (dpi) of truetype font
		Ogre::uint mTtfResolution;

		Ogre::uint8 mDistance;
		Ogre::uint8 mSpaceWidth;
		Ogre::uint8 mTabWidth;
		Ogre::uint8 mCursorWidth;
		Ogre::uint8 mOffsetHeight;
		Ogre::uint16 mDefaultHeight;
		Ogre::uint16 mHeightPix;

		// отдельная информация о символах
		GlyphInfo mSpaceGlyphInfo, mTabGlyphInfo, mSelectGlyphInfo, mSelectDeactiveGlyphInfo, mCursorGlyphInfo;

		// символы которые не нужно рисовать
		VectorPairCodePoint mVectorHideCodePoint;

	protected:

		// вся информация о символах
		VectorRangeInfo mVectorRangeInfo;

		/// Texture pointer
		Ogre::TexturePtr mTexture;

        /// for TRUE_TYPE font only
        bool mAntialiasColour;

		/// @copydoc Ogre::Resource::loadImpl
		virtual void loadImpl();
		/// @copydoc Ogre::Resource::unloadImpl
		virtual void unloadImpl();
		/// @copydoc Ogre::Resource::calculateSize
		size_t calculateSize(void) const { return 0; } // permanent resource is in the texture

	public:

        /** Constructor.
		@see Resource */
		Font(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0);
        virtual ~Font();

		GlyphInfo * getSpaceGlyphInfo() {return & mSpaceGlyphInfo;}
		GlyphInfo * getTabGlyphInfo() {return & mTabGlyphInfo;}
		GlyphInfo * getSelectGlyphInfo() {return & mSelectGlyphInfo;}
		GlyphInfo * getSelectDeactiveGlyphInfo() {return & mSelectDeactiveGlyphInfo;}
		GlyphInfo * getCursorGlyphInfo() {return & mCursorGlyphInfo;}

		void setSource(const Ogre::String& source) { mSource = source; }
		const Ogre::String& getSource(void) const { return mSource; }

		void setTrueTypeSize(Ogre::Real ttfSize) { mTtfSize = ttfSize; }
		Ogre::Real getTrueTypeSize(void) const { return mTtfSize; }

		void setTrueTypeResolution(Ogre::uint ttfResolution) { mTtfResolution = ttfResolution; }
		Ogre::uint getTrueTypeResolution(void) const { return mTtfResolution; }

		GlyphInfo * getGlyphInfo(CodePoint _id);

		void addCodePointRange(Ogre::Real _first, Ogre::Real _second)
		{
			mVectorRangeInfo.push_back(RangeInfo((Ogre::uint32)_first, (Ogre::uint32)_second));
		}

		void addHideCodePointRange(Ogre::Real _first, Ogre::Real _second)
		{
			mVectorHideCodePoint.push_back(PairCodePoint((unsigned int)_first, (unsigned int)_second));
		}

		// проверяет, входит ли символ в зоны ненужных символов
		bool checkHidePointCode(CodePoint _id)
		{
			for (VectorPairCodePoint::iterator iter=mVectorHideCodePoint.begin(); iter!=mVectorHideCodePoint.end(); ++iter) {
				if ((_id >= iter->first) && (_id <= iter->second)) return true;
			}
			return false;
		}

		/** Clear the list of code point ranges. */
		void clearCodePointRanges()
		{
			mVectorRangeInfo.clear();
			mVectorHideCodePoint.clear();
		}

		const Ogre::TexturePtr& getTextureFont() const { return mTexture; }
		const Ogre::TexturePtr& getTextureFont() { return mTexture; }

        void setAntialiasColour(bool enabled) { mAntialiasColour = enabled; }
        bool getAntialiasColour(void) const { return mAntialiasColour; }

		Ogre::uint16 getDefaultHeight() const { return mDefaultHeight; }
		void setDefaultHeight(Ogre::uint16 _height) { mDefaultHeight = _height; }

		Ogre::uint16 getHeightPix() { return mHeightPix; }

		/** Implementation of ManualResourceLoader::loadResource, called
			when the Texture that this font creates needs to (re)load. */
		void loadResource(Ogre::Resource* resource);

		//ширина пробела в пикселях
		CodePoint setSpaceWidth() {return mSpaceWidth;}
		void setSpaceWidth(Ogre::uint8 _pix) {mSpaceWidth = _pix;}

		// ширина таба в пикселях
		Ogre::uint8 getTabWidth() {return mTabWidth;}
		void setTabWidth(Ogre::uint8 _pix) {mTabWidth = _pix;}

		// ширина курсора в пикселях
		Ogre::uint8 getCursorWidth() {return mCursorWidth;}
		void setCursorWidth(Ogre::uint8 _pix) {mCursorWidth = _pix;}

		// расстояние между символами при генерации в пикселях
		Ogre::uint8 getDistance() {return mDistance;}
		void setDistance(Ogre::uint8 _pix) {mDistance = _pix;}

		// смещение всех символов по горизонтали
		Ogre::uint8 getOffsetHeight() {return mOffsetHeight;}
		void setOffsetHeight(Ogre::uint8 _pix) {mOffsetHeight = _pix;}

    };


	/** Specialisation of SharedPtr to allow SharedPtr to be assigned to FontPtr
	@note Has to be a subclass since we need operator=.
	We could templatise this instead of repeating per Resource subclass,
	except to do so requires a form VC6 does not support i.e.
	ResourceSubclassPtr<T> : public SharedPtr<T>
	*/

	class _MyGUIExport FontPtr : public Ogre::SharedPtr<Font>
	{
	public:
		FontPtr() : Ogre::SharedPtr<Font>() {}
		explicit FontPtr(Font* rep) : Ogre::SharedPtr<Font>(rep) {}
		FontPtr(const FontPtr& r) : Ogre::SharedPtr<Font>(r) {}
		FontPtr(const Ogre::ResourcePtr& r) : Ogre::SharedPtr<Font>()
		{
			// lock & copy other mutex pointer
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			    pRep = static_cast<Font*>(r.getPointer());
			    pUseCount = r.useCountPointer();
			    if (pUseCount)
			    {
				    ++(*pUseCount);
			    }
            }
		}

		/// Operator used to convert a ResourcePtr to a FontPtr
		FontPtr& operator=(const Ogre::ResourcePtr& r)
		{
			if (pRep == static_cast<Font*>(r.getPointer()))
				return *this;
			release();
			// lock & copy other mutex pointer
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
                OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			    pRep = static_cast<Font*>(r.getPointer());
			    pUseCount = r.useCountPointer();
			    if (pUseCount)
			    {
				    ++(*pUseCount);
			    }
            }
			else
			{
				// RHS must be a null pointer
				MYGUI_ASSERT(r.isNull(), "RHS must be null if it has no mutex!");
				setNull();
			}
			return *this;
		}
	}; // class _MyGUIExport FontPtr : public Ogre::SharedPtr<Font>

} // namespace MyGUI

#endif // __MYGUI_FONT_H__
