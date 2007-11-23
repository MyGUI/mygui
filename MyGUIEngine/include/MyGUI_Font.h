#ifndef __MYGUI_FONT_H__
#define __MYGUI_FONT_H__

#include "Prerequest.h"
#include "OgreResource.h"
#include "OgreTexture.h"
#include "OgreMaterial.h"
#include "OgreCommon.h"

namespace MyGUI
{

    /** Class representing a font in the system.
    @remarks
    This class is simply a way of getting a font texture into the OGRE system and
    to easily retrieve the texture coordinates required to accurately render them.
    Fonts can either be loaded from precreated textures, or the texture can be generated
    using a truetype font. You can either create the texture manually in code, or you
    can use a .fontdef script to define it (probably more practical since you can reuse
    the definition more easily)
	@note
	This class extends both Resource and ManualResourceLoader since it is
	both a resource in it's own right, but it also provides the manual load
	implementation for the Texture it creates.
    */
	class _MyGUIExport Font : public Ogre::Resource, public Ogre::ManualResourceLoader
    {

	public:
		typedef Ogre::uint32 CodePoint;
		typedef Ogre::FloatRect UVRect;
		/// Information about the position and size of a glyph in a texture
		struct GlyphInfo 
		{
			CodePoint codePoint;
			UVRect uvRect;
			Ogre::Real aspectRatio;

			GlyphInfo(CodePoint id, const UVRect& rect, Ogre::Real aspect)
				: codePoint(id), uvRect(rect), aspectRatio(aspect) { }
		};
		/// A range of code points, inclusive on both ends
		typedef std::pair<CodePoint, CodePoint> CodePointRange;
		typedef std::vector<CodePointRange> CodePointRangeList;

		// constants
		enum constCodePoints
		{
			FONT_CODE_SELECT = 0,
			FONT_CODE_SPACE = 0x0020,
			FONT_CODE_TAB = 0x0009,
			FONT_CODE_NEL = 0x0085,
			FONT_CODE_CR = 0x000D,
			FONT_CODE_LF = 0x000A
		};

	protected:

        /// Source of the font (either an image name or a truetype font)
		Ogre::String mSource;
        /// Size of the truetype font, in points
		Ogre::Real mTtfSize;
        /// Resolution (dpi) of truetype font
		Ogre::uint mTtfResolution;

		CodePoint mSpaceSimbol;
		Ogre::uint8 mCountSpaceTab;

	protected:
		/// Map from unicode code point to texture coordinates
		typedef std::map<CodePoint, GlyphInfo> CodePointMap;
		CodePointMap mCodePointMap;

        /// The material which is generated for this font
		Ogre::MaterialPtr mpMaterial;

        /// The material which is generated for this font
		Ogre::MaterialPtr mpMaterialSelectedFont;

		/// Texture pointer
		Ogre::TexturePtr mTexture;

        /// for TRUE_TYPE font only
        bool mAntialiasColour;

		/// Range of code points to generate glyphs for (truetype only)
		CodePointRangeList mCodePointRangeList;

		/// @copydoc Resource::loadImpl
		virtual void loadImpl();
		/// @copydoc Resource::unloadImpl
		virtual void unloadImpl();
		/// @copydoc Resource::calculateSize
		size_t calculateSize(void) const { return 0; } // permanent resource is in the texture 

	public:

        /** Constructor.
		@see Resource
        */

		Font(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0);
        virtual ~Font();

        /** Sets the source of the font.
        @remarks
            If you have created a font of type FT_IMAGE, this method tells the
            Font which image to use as the source for the characters. So the parameter 
            should be the name of an appropriate image file. Note that when using an image
            as a font source, you will also need to tell the font where each character is
            located using setGlyphTexCoords (for each character).
        @par
            If you have created a font of type FT_TRUETYPE, this method tells the
            Font which .ttf file to use to generate the text. You will also need to call 
            setTrueTypeSize and setTrueTypeResolution, and call addCodePointRange
			as many times as required to define the range of glyphs you want to be
			available.
        @param source An image file or a truetype font, depending on the type of this font
        */
		void setSource(const Ogre::String& source);

        /** Gets the source this font (either an image or a truetype font).
        */
		const Ogre::String& getSource(void) const;

        /** Sets the size of a truetype font (only required for FT_TRUETYPE). 
        @param ttfSize The size of the font in points. Note that the
            size of the font does not affect how big it is on the screen, just how large it is in
            the texture and thus how detailed it is.
        */
		void setTrueTypeSize(Ogre::Real ttfSize);
        /** Gets the resolution (dpi) of the font used to generate the texture
        (only required for FT_TRUETYPE).
        @param ttfResolution The resolution in dpi
        */
        void setTrueTypeResolution(Ogre::uint ttfResolution);

        /** Gets the point size of the font used to generate the texture.
        @remarks
            Only applicable for FT_TRUETYPE Font objects.
            Note that the size of the font does not affect how big it is on the screen, 
            just how large it is in the texture and thus how detailed it is.            
        */
        Ogre::Real getTrueTypeSize(void) const;
        /** Gets the resolution (dpi) of the font used to generate the texture.
        @remarks
            Only applicable for FT_TRUETYPE Font objects.
        */
        Ogre::uint getTrueTypeResolution(void) const;


        /** Returns the teture coordinates of the associated glyph. 
            @remarks Parameter is a short to allow both ASCII and wide chars.
            @param id The code point (unicode)
            @returns A rectangle with the UV coordinates, or null UVs if the
				code point was not present
        */
        inline const UVRect& getGlyphTexCoords(CodePoint id) const
        {
			CodePointMap::const_iterator i = mCodePointMap.find(id);
			if (i != mCodePointMap.end())
			{
				return i->second.uvRect;
			}
			else
			{
				static UVRect nullRect(0.0, 0.0, 0.0, 0.0);
				return nullRect;
			}
        }

