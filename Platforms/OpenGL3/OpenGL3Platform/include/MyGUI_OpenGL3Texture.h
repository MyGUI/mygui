/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef MYGUI_OPENGL3_TEXTURE_H_
#define MYGUI_OPENGL3_TEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_OpenGL3ImageLoader.h"

namespace MyGUI
{

	class OpenGL3RTTexture;

	class OpenGL3Texture : public ITexture
	{
	public:
		OpenGL3Texture(const std::string& _name, OpenGL3ImageLoader* _loader);
		~OpenGL3Texture() override;

		const std::string& getName() const override;

		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format) override;
		void loadFromFile(const std::string& _filename) override;
		void saveToFile(const std::string& _filename) override;
		void setShader(const std::string& _shaderName) override;

		void destroy() override;

		int getWidth() override;
		int getHeight() override;

		void* lock(TextureUsage _access) override;
		void unlock() override;
		bool isLocked() override;

		PixelFormat getFormat() override;
		TextureUsage getUsage() override;
		size_t getNumElemBytes() override;

		IRenderTarget* getRenderTarget() override;

	/*internal:*/
		unsigned int getTextureId() const;
		unsigned int getShaderId() const;
		void setUsage(TextureUsage _usage);
		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data);

	private:
		void _create();

	private:
		std::string mName;
		int mWidth;
		int mHeight;
		int mPixelFormat;
		int mInternalPixelFormat;
		int mUsage;
		int mAccess;
		size_t mNumElemBytes;
		size_t mDataSize;
		unsigned int mTextureId;
		unsigned int mProgramId;
		unsigned int mPboID;
		bool mLock;
		void* mBuffer;
		PixelFormat mOriginalFormat;
		TextureUsage mOriginalUsage;
		OpenGL3ImageLoader* mImageLoader;
		OpenGL3RTTexture* mRenderTarget;
	};

} // namespace MyGUI

#endif // MYGUI_OPENGL3_TEXTURE_H_
