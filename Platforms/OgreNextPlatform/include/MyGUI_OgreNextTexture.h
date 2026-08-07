#ifndef MYGUI_OGRE_NEXT_TEXTURE_H_
#define MYGUI_OGRE_NEXT_TEXTURE_H_

#include "MyGUI_OgreNextPrerequisites.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"

#include <OgreMaterial.h>
#include <OgrePixelFormatGpu.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	class OgreNextTexture : public ITexture
	{
	public:
		OgreNextTexture(const std::string& _name, const std::string& _group);
		~OgreNextTexture() override;

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

		// internal
		Ogre::TextureGpu* getOgreTexture() const
		{
			return mTexture;
		}
		void setOgreTexture(Ogre::TextureGpu* value);

		Ogre::MaterialPtr getMaterial() const
		{
			return mMaterial;
		}

		static Ogre::PixelFormatGpu convertFormat(PixelFormat format);

	private:
		void ensureMaterial();
		void releaseMaterial();
		void setFormatFromOgreTexture();

		std::string mName;
		std::string mGroup;
		std::string mShaderName;

		Ogre::TextureGpu* mTexture{nullptr};
		bool mOwnsTexture{false};

		Ogre::MaterialPtr mMaterial;

		TextureUsage mOriginalUsage;
		PixelFormat mOriginalFormat;
		Ogre::PixelFormatGpu mPixelFormat{Ogre::PFG_UNKNOWN};
		size_t mNumElemBytes{0};

		void* mLockedBuffer{nullptr};
		int mLockedWidth{0};
		int mLockedHeight{0};
		bool mLocked{false};
		bool mLockedRead{false};

		ITextureInvalidateListener* mListener{nullptr};
		IRenderTarget* mRenderTarget{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_TEXTURE_H_