        /** Sets the texture coordinates of a glyph.
        @remarks
            You only need to call this if you're setting up a font loaded from a texture manually.
        @note
            Also sets the aspect ratio (width / height) of this character. textureAspect
			is the width/height of the texture (may be non-square)
        */
        inline void setGlyphTexCoords(CodePoint id, Ogre::Real u1, Ogre::Real v1, Ogre::Real u2, Ogre::Real v2, Ogre::Real textureAspect)
        {
			CodePointMap::iterator i = mCodePointMap.find(id);
			if (i != mCodePointMap.end())
			{
				i->second.uvRect.left = u1;
				i->second.uvRect.top = v1;
				i->second.uvRect.right = u2;
				i->second.uvRect.bottom = v2;
				i->second.aspectRatio = textureAspect * (u2 - u1)  / (v2 - v1);
			}
			else
			{
				mCodePointMap.insert(
					CodePointMap::value_type(id, 
						GlyphInfo(id, UVRect(u1, v1, u2, v2), 
							textureAspect * (u2 - u1)  / (v2 - v1))));
			}

        }
        /** Gets the aspect ratio (width / height) of this character. */
        inline Ogre::Real getGlyphAspectRatio(CodePoint id) const
        {
			CodePointMap::const_iterator i = mCodePointMap.find(id);
			if (i != mCodePointMap.end())
			{
				return i->second.aspectRatio;
			}
			else
			{
				return 1.0;
			}
        }
        /** Sets the aspect ratio (width / height) of this character.
        @remarks
            You only need to call this if you're setting up a font loaded from a 
			texture manually.
        */
        inline void setGlyphAspectRatio(CodePoint id, Ogre::Real ratio)
        {
			CodePointMap::iterator i = mCodePointMap.find(id);
			if (i != mCodePointMap.end())
			{
				i->second.aspectRatio = ratio;
			}
        }

		/** Gets the information available for a glyph corresponding to a
			given code point, or throws an exception if it doesn't exist;
		*/
		const GlyphInfo& getGlyphInfo(CodePoint id) const;

		/** Adds a range of code points to the list of code point ranges to generate
			glyphs for, if this is a truetype based font.
		@remarks
			In order to save texture space, only the glyphs which are actually
			needed by the application are generated into the texture. Before this
			object is loaded you must call this method as many times as necessary
			to define the code point range that you need.
		*/
		void addCodePointRange(const CodePointRange& range)
		{
			mCodePointRangeList.push_back(range);
		}

		/** Clear the list of code point ranges.
		*/
		void clearCodePointRanges()
		{
			mCodePointRangeList.clear();
		}
		/** Get a const reference to the list of code point ranges to be used to
			generate glyphs from a truetype font.
		*/
		const CodePointRangeList& getCodePointRangeList() const
		{
			return mCodePointRangeList;
		}
        /** Gets the material generated for this font, as a weak reference. 
        @remarks
            This will only be valid after the Font has been loaded. 
        */
        inline const Ogre::MaterialPtr& getMaterial() const
        {
            return mpMaterial;
        }

        /** Gets the material generated for this font, as a weak reference. 
        @remarks
            This will only be valid after the Font has been loaded. 
        */
        inline const Ogre::MaterialPtr& getMaterial()
        {
            return mpMaterial;
        }

		inline const Ogre::MaterialPtr& getMaterialSelectedFont() const
        {
            return mpMaterialSelectedFont;
        }

		inline const Ogre::MaterialPtr& getMaterialSelectedFont()
        {
            return mpMaterialSelectedFont;
        }

        /** Sets whether or not the colour of this font is antialiased as it is generated
            from a true type font.
        @remarks
        	This is valid only for a FT_TRUETYPE font. If you are planning on using 
            alpha blending to draw your font, then it is a good idea to set this to
            false (which is the default), otherwise the darkening of the font will combine
            with the fading out of the alpha around the edges and make your font look thinner
            than it should. However, if you intend to blend your font using a colour blending
            mode (add or modulate for example) then it's a good idea to set this to true, in
            order to soften your font edges.
        */
        inline void setAntialiasColour(bool enabled)
        {
        	mAntialiasColour = enabled;
        }

		/** Gets whether or not the colour of this font is antialiased as it is generated
		from a true type font.
		*/
        inline bool getAntialiasColour(void) const
        {
            return mAntialiasColour;
        }

		/** Implementation of ManualResourceLoader::loadResource, called
			when the Texture that this font creates needs to (re)load.
		*/
		void loadResource(Ogre::Resource* resource);

		// устанавливает и берет значение ширины из символа для пробела
		inline CodePoint getSpaceSimbol() {return mSpaceSimbol;}
		inline void setSpaceSimbol(CodePoint _simbol) {mSpaceSimbol = _simbol;}

		// устанавливает колличество пробелов для таба
		inline Ogre::uint8 getCountSpaceTab() {return mCountSpaceTab;}
		inline void setCountSpaceTab(Ogre::uint8 _count) {mCountSpaceTab = _count;}

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
				assert(r.isNull() && "RHS must be null if it has no mutex!");
				setNull();
			}
			return *this;
		}
	}; // class _MyGUIExport FontPtr : public Ogre::SharedPtr<Font>

} // namespace MyGUI

#endif // __MYGUI_FONT_H__