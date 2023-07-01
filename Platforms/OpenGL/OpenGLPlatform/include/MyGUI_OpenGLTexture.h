/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef MYGUI_OPENGL_TEXTURE_H_
#define MYGUI_OPENGL_TEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_OpenGLImageLoader.h"

namespace MyGUI
{

	class OpenGLRTTexture;

	class OpenGLTexture : public ITexture
	{
	public:
		OpenGLTexture(const std::string& _name, OpenGLImageLoader* _loader);
		~OpenGLTexture() override;

		const std::string& getName() const override;

		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format) override;
		void loadFromFile(const std::string& _filename) override;
		void saveToFile(const std::string& _filename) override;
		void setShader(const std::string& _shaderName) override;

		void destroy() override;

		int getWidth() const override;
		int getHeight() const override;

		void* lock(TextureUsage _access) override;
		void unlock() override;
		bool isLocked() const override;

		PixelFormat getFormat() const override;
		TextureUsage getUsage() const override;
		size_t getNumElemBytes() const override;

		IRenderTarget* getRenderTarget() override;

		/*internal:*/
		unsigned int getTextureId() const;
		void setUsage(TextureUsage _usage);
		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data);

	private:
		void _create();

	private:
		std::string mName;
		int mWidth{0};
		int mHeight{0};
		int mPixelFormat{0};
		int mInternalPixelFormat{0};
		int mUsage{0};
		int mAccess{0};
		size_t mNumElemBytes{0};
		size_t mDataSize{0};
		unsigned int mTextureId{0};
		unsigned int mPboID{0};
		bool mLock{false};
		void* mBuffer{nullptr};
		PixelFormat mOriginalFormat;
		TextureUsage mOriginalUsage;
		OpenGLImageLoader* mImageLoader;
		OpenGLRTTexture* mRenderTarget{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_OPENGL_TEXTURE_H_
