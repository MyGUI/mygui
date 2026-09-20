#include "Precompiled.h"
#include "BaseManager.h"
#include "MyGUI_FileSystemUtility.h"

#include <SDL_image.h>
#include <MyGUI_GL.h>

#include <vector>

namespace base
{

	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		// for MacOS, but would not hurt for other platforms
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

		mContext = SDL_GL_CreateContext(mSdlWindow);
		if (mContext == nullptr)
		{
			std::cerr << "Failed to create SDL context: " << SDL_GetError();
			exit(1);
		}
#ifdef _WIN32
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		{
			std::cerr << "Failed to initialize GLAD" << std::endl;
			exit(1);
		}
#endif
#ifndef __EMSCRIPTEN__
		if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
		{
			std::cerr << "Failed to initialize SDL_image: " << IMG_GetError();
			exit(1);
		}
#endif
		return true;
	}

	void BaseManager::destroyRender()
	{
		IMG_Quit();
		SDL_GL_DeleteContext(mContext);
	}

	void BaseManager::createGuiPlatform()
	{
		mPlatform = new MyGUI::OpenGL3Platform();
		setupResources();
		mPlatform->initialise(this);
	}

	void BaseManager::destroyGuiPlatform()
	{
		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}

	void BaseManager::drawOneFrame()
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (mPlatform)
			mPlatform->getRenderManagerPtr()->drawOneFrame();

		if (mScreenShotRequested || mCaptureRequested)
		{
			const bool saveScreenshot = mScreenShotRequested;
			mScreenShotRequested = false;
			int w, h;
			SDL_GL_GetDrawableSize(mSdlWindow, &w, &h);
			std::vector<std::uint8_t> pixels(w * h * 4);
			GLint packAlignment = 0;
			glGetIntegerv(GL_PACK_ALIGNMENT, &packAlignment);
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glReadPixels(0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, pixels.data());
			glPixelStorei(GL_PACK_ALIGNMENT, packAlignment);
			if (mCaptureRequested)
			{
				std::vector<float> depth;
				if (mSceneDepthProbe)
				{
					for (const auto point :
						 {MyGUI::IntPoint(8, 8),
						  MyGUI::IntPoint(w - 9, 8),
						  MyGUI::IntPoint(8, h - 9),
						  MyGUI::IntPoint(w - 9, h - 9),
						  MyGUI::IntPoint(w / 2, h / 2)})
					{
						float sample = 0;
						glReadPixels(point.left, point.top, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &sample);
						depth.push_back(sample);
					}
				}
				const GLenum error = glGetError();
				if (error != GL_NO_ERROR)
					failFrameCapture(
						"OpenGL error during rendering/readback: " + std::to_string(error),
						error == GL_OUT_OF_MEMORY || error == 0x0507 /* GL_CONTEXT_LOST */);
				else
					completeFrameCapture(pixels.data(), w, h, size_t(w) * 4, true, true, depth);
			}
			// Flip vertically (OpenGL origin is bottom-left, images expect top-left)
			const int stride = w * 4;
			for (int y = 0; y < h / 2; ++y)
			{
				auto* top = pixels.data() + y * stride;
				auto* bottom = pixels.data() + (h - 1 - y) * stride;
				std::swap_ranges(top, top + stride, bottom);
			}
			if (saveScreenshot)
				saveImage(w, h, MyGUI::PixelFormat::R8G8B8A8, pixels.data(), mScreenShotFile);
		}

		SDL_GL_SwapWindow(mSdlWindow);
	}

	bool BaseManager::setSceneDepthProbe(bool _enabled)
	{
		int bits = 0;
		if (_enabled)
		{
			if (SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &bits) != 0)
				throw std::runtime_error("Cannot query scene depth buffer");
			if (bits == 0)
				return false;
		}
		mSceneDepthProbe = _enabled;
		glDepthMask(GL_TRUE);
		glClearDepth(_enabled ? 0.25 : 1.0);
		return true;
	}

	bool BaseManager::setHostileRenderState(bool _enabled)
	{
		if (_enabled)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, _enabled ? GL_LINE : GL_FILL);
		return true;
	}

	void BaseManager::resizeRender(int _width, int _height)
	{
		int dw, dh;
		SDL_GL_GetDrawableSize(mSdlWindow, &dw, &dh);
		glViewport(0, 0, dw, dh);
	}

	void BaseManager::addResourceLocation(const std::filesystem::path& _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}

	void* BaseManager::loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename)
	{
		std::string fullname = MyGUI::OpenGL3DataManager::getInstance().getDataPath(_filename);
		void* result = nullptr;
		SDL_Surface* image = nullptr;
		SDL_Surface* cvtImage = nullptr; // converted surface with RGBA/RGB pixel format
		image = IMG_Load(fullname.c_str());
		MYGUI_ASSERT(image != nullptr, "Failed to load image: " + fullname);

		_width = image->w;
		_height = image->h;

		int bpp = image->format->BytesPerPixel;
		if (bpp < 3)
		{
			result = convertPixelData(image, _format);
		}
		else
		{
			Uint32 pixelFmt = bpp == 3 ? SDL_PIXELFORMAT_BGR24 : SDL_PIXELFORMAT_BGRA32;
			cvtImage = SDL_ConvertSurfaceFormat(image, pixelFmt, 0);
			result = convertPixelData(cvtImage, _format);
			SDL_FreeSurface(cvtImage);
		}
		SDL_FreeSurface(image);

		return result;
	}

	void BaseManager::saveImage(
		int _width,
		int _height,
		MyGUI::PixelFormat _format,
		void* _texture,
		const std::filesystem::path& _filename)
	{
		int bpp = _format.getBytesPerPixel();
		Uint32 fmt = (bpp == 3) ? SDL_PIXELFORMAT_BGR24 : SDL_PIXELFORMAT_BGRA32;
		SDL_Surface* surface =
			SDL_CreateRGBSurfaceWithFormatFrom(_texture, _width, _height, bpp * 8, _width * bpp, fmt);
		IMG_SavePNG(surface, MyGUI::utility::toUtf8(_filename).c_str());
		SDL_FreeSurface(surface);
	}

}
