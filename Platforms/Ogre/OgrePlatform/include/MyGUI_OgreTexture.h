/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/

#ifndef MYGUI_OGRE_TEXTURE_H_
#define MYGUI_OGRE_TEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"

#include <OgreResource.h>
#include <OgreTexture.h>

namespace MyGUI
{

	struct OgreShaderInfo;

	class OgreTexture : public ITexture, public Ogre::ManualResourceLoader
	{
	public:
		OgreTexture(const std::string& _name, const std::string& _group);
		~OgreTexture() override;

		const std::string& getName() const override;

		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format) override;
		void loadFromFile(const std::string& _filename) override;
		void saveToFile(const std::string& _filename) override;
		void setShader(const std::string& _shaderName) override;

		void setInvalidateListener(ITextureInvalidateListener* _listener) override;

		void destroy() override;

		void* lock(TextureUsage _access) override;
		void unlock() override;
		bool isLocked() const override;

		int getWidth() const override;
		int getHeight() const override;

		PixelFormat getFormat() const override
		{
			return mOriginalFormat;
		}
		TextureUsage getUsage() const override
		{
			return mOriginalUsage;
		}
		size_t getNumElemBytes() const override
		{
			return mNumElemBytes;
		}

		IRenderTarget* getRenderTarget() override;

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
		}
		OgreShaderInfo* getShaderInfo() const
		{
			return mShaderInfo;
		}

	private:
		void setUsage(TextureUsage _usage);
		void setFormat(PixelFormat _format);
		void setFormatByOgreTexture();

		void loadResource(Ogre::Resource* resource) override;

	private:
		Ogre::TexturePtr mTexture;
		OgreShaderInfo* mShaderInfo = nullptr;
		std::string mName;
		std::string mGroup;

		TextureUsage mOriginalUsage;
		PixelFormat mOriginalFormat;
		size_t mNumElemBytes{0};

		Ogre::PixelFormat mPixelFormat{Ogre::PF_UNKNOWN};
		Ogre::TextureUsage mUsage{Ogre::TU_DEFAULT};

		ITextureInvalidateListener* mListener{nullptr};
		IRenderTarget* mRenderTarget{nullptr};
		Ogre::PixelBox mTmpData;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_TEXTURE_H_
