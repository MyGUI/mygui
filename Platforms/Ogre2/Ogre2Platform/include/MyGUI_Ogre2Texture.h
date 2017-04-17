/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/

#ifndef MYGUI_OGRE2_TEXTURE_H_
#define MYGUI_OGRE2_TEXTURE_H_

#include "MyGUI_Diagnostic.h"
#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_Ogre2Diagnostic.h"

#include <Ogre.h>
#include <OgreResource.h>
#include <OgreTexture.h>
#include <OgrePixelBox.h>
#include <OgreSharedPtr.h>

#include <OgreHlms.h>
#include <OgreHlmsDatablock.h>
#include <OgreHlmsSamplerblock.h>
#include <OgreHlmsManager.h>
#include <Hlms/Unlit/OgreHlmsUnlitDatablock.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	class OgreHlmsBlocks 
	{

	public:
		OgreHlmsBlocks() 
		{
			mMacroBlock.mCullMode = Ogre::CULL_NONE;
			mMacroBlock.mPolygonMode = Ogre::PM_SOLID;
			mMacroBlock.mDepthBiasConstant = 0;
			mMacroBlock.mDepthBiasSlopeScale = 0;
			mMacroBlock.mDepthCheck = false;
			mMacroBlock.mDepthWrite = false;

			mBlendBlock.mAlphaToCoverageEnabled = false;
			mBlendBlock.mSourceBlendFactor = Ogre::SBF_SOURCE_ALPHA;
			mBlendBlock.mDestBlendFactor = Ogre::SBF_ONE_MINUS_SOURCE_ALPHA;

			mSamplerBlock.mU = Ogre::TAM_CLAMP;
			mSamplerBlock.mV = Ogre::TAM_CLAMP;
			mSamplerBlock.mW = Ogre::TAM_CLAMP;
		}

		Ogre::HlmsUnlitDatablock* createUnlitDataBlock( Ogre::String id ) const 
		{
			Ogre::Hlms* hlms = Ogre::Root::getSingleton().getHlmsManager()->getHlms( Ogre::HLMS_UNLIT );

			MYGUI_PLATFORM_ASSERT(hlms != 0, "Ogre::HLMS_UNLIT model was not properly setup.");

			return static_cast<Ogre::HlmsUnlitDatablock*>(hlms->createDatablock(Ogre::IdString( id ), id, mMacroBlock, mBlendBlock, mParamsVec));
		}

	private:
		Ogre::HlmsMacroblock mMacroBlock;
		Ogre::HlmsBlendblock mBlendBlock;
		Ogre::HlmsSamplerblock  mSamplerBlock;
		Ogre::HlmsParamVec mParamsVec;
	};



	class Ogre2Texture :
		public ITexture,
		public Ogre::ManualResourceLoader
	{
	public:
		Ogre2Texture(const std::string& _name, const std::string& _group);
		virtual ~Ogre2Texture();

		virtual const std::string& getName() const;

		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format);
		virtual void loadFromFile(const std::string& _filename);
		virtual void saveToFile(const std::string& _filename);

		virtual void setInvalidateListener(ITextureInvalidateListener* _listener);

		virtual void destroy();

		virtual void* lock(TextureUsage _access);
		virtual void unlock();
		virtual bool isLocked();

		virtual int getWidth();
		virtual int getHeight();

		virtual PixelFormat getFormat()
		{
			return mOriginalFormat;
		}
		virtual TextureUsage getUsage()
		{
			return mOriginalUsage;
		}
		virtual size_t getNumElemBytes()
		{
			return mNumElemBytes;
		}

		virtual IRenderTarget* getRenderTarget();

		static Ogre::TextureUsage convertUsage(TextureUsage _usage);
		static Ogre::PixelFormat convertFormat(PixelFormat _format);
	/*internal:*/
		Ogre::TexturePtr getOgreTexture()
		{
			return mTexture;
		}
		void setOgreTexture(Ogre::TexturePtr _value)
		{
			mTexture = _value;
			setFormatByOgreTexture(); 
			setDataBlockTexture(mTexture); //must be set as well. !!! or youll get a coloured blank myGUI texture.
		}

		Ogre::HlmsDatablock* getDataBlock();

	private:
		void setUsage(TextureUsage _usage);
		void setFormat(PixelFormat _format);
		void setFormatByOgreTexture();

		virtual void loadResource(Ogre::Resource* resource);

		void setDataBlockTexture(Ogre::TexturePtr _value);

	private:
		Ogre::TexturePtr mTexture;
		std::string mName;
		std::string mGroup;

		TextureUsage mOriginalUsage;
		PixelFormat mOriginalFormat;
		size_t mNumElemBytes;

		Ogre::PixelFormat mPixelFormat;
		Ogre::TextureUsage mUsage;

		ITextureInvalidateListener* mListener;
		IRenderTarget* mRenderTarget;
		Ogre::PixelBox mTmpData;

		Ogre::HlmsUnlitDatablock* mDataBlock;

	private:
		static const OgreHlmsBlocks HLMS_BLOCKS;
		static const Ogre::uint8 TEXTURE_UNIT_NUMBER;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_TEXTURE_H_
