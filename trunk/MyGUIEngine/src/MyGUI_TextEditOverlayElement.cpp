/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_Prerequest.h"
#include "MyGUI_TextEditOverlayElement.h"
#include "MyGUI_Gui.h"

#include <OgreRoot.h>
#include <OgreOverlayManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwareVertexBuffer.h>

namespace MyGUI
{

    Ogre::String TextEditOverlayElement::msTypeName = "TextEdit";

	TextEditOverlayElement::TextEditOverlayElement(const Ogre::String& name) :
		OverlayElement(name),
		mAlign(ALIGN_CENTER),
		mDefaultColour(0xFFFFFFFF), mInverseColour(0xFF000000),
		mStartSelect(0), mEndSelect(0),
		mRawDataOutOfDate(false),
		mSpaceGlyphInfo(null),
		mTabGlyphInfo(null),
		mBackgroundNormal(true),
		mTransparent(false),
		mAllocSize(0),
        mCharHeight(0.02), mPixelCharHeight(12),
		mViewportAspectCoef(-1),
		mOldViewportAspectCoef(1.0f)
    {
		mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());
		setMetricsMode(Ogre::GMM_PIXELS);
	}

	TextEditOverlayElement::~TextEditOverlayElement()
	{
		delete mRenderOp.vertexData;
	}

    void TextEditOverlayElement::initialise(void)
    {
		if (!mInitialised)
		{
			// Set up the render op
			// Combine positions and texture coords since they tend to change together
			// since character sizes are different
			mRenderOp.vertexData = new Ogre::VertexData();
			Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
			size_t offset = 0;
			// Positions
			decl->addElement(OVERLAY_POSITION_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
			// Texcoords
			decl->addElement(OVERLAY_POSITION_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

			decl->addElement(OVERLAY_POSITION_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 1);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

			decl->addElement(OVERLAY_POSITION_BINDING, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);

			mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
			mRenderOp.useIndexes = false;
			mRenderOp.vertexData->vertexStart = 0;
			// Vertex buffer will be created in checkMemoryAllocation

			checkMemoryAllocation( OVERLAY_DEFAULT_INITIAL_CHARS );

			mInitialised = true;
		}

    }

    void TextEditOverlayElement::checkMemoryAllocation( size_t numChars )
    {
		if( mAllocSize < numChars)
		{
			// ����������� ��� �� �������
			numChars += OVERLAY_DEFAULT_INITIAL_CHARS;

			// Create and bind new buffers
			// Note that old buffers will be deleted automatically through reference counting

			// 6 verts per char since we're doing tri lists without indexes
			// Allocate space for positions & texture coords
			Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
			Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

			mRenderOp.vertexData->vertexCount = numChars * 6;

			// Create dynamic since text tends to change alot
			// positions & texcoords
			Ogre::HardwareVertexBufferSharedPtr vbuf =
				Ogre::HardwareBufferManager::getSingleton().
					createVertexBuffer(
						decl->getVertexSize(OVERLAY_POSITION_BINDING),
						mRenderOp.vertexData->vertexCount,
						Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
			bind->setBinding(OVERLAY_POSITION_BINDING, vbuf);

			mAllocSize = numChars;
		}


    }

    void TextEditOverlayElement::updatePositionGeometry()
    {

		// ��� ������, �� ���������
		if (mpFont.isNull()) return;

		// ���� ���� ��������, ��� ���������� ��������� ������
		updateRawData();

		// ������� ������������� �������
		size_t cursor = 0;

		// ������� �����
		Ogre::RGBA colour_current, colour = mDefaultColour;
		Ogre::RGBA colour_inverse = mInverseColour;

		FloatPoint background(mBackgroundFill);
		if (false == mBackgroundNormal) background = mBackgroundFillDeactive;

		checkMemoryAllocation( mCaption.size() );
		mRenderOp.vertexData->vertexCount = 0;

		// Get position / texcoord buffer
		Ogre::HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(OVERLAY_POSITION_BINDING);
		float *pVert = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		// ��� ���������� ���������, ������� ����������� =)
		float realCharHeight = mCharHeight * 2.0;
		float realWidth = _getWidth() * 2.0;
		float realHeight = _getHeight() * 2.0;
		float realLeft = _getDerivedLeft() * 2.0;
		float realTop = _getDerivedTop() * 2.0;

		// ������� �������� ������
		float right, left = realLeft - 1.0;
		float bottom, top = 1.0 - realTop;

		// ���� ������� ������
		float left_margin = (mPixelScaleX * (float)mMargin.left * 2.0) + left;
		float top_margin = top - (mPixelScaleY * (float)mMargin.top * 2.0);
		float right_margin = (left + realWidth) - (mPixelScaleX * (float)mMargin.right * 2.0);
		float bottom_margin = (top - realHeight) + (mPixelScaleY * (float)mMargin.bottom * 2.0);

		// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
		float left_shift = 0;
		if (mContextSize.width <= realWidth) {
			if ( IS_ALIGN_RIGHT(mAlign) ) left_shift = mContextSize.width - realWidth; // ������������ �� ������ �������
			else if ( IS_ALIGN_HCENTER(mAlign) ) left_shift = (mContextSize.width - realWidth) * 0.5; // ������������ �� ������
		}
		else left_shift = mPixelScaleX * (float)mPointShift.left * 2.0;
		right = left;

		// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
		if (mContextSize.height <= realHeight) {
			if ( IS_ALIGN_BOTTOM(mAlign) ) top += (mContextSize.height - realHeight);
			else if ( IS_ALIGN_VCENTER(mAlign) ) top += (mContextSize.height - realHeight) * 0.5;
		}
		else top += mPixelScaleY * (float)mPointShift.top * 2.0;
		bottom = top;

		// ������ ��������������� ��� ������
		float vertex_top, vertex_bottom, vertex_left, vertex_right;

		// �������� ����
		VectorLineInfo::iterator end = mLinesInfo.end();
		for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

			// �������� ������� ������
			top = bottom;
			bottom -= realCharHeight;
			// ����������� � ���������
			vertex_top = top;
			vertex_bottom = bottom;

			VectorCharInfo::iterator index = line->begin();
			VectorCharInfo::iterator end_index = line->end();
			// ������ ������ ������ ������
			float len = index->getValueFloat();
			++index;
			// ������ ����������� ��������
			size_t count = index->getValueSizeT();
			++index;

			// ���� �� ������������� ���������� ����������
			bool texture_crop_height = false;

			if (vertex_top > top_margin) {
				// �������� �� ������ �����
				if (vertex_bottom > top_margin) {

					// ���������� ������� ���� ������ �������
					for (;index != end_index; ++index) {
						if ( index->isColour() ) {
							colour = index->getColour() | (colour & 0xFF000000);
							colour_inverse = colour ^ 0x00FFFFFF;
						}
					}

					cursor += count;
					continue;
				}
				// ��������
				vertex_top = top_margin;
				texture_crop_height = true;
			}
			if (vertex_bottom < bottom_margin) {
				// ������ ���� ����
				if (vertex_top < bottom_margin) {
					line = end;
					line --;
					cursor += count;
					continue;
				}
				// ��������
				vertex_bottom = bottom_margin;
				texture_crop_height = true;
			}

			// �������� ������� ������
			right = (realLeft - 1.0) - left_shift; // ������������ �� ����� �������
			if ( IS_ALIGN_RIGHT(mAlign) ) right += (mContextSize.width - len); // ������������ �� ������ �������
			else if ( IS_ALIGN_HCENTER(mAlign) ) right += (mContextSize.width - len) * 0.5; // ������������ �� ������

			// ������� ��������� � ������
			size_t cur = cursor;

			// ���������� ���� ������
			for (;index != end_index; ++index) {

				// ��������� �� ����� �����
				if (index->isColour()) {
					colour = index->getColour() | (colour & 0xFF000000);
					colour_inverse = colour ^ 0x00FFFFFF;
					continue;
				}

				// ������������ ������
				Font::GlyphInfo * info = index->getGlyphInfo();
				Ogre::Real horiz_height = info->aspectRatio * mViewportAspectCoef * realCharHeight;

				// �������� ������� ������
				left = right;
				right += horiz_height;

				// �������� �������� ��� ����
				FloatPoint background_current;

				// ������ �� �������
				if ( (cur >= mEndSelect) || (cur < mStartSelect) ) {

					// ���� ������ ��� ��������� �� ������ ������ ��� ���������
					if ( (info->codePoint == Font::FONT_CODE_SPACE) || (info->codePoint == Font::FONT_CODE_TAB) ) {
						cur ++;
						continue;
					}

					colour_current = colour;
					background_current = mBackgroundEmpty;
				}
				// ������ �������
				else {
					// ��������� �����
					colour_current = colour_inverse;
					background_current = background;
				}

				// ����������� � ���������
				vertex_left = left;
				vertex_right = right;

				// ������� ���������� ����������
				float texture_left = info->uvRect.left;
				float texture_right = info->uvRect.right;
				float texture_top = info->uvRect.top;
				float texture_bottom = info->uvRect.bottom;

				// ���� �� ������������� ���������� ����������
				bool texture_crop_width = false;

				if (vertex_left < left_margin) {
					// �������� �� ������ �����
					if (vertex_right < left_margin) {
						cur ++;
						continue;
					}
					// ��������
					vertex_left = left_margin;
					texture_crop_width = true;
				}
				if (vertex_right > right_margin) {
					// ������ ������ �� ����� �� ����
					if (vertex_left > right_margin) {
						index ++;
						// ��� ���� ����� ���� ����� �� ��������, ����� ������ �� ����� ������
						while (index != end_index) {
							// ��������� �� ����� �����
							if ( index->isColour() ) {
								colour = index->getColour() | (colour & 0xFF000000);
								colour_inverse = colour ^ 0x00FFFFFF;
							}
							index ++;
						};
						index --; // ����� ��� ���������� ��������� �� end
						continue;
					}
					// ��������
					vertex_right = right_margin;
					texture_crop_width = true;
				}

				// �������� �������� �� ���������
				if (texture_crop_height) {
					// ���������� ������ �������� � ���������� �����������
					texture_top += (top - vertex_top) * mTextureHeightOne;
					// �������� ������ �������� � ���������� �����������
					texture_bottom -= (vertex_bottom - bottom) * mTextureHeightOne;
				}

				// �������� �������� �� �����������
				if (texture_crop_width) {
					// ���������� ������ �������� � ���������� �����������
					texture_left += (vertex_left - left) * mTextureWidthOne;
					// �������� ������ �������� � ���������� �����������
					texture_right -= (right - vertex_right) * mTextureWidthOne;
				}

				// each vert is (x, y, z, u, v)
				//-------------------------------------------------------------------------------------
				// First tri
				//
				// Upper left
				*pVert++ = vertex_left;
				*pVert++ = vertex_top;
				*pVert++ = -1.0;
				*pVert++ = texture_left;
				*pVert++ = texture_top;
				*pVert++ = background_current.left;
				*pVert++ = background_current.top;
				*((Ogre::RGBA *)(pVert++)) = colour_current;

				// Bottom left
				*pVert++ = vertex_left;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_left;
				*pVert++ = texture_bottom;
				*pVert++ = background_current.left;
				*pVert++ = background_current.top;
				*((Ogre::RGBA *)(pVert++)) = colour_current;

				// Top right
				*pVert++ = vertex_right;
				*pVert++ = vertex_top;
				*pVert++ = -1.0;
				*pVert++ = texture_right;
				*pVert++ = texture_top;
				*pVert++ = background_current.left;
				*pVert++ = background_current.top;
				*((Ogre::RGBA *)(pVert++)) = colour_current;
				//-------------------------------------------------------------------------------------

				//-------------------------------------------------------------------------------------
				// Second tri
				//
				// Top right (again)
				*pVert++ = vertex_right;
				*pVert++ = vertex_top;
				*pVert++ = -1.0;
				*pVert++ = texture_right;
				*pVert++ = texture_top;
				*pVert++ = background_current.left;
				*pVert++ = background_current.top;
				*((Ogre::RGBA *)(pVert++)) = colour_current;

				// Bottom left (again)
				*pVert++ = vertex_left;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_left;
				*pVert++ = texture_bottom;
				*pVert++ = background_current.left;
				*pVert++ = background_current.top;
				*((Ogre::RGBA *)(pVert++)) = colour_current;

				// Bottom right
				*pVert++ = vertex_right;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_right;
				*pVert++ = texture_bottom;
				*pVert++ = background_current.left;
				*pVert++ = background_current.top;
				*((Ogre::RGBA *)(pVert++)) = colour_current;
				//-------------------------------------------------------------------------------------

				mRenderOp.vertexData->vertexCount += 6;
				cur ++;

			}

			// ��������� ������
			cursor += count;
		}

		// Unlock vertex buffer
		vbuf->unlock();

    }

	void TextEditOverlayElement::updateTextureGeometry()
	{
		// Nothing to do, we combine positions and textures
	}

    void TextEditOverlayElement::setCaption( const Ogre::DisplayString& caption )
    {
        mCaption = caption;
		mGeomPositionsOutOfDate = true;
		mGeomUVsOutOfDate = true;
		mRawDataOutOfDate = true;
    }

    void TextEditOverlayElement::setFontName( const Ogre::String& font )
    {
		mpFont = FontManager::getInstance().getByName( font );
		if (mpFont.isNull()) MYGUI_EXCEPT("Could not find font " << font);
		mpFont->load();
		mpMaterial = mpFont->getMaterialSelectedFont();
		mpMaterial->setDepthCheckEnabled(false);
		mpMaterial->setLightingEnabled(false);

		// ������� ������ � ���������
		mSpaceGlyphInfo = mpFont->getSpaceGlyphInfo();
		mTabGlyphInfo = mpFont->getTabGlyphInfo();

		// ������� ������� ����� �� �������� ��� ��������� ������
		Font::GlyphInfo * info = mSpaceGlyphInfo;
		mBackgroundEmpty.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));
		info = mpFont->getSelectGlyphInfo();
		mBackgroundFill.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));
		info = mpFont->getSelectDeactiveGlyphInfo();
		mBackgroundFillDeactive.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));

		mGeomPositionsOutOfDate = true;
		mGeomUVsOutOfDate = true;
		mRawDataOutOfDate = true;
    }

	const Ogre::String& TextEditOverlayElement::getFontName() const
	{
		return mpFont->getName();
	}

	void TextEditOverlayElement::setCharHeight( Ogre::Real height )
	{
		mPixelCharHeight = static_cast<unsigned>(height);
		mGeomPositionsOutOfDate = true;
		mRawDataOutOfDate = true;
	}

	Ogre::Real TextEditOverlayElement::getCharHeight() const
	{
		return mPixelCharHeight;
	}

	const Ogre::String& TextEditOverlayElement::getTypeName(void) const
	{
		return msTypeName;
	}

	void TextEditOverlayElement::getRenderOperation(Ogre::RenderOperation& op)
	{
		op = mRenderOp;
	}

	void TextEditOverlayElement::setMaterialName(const Ogre::String& matName)
	{
		OverlayElement::setMaterialName(matName);
	}

    void TextEditOverlayElement::addBaseParameters(void)
    {
    }

    void TextEditOverlayElement::setColour(const Ogre::ColourValue& _colour)
    {
		Ogre::Root::getSingleton().convertColourValue(_colour, &mDefaultColour);

		// ������������� ����
		mInverseColour = mDefaultColour ^ 0x00FFFFFF;

		mGeomPositionsOutOfDate = true;
    }
    //---------------------------------------------------------------------
    const Ogre::ColourValue& TextEditOverlayElement::getColour(void) const
    {
        // Either one
		return Ogre::ColourValue::White;
    }

    void TextEditOverlayElement::setMetricsMode(Ogre::GuiMetricsMode gmm)
    {
		const FloatSize & size = Gui::getInstance().getViewSize();

		mViewportAspectCoef = size.height / size.width;
		mPixelScaleX = 1.0 / size.width;
		mPixelScaleY = 1.0 / size.height;

		if (mMetricsMode == Ogre::GMM_RELATIVE) {
			mPixelLeft = mLeft;
			mPixelTop = mTop;
			mPixelWidth = mWidth;
			mPixelHeight = mHeight;
		}

		mLeft = mPixelLeft * mPixelScaleX;
		mTop = mPixelTop * mPixelScaleY;
		mWidth = mPixelWidth * mPixelScaleX;
		mHeight = mPixelHeight * mPixelScaleY;

		mMetricsMode = gmm;
		mDerivedOutOfDate = true;
		_positionsOutOfDate();

		// Set pixel variables based on viewport multipliers
		mPixelCharHeight = static_cast<unsigned>(mCharHeight * size.height);
    }

    //---------------------------------------------------------------------------------------------
	void TextEditOverlayElement::updateRawData()
	{
		if (mpFont.isNull()) return;
		if ( ! mRawDataOutOfDate && (mOldViewportAspectCoef == mViewportAspectCoef) ) return;

		// ������ ��� ������� ����������� ������
		static const char convert_colour[64] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0};

		// ���������� ������� ����� ������� � ���������� �����������
		float realCharHeight = mCharHeight * 2.0;
		Font::GlyphInfo * info = mpFont->getGlyphInfo('A');
		mTextureHeightOne = (info->uvRect.bottom - info->uvRect.top) / (realCharHeight);
		mTextureWidthOne = (info->uvRect.right - info->uvRect.left) / (info->aspectRatio * mViewportAspectCoef * realCharHeight);

		mLinesInfo.clear();

		// ������� ������ �������
		mLinesInfo.push_back(VectorCharInfo());
		mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������ ������ ������ � �������� �����������
		mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������, ����������� �������� ��������
		float len = 0, width = 0;
		size_t count = 1;

		Ogre::DisplayString::const_iterator end = mCaption.end();
		for (Ogre::DisplayString::const_iterator index=mCaption.begin(); index!=end; ++index) {

			Font::CodePoint character = MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index);

			if (character == Font::FONT_CODE_CR || character == Font::FONT_CODE_NEL || character == Font::FONT_CODE_LF) {
				// ���������� ������ ���������� ������
				mLinesInfo.back()[0] = EnumCharInfo(len);
				mLinesInfo.back()[1] = EnumCharInfo(count);
				if (width < len) width = len;
				count = 1;
				len = 0;

				// � ������� �����
				mLinesInfo.push_back(VectorCharInfo());
				mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������ ������ ������ � ��������
				mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������, ����������� �������� ��������

				if (character == Font::FONT_CODE_CR) {
					Ogre::DisplayString::const_iterator peeki = index;
					peeki++;
					if (peeki != end && MYGUI_DEREF_DISPLAYSTRING_ITERATOR(peeki) == Font::FONT_CODE_LF) index = peeki; // skip both as one newline
				}
				// ��������� ������
				continue;

			} else if (character == _T('#')) {
				// ����� ��������� ������
				++ index;
				if (index == end) {--index ;continue;} // ��� ������

				character = MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index);
				// ���� ��� ������, �� ������ ���� ����, ���� ��� �� ������ ����
				if (character != _T('#')) {

					// ������ ������ ������
					Ogre::RGBA colour = convert_colour[(character-48) & 0x3F];

					// � ��� ���� �������� ����� �����
					for (char i=0; i<5; i++) {
						++ index;
						if (index == end) {--index ;continue;} // ��� ������
						colour <<= 4;
						colour += convert_colour[ (MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index)-48) & 0x3F];
					}

					// ���� �����, �� ������ ������� � ����� ����������
					if (mRenderGL) colour = ((colour&0x00FF0000)>>16)|((colour&0x000000FF)<<16)|(colour&0xFF00FF00);

					// ���������� ����, � ������� ����� �������
					mLinesInfo.back().push_back( EnumCharInfo(colour, true) );

					continue;
				}
			}

			Font::GlyphInfo * info;
			if (Font::FONT_CODE_SPACE == character) info = mpFont->getSpaceGlyphInfo();
			else if (Font::FONT_CODE_TAB == character) info = mpFont->getTabGlyphInfo();
			else info = mpFont->getGlyphInfo(character);

			len += info->aspectRatio * realCharHeight * mViewportAspectCoef;

			// ��������� �� ���� � �������
			mLinesInfo.back().push_back( EnumCharInfo(info) );
			count ++;

		}

		// ���������� ������ ���������� ������
		mLinesInfo.back()[0] = EnumCharInfo(len);
		mLinesInfo.back()[1] = EnumCharInfo(count);
		if (width < len) width = len;


		// ��������� �����
		mRawDataOutOfDate = false;
		mOldViewportAspectCoef = mViewportAspectCoef;

		// ������������� ������ ������
		mContextSize.set(width, (float)mLinesInfo.size() * mCharHeight * 2.0);

	} // void updateRawData()

	size_t TextEditOverlayElement::getTextCursorFromPoint(IntPoint & _point)
	{
		// ���� ���� ��������, ��� ���������� ��������� ������
		updateRawData();

		// ������� �������
		size_t position = 0;

		// ��� ���������� ���������, ������� ����������� =)
		float realCharHeight = mCharHeight * 2.0;
		float realWidth = _getWidth() * 2.0;
		float realHeight = _getHeight() * 2.0;
		float realLeft = _getDerivedLeft() * 2.0;
		float realTop = _getDerivedTop() * 2.0;

		// ���������� ����������
		float x = mPixelScaleX * (float)_point.left * 2.0;
		float y = ( 1.0f - (mPixelScaleY * (float)_point.top * 2.0) );

		// ������� �������� ������
		float right, left = realLeft - 1.0;
		float bottom, top = 1.0 - realTop;

		// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
		float left_shift = 0;
		if (mContextSize.width <= realWidth) {
			if ( IS_ALIGN_RIGHT(mAlign) ) left_shift = mContextSize.width - realWidth; // ������������ �� ������ �������
			else if ( IS_ALIGN_HCENTER(mAlign) ) left_shift = (mContextSize.width - realWidth) * 0.5; // ������������ �� ������
		}
		else left_shift = mPixelScaleX * (float)mPointShift.left * 2.0;
		right = left;

		// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
		if (mContextSize.height <= realHeight) {
			if ( IS_ALIGN_BOTTOM(mAlign) ) top += (mContextSize.height - realHeight);
			else if ( IS_ALIGN_VCENTER(mAlign) ) top += (mContextSize.height - realHeight) * 0.5;
		}
		else top += mPixelScaleY * (float)mPointShift.top * 2.0;
		bottom = top;

		// ������������ ���������� �� ������ ������
		if (y < (bottom - mContextSize.height)) y = bottom - mContextSize.height;

		// �������� ����
		VectorLineInfo::iterator end = mLinesInfo.end();
		for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

			// �������� ������� ������
			top = bottom;
			bottom -= realCharHeight;

			VectorCharInfo::iterator index = line->begin();
			VectorCharInfo::iterator end_index = line->end();

			// ������ ������ ������ ������
			float len = index->getValueFloat();
			++index;

			// ����������� ��������
			size_t count = index->getValueSizeT();
			++index;

			// ��������� �������
			if (y < bottom) {
				position += count;
				continue;
			}

			// �������� ������� ������
			right = (realLeft - 1.0) - left_shift; // ������������ �� ����� �������
			if ( IS_ALIGN_RIGHT(mAlign) ) right += (mContextSize.width - len); // ������������ �� ������ �������
			else if ( IS_ALIGN_HCENTER(mAlign) ) right += (mContextSize.width - len) * 0.5; // ������������ �� ������

			if (x <= (1.0 + right)) {
				// � ������ ������
				_point.left = (int)((1.0f + right) / (mPixelScaleX * 2.0));
				_point.top = (int)((1.0f - top) / (mPixelScaleY * 2.0));
				return position;

			} else if (x >= (1.0 + right + len)) {
				// � ����� ������
				_point.left = (int)((1.0f + right + len) / (mPixelScaleX * 2.0));
				_point.top = (int)((1.0f - top) / (mPixelScaleY * 2.0));
				position += count - 1;
				return position;
			}

			// ���������� ���� ������
			for (;index != end_index; ++index) {

				// ��������� �� ����� �����
				if ( index->isColour() ) continue;

				// ������������ ������
				Font::GlyphInfo * info = index->getGlyphInfo();
				Ogre::Real horiz_height = info->aspectRatio * mViewportAspectCoef * realCharHeight;

				// �������� ������� ������
				left = right;
				right += horiz_height;

				// ������ � ������, ���������� � ��������� ������
				if (x < (1.0 + right)) {
					if (x < ((1.0 + right) - (horiz_height * 0.5)) ) {
						// � ����� �������� �������
						_point.left = (int)((1.0f + left) / (mPixelScaleX * 2.0));

					} else {
						// � ������ �������� �������
						position ++;
						_point.left = (int)((1.0f + right) / (mPixelScaleX * 2.0));
					}
					_point.top = (int)((1.0f - top) / (mPixelScaleY * 2.0));
					return position;
				}

				// ��������� ������
				position ++;

			}
		}

		return position;
	}

	IntPoint TextEditOverlayElement::getTextCursorFromPosition(size_t _position)
	{
		// ���� ���� ��������, ��� ���������� ��������� ������
		updateRawData();

		// ������� �������
		size_t pos = _position;

		// ��� ���������� ���������, ������� ����������� =)
		float realCharHeight = mCharHeight * 2.0;
		float realWidth = _getWidth() * 2.0;
		float realHeight = _getHeight() * 2.0;
		float realLeft = _getDerivedLeft() * 2.0;
		float realTop = _getDerivedTop() * 2.0;

		// ������� �������� ������
		float right, left = realLeft - 1.0;
		float bottom, top = 1.0 - realTop;

		// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
		float left_shift = 0;
		if (mContextSize.width <= realWidth) {
			if ( IS_ALIGN_RIGHT(mAlign) ) left_shift = mContextSize.width - realWidth; // ������������ �� ������ �������
			else if ( IS_ALIGN_HCENTER(mAlign) ) left_shift = (mContextSize.width - realWidth) * 0.5; // ������������ �� ������
		}
		else left_shift = mPixelScaleX * (float)mPointShift.left * 2.0;
		right = left;

		// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
		if (mContextSize.height <= realHeight) {
			if ( IS_ALIGN_BOTTOM(mAlign) ) top += (mContextSize.height - realHeight);
			else if ( IS_ALIGN_VCENTER(mAlign) ) top += (mContextSize.height - realHeight) * 0.5;
		}
		else top += mPixelScaleY * (float)mPointShift.top * 2.0;
		bottom = top;

		// �������� ����
		VectorLineInfo::iterator end = mLinesInfo.end();
		for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

			// �������� ������� ������
			top = bottom;
			bottom -= realCharHeight;

			VectorCharInfo::iterator index = line->begin();
			VectorCharInfo::iterator end_index = line->end();

			// ������ ������ ������ ������
			float len = index->getValueFloat();
			++index;

			// ����������� ��������
			size_t count = index->getValueSizeT();
			++index;

			// ��������� �������
			if (pos >= count) {
				pos -= count;
				continue;
			}

			// �������� ������� ������
			right = (realLeft - 1.0) - left_shift; // ������������ �� ����� �������
			if ( IS_ALIGN_RIGHT(mAlign) ) right += (mContextSize.width - len); // ������������ �� ������ �������
			else if ( IS_ALIGN_HCENTER(mAlign) ) right += (mContextSize.width - len) * 0.5; // ������������ �� ������

			// �����, ����������
			if (pos == 0) return IntPoint((int)((1.0f + right) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));

			unsigned int currect_pos = 0;

			// ���������� ���� ������
			for (;index != end_index; ++index) {

				// ��������� �� ����� �����
				if ( index->isColour() ) continue;

				// ������������ ������
				Font::GlyphInfo * info = index->getGlyphInfo();
				Ogre::Real horiz_height = info->aspectRatio * mViewportAspectCoef * realCharHeight;

				// �������� ������� ������
				left = right;
				right += horiz_height;

				// �����, ����������
				if (pos == currect_pos) return IntPoint((int)((1.0f + left) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));
				else if (pos == (currect_pos+1)) return IntPoint((int)((1.0f + right) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));

				// ��������� ������
				currect_pos ++;

			}

		}

		// � ����� �����
		return IntPoint((int)((1.0f + right) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));
	}

	IntSize TextEditOverlayElement::getTextSize(const Ogre::DisplayString& _text)
	{
		IntSize size;

		if (mpFont.isNull()) return size;

		float len = 0, width = 0;
		int height = 1;

		Ogre::DisplayString::const_iterator end = _text.end();
		for (Ogre::DisplayString::const_iterator index=_text.begin(); index!=end; ++index) {

			Font::CodePoint character = MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index);

			if (character == Font::FONT_CODE_CR || character == Font::FONT_CODE_NEL || character == Font::FONT_CODE_LF) {
				if (width < len) width = len;
				len = 0;
				height ++;

				if (character == Font::FONT_CODE_CR) {
					Ogre::DisplayString::const_iterator peeki = index;
					peeki++;
					if (peeki != end && MYGUI_DEREF_DISPLAYSTRING_ITERATOR(peeki) == Font::FONT_CODE_LF) index = peeki; // skip both as one newline
				}
				// ��������� ������
				continue;

			} else if (character == _T('#')) {
				// ����� ��������� ������
				++ index;
				if (index == end) {--index ;continue;} // ��� ������

				character = MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index);
				// ���� ��� ������, �� ������ ���� ����, ���� ��� �� ������ ����
				if (character != _T('#')) {
					// � ��� ���� �������� ����� �����
					for (char i=0; i<5; i++) {
						++ index;
						if (index == end) {--index ;continue;} // ��� ������
					}
					continue;
				}
			}

			Font::GlyphInfo * info;
			if (Font::FONT_CODE_SPACE == character) info = mpFont->getSpaceGlyphInfo();
			else if (Font::FONT_CODE_TAB == character) info = mpFont->getTabGlyphInfo();
			else info = mpFont->getGlyphInfo(character);

			len += info->aspectRatio * (float)mPixelCharHeight;
		}

		if (width < len) width = len;

		size.set((int)width, height * (int)mPixelCharHeight);
		return size;
	}

    void TextEditOverlayElement::_update(void)
    {
		const FloatSize & size = Gui::getInstance().getViewSize();

		mViewportAspectCoef = size.height / size.width;

		if (Ogre::OverlayManager::getSingleton().hasViewportChanged() || mGeomPositionsOutOfDate) {
			// Recalc character size
			mCharHeight = (Ogre::Real) mPixelCharHeight / size.height;//vpHeight;
			mGeomPositionsOutOfDate = true;
		}


		mPixelScaleX = 1.0 / size.width;
		mPixelScaleY = 1.0 / size.height;

		mLeft = mPixelLeft * mPixelScaleX;
		mTop = mPixelTop * mPixelScaleY;
		mWidth = mPixelWidth * mPixelScaleX;
		mHeight = mPixelHeight * mPixelScaleY;

		_updateFromParent();
		// NB container subclasses will update children too

		// Tell self to update own position geometry
		if (mGeomPositionsOutOfDate && mInitialised) {
			updatePositionGeometry();
			mGeomPositionsOutOfDate = false;
		}
		// Tell self to update own texture geometry
		if (mGeomUVsOutOfDate && mInitialised) {
			updateTextureGeometry();
			mGeomUVsOutOfDate = false;
		}

    }

	void TextEditOverlayElement::_updateFromParent(void)
    {
		const FloatSize & size = Gui::getInstance().getViewSize();

		Ogre::Real parentLeft, parentTop, parentBottom, parentRight;

		if (mParent) {
			parentLeft = mParent->_getDerivedLeft();
			parentTop = mParent->_getDerivedTop();
		}
		else {
			Ogre::RenderSystem* rSys = Ogre::Root::getSingleton().getRenderSystem();

			// Calculate offsets required for mapping texel origins to pixel origins in the
			// current rendersystem
			Ogre::Real hOffset = rSys->getHorizontalTexelOffset() / size.width;
			Ogre::Real vOffset = rSys->getVerticalTexelOffset() / size.height;

			parentLeft = 0.0f + hOffset;
			parentTop = 0.0f + vOffset;
			parentRight = 1.0f + hOffset;
			parentBottom = 1.0f + vOffset;
		}

		mDerivedLeft = parentLeft + mLeft;

		mDerivedTop = parentTop + mTop;

		mDerivedOutOfDate = false;

		if (mParent != 0) {
			Ogre::Rectangle parent;
			Ogre::Rectangle child;

			mParent->_getClippingRegion(parent);

			child.left   = mDerivedLeft;
			child.top    = mDerivedTop;
			child.right  = mDerivedLeft + mWidth;
			child.bottom = mDerivedTop + mHeight;

			mClippingRegion = intersect(parent, child);
		}
		else {
			mClippingRegion.left   = mDerivedLeft;
			mClippingRegion.top    = mDerivedTop;
			mClippingRegion.right  = mDerivedLeft + mWidth;
			mClippingRegion.bottom = mDerivedTop + mHeight;
		}
    }

    void TextEditOverlayElement::_notifyViewport()
    {
		const FloatSize & size = Gui::getInstance().getViewSize();

		mPixelScaleX = 1.0 / size.width;
		mPixelScaleY = 1.0 / size.height;

		mLeft = mPixelLeft * mPixelScaleX;
		mTop = mPixelTop * mPixelScaleY;
		mWidth = mPixelWidth * mPixelScaleX;
		mHeight = mPixelHeight * mPixelScaleY;

		mGeomPositionsOutOfDate = true;
		mRawDataOutOfDate = true;
		mDerivedOutOfDate = true;

	}

} // namespace MyGUI
