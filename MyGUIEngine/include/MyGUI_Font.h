/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
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
#ifndef __MYGUI_FONT_H__
#define __MYGUI_FONT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

#include <OgreResource.h>
#include <OgreTexture.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class MYGUI_EXPORT Font : public Ogre::Resource, public Ogre::ManualResourceLoader
    {

	public:

		// информация об диапазоне
		struct PairCodePoint
		{
			PairCodePoint(Char _first, Char _last) :
				first(_first),
				last(_last)
			{
			}

			// проверяет входит ли символ в диапазон
			bool isExist(Char _code) { return _code >= first && _code <= last; }

		public:
			Char first;
			Char last;
		};

		typedef std::vector<PairCodePoint> VectorPairCodePoint;

		// информация об одном символе
		struct GlyphInfo
		{
			Char codePoint;
			FloatRect uvRect;
			float aspectRatio;

			GlyphInfo() :
				codePoint(0),
				aspectRatio(1)
			{
			}

			GlyphInfo(Char _code, const FloatRect& _rect, float _aspect) :
				codePoint(_code),
				uvRect(_rect),
				aspectRatio(_aspect)
			{
			}

		};

		typedef std::vector<GlyphInfo> VectorGlyphInfo;

		// инфомация о диапазоне символов
		struct RangeInfo
		{
		private:
			RangeInfo() { }

		public:
			RangeInfo(Char _first, Char _last) :
				first(_first),
				last(_last)
			{
				range.resize(last - first + 1);
			}

			// проверяет входит ли символ в диапазон
			bool isExist(Char _code) { return _code >= first && _code <= last; }

			// возвращает указатель на глиф, или 0, если код не входит в диапазон
			GlyphInfo * getInfo(Char _code) { return isExist(_code) ? &range[_code - first] : nullptr; }

		public:
			Char first;
			Char last;
			VectorGlyphInfo range;
		};

		typedef std::vector<RangeInfo> VectorRangeInfo;

		struct PairCodeCoord
		{
			PairCodeCoord(Char _code, const IntCoord& _coord) :
				code(_code),
				coord(_coord)
			{
			}

			bool operator < (const PairCodeCoord & _value) const { return code < _value.code; }

			Char code;
			IntCoord coord;
		};

		typedef std::vector<PairCodeCoord> VectorPairCodeCoord;

		enum constCodePoints
		{
			FONT_CODE_SELECT = 0,
			FONT_CODE_TAB = 0x0009,
			FONT_CODE_LF = 0x000A,
			FONT_CODE_CR = 0x000D,
			FONT_CODE_SPACE = 0x0020,
			FONT_CODE_LATIN_START = 0x0021,
			FONT_CODE_NEL = 0x0085,
			FONT_CODE_LATIN_END = 0x00A6
		};

	protected:

        /// Source of the font (either an image name or a truetype font)
		Ogre::String mSource;
        /// Size of the truetype font, in points
		float mTtfSize;
        /// Resolution (dpi) of truetype font
		uint mTtfResolution;

		int mDistance;
		int mSpaceWidth;
		int mTabWidth;
		int mCursorWidth;
		int mOffsetHeight;
		int mDefaultHeight;
		int mHeightPix;

		// отдельная информация о символах
		GlyphInfo mSpaceGlyphInfo, mTabGlyphInfo, mSelectGlyphInfo, mSelectDeactiveGlyphInfo, mCursorGlyphInfo;

		// символы которые не нужно рисовать
		VectorPairCodePoint mVectorHideCodePoint;

		// символы созданные руками
		VectorPairCodeCoord mVectorPairCodeCoord;

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

		void addGlyph(GlyphInfo * _info, Char _index, int _left, int _top, int _right, int _bottom, int _finalw, int _finalh, float _aspect, int _addHeight = 0);

		void loadResourceTrueType(Ogre::Resource* res);
		void addRange(VectorPairCodeCoord & _info, size_t _first, size_t _last, int _width, int _height, float _aspect);
		void checkTexture();

	public:

        /** Constructor.
		@see Resource */
		Font(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0);
        virtual ~Font();

		GlyphInfo * getSpaceGlyphInfo() { return & mSpaceGlyphInfo; }
		GlyphInfo * getTabGlyphInfo() { return & mTabGlyphInfo; }
		GlyphInfo * getSelectGlyphInfo() { return & mSelectGlyphInfo; }
		GlyphInfo * getSelectDeactiveGlyphInfo() { return & mSelectDeactiveGlyphInfo; }
		GlyphInfo * getCursorGlyphInfo() { return & mCursorGlyphInfo; }

		void setSource(const Ogre::String& source) { mSource = source; }
		const Ogre::String& getSource() const { return mSource; }

		void setTrueTypeSize(Ogre::Real ttfSize) { mTtfSize = ttfSize; }
		Ogre::Real getTrueTypeSize() const { return mTtfSize; }

		void setTrueTypeResolution(Ogre::uint ttfResolution) { mTtfResolution = ttfResolution; }
		Ogre::uint getTrueTypeResolution() const { return mTtfResolution; }

		GlyphInfo * getGlyphInfo(Char _id);

		void addCodePointRange(Ogre::Real _first, Ogre::Real _second)
		{
			mVectorRangeInfo.push_back(RangeInfo((Ogre::uint32)_first, (Ogre::uint32)_second));
		}

		void addHideCodePointRange(Ogre::Real _first, Ogre::Real _second)
		{
			mVectorHideCodePoint.push_back(PairCodePoint((unsigned int)_first, (unsigned int)_second));
		}

		// проверяет, входит ли символ в зоны ненужных символов
		bool checkHidePointCode(Char _id)
		{
			for (VectorPairCodePoint::iterator iter=mVectorHideCodePoint.begin(); iter!=mVectorHideCodePoint.end(); ++iter) {
				if (iter->isExist(_id)) return true;
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

		int getDefaultHeight() const { return mDefaultHeight; }
		void setDefaultHeight(int _height) { mDefaultHeight = _height; }

		unsigned int getHeightPix() { return uint(mHeightPix); }

		/** Implementation of ManualResourceLoader::loadResource, called
			when the Texture that this font creates needs to (re)load. */
		void loadResource(Ogre::Resource* resource);

		//ширина пробела в пикселях
		Char setSpaceWidth() { return mSpaceWidth; }
		void setSpaceWidth(int _pix) { mSpaceWidth = _pix; }

		// ширина таба в пикселях
		int getTabWidth() { return mTabWidth; }
		void setTabWidth(int _pix) { mTabWidth = _pix; }

		// ширина курсора в пикселях
		int getCursorWidth() { return mCursorWidth; }
		void setCursorWidth(int _pix) { mCursorWidth = _pix; }

		// расстояние между символами при генерации в пикселях
		int getDistance() { return mDistance; }
		void setDistance(int _pix) { mDistance = _pix; }

		// смещение всех символов по горизонтали
		int getOffsetHeight() { return mOffsetHeight; }
		void setOffsetHeight(int _pix) { mOffsetHeight = _pix; }

		void addGlyph(uint _index, const IntCoord& _coord);

		bool isTrueType() { return mTtfResolution != 0; }

		void initialise();

    };


	/** Specialisation of SharedPtr to allow SharedPtr to be assigned to FontPtr
	@note Has to be a subclass since we need operator=.
	We could templatise this instead of repeating per Resource subclass,
	except to do so requires a form VC6 does not support i.e.
	ResourceSubclassPtr<T> : public SharedPtr<T>
	*/

	class MYGUI_EXPORT FontPtr : public Ogre::SharedPtr<Font>
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
				// RHS must be a nullptr pointer
				MYGUI_ASSERT(r.isNull(), "RHS must be nullptr if it has no mutex!");
				setNull();
			}
			return *this;
		}
	}; // class MYGUI_EXPORT FontPtr : public Ogre::SharedPtr<Font>

} // namespace MyGUI

#endif // __MYGUI_FONT_H__
