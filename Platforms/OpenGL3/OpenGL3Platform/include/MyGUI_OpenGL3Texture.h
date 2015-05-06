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
		virtual ~OpenGL3Texture();

		virtual const std::string& getName() const;

		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format);
		virtual void loadFromFile(const std::string& _filename);
		virtual void saveToFile(const std::string& _filename);

		virtual void destroy();

		virtual int getWidth();
		virtual int getHeight();

		virtual void* lock(TextureUsage _access);
		virtual void unlock();
		virtual bool isLocked();

		virtual PixelFormat getFormat();
		virtual TextureUsage getUsage();
		virtual size_t getNumElemBytes();

		virtual IRenderTarget* getRenderTarget();

	/*internal:*/
		unsigned int getTextureID() const;
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
		unsigned int mTextureID;
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
