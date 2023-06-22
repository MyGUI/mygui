/*!
	@file
	@author		A. Eduardo Garcia Hdez
	@date		03/2022
*/

#ifndef MYGUI_SDL2_TEXTURE_H_
#define MYGUI_SDL2_TEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_SDL2ImageLoader.h"

#include <SDL.h>

namespace MyGUI
{

	class SDL2RenderManager;
	class SDL2RTTexture;

	class SDL2Texture : public ITexture
	{
	public:
		SDL2Texture(const std::string& _name, SDL2ImageLoader* _loader, SDL2RenderManager *_manager, SDL_Renderer *renderer);
		~SDL2Texture() override;

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
		SDL_Texture * getTexture() const { return mTexture; }

		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data);

	private:
		void _create();

	private:
		std::string mName;
		int mWidth;
		int mHeight;
		SDL_PixelFormatEnum mPixelFormat;
		int mInternalPixelFormat;
		int mUsage;
		int mAccess;
		size_t mNumElemBytes;
		size_t mDataSize;
		unsigned int mPboID;
		bool mLock;
		void* mBuffer;
		PixelFormat mOriginalFormat;
		TextureUsage mOriginalUsage;
		SDL2ImageLoader* mImageLoader;
		SDL2RTTexture* mRenderTarget;

		SDL_Renderer *mRenderer;
		SDL_Texture *mTexture;
	};

} // namespace MyGUI

#endif // MYGUI_SDL2_TEXTURE_H_
