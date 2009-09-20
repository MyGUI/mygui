/*!
	@file
	@author		George Evmenov
	@date		07/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_OPENGL_TEXTURE_H__
#define __MYGUI_OPENGL_TEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{

	class OpenGLTexture : public ITexture
	{
	public:
		OpenGLTexture(const std::string& _name);
		virtual ~OpenGLTexture();

		virtual const std::string& getName();

		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format);
		virtual void loadFromFile(const std::string& _filename);
		virtual void saveToFile(const std::string& _filename);

		virtual void destroy();

		virtual int getWidth() { return mWidth; }
		virtual int getHeight() { return mHeight; }

		virtual void* lock(TextureUsage _access);
		virtual void unlock();
		virtual bool isLocked() { return mLock; }

		virtual PixelFormat getFormat()  { return mOriginalFormat; }
		virtual TextureUsage getUsage() { return mOriginalUsage; }
		virtual size_t getNumElemBytes() { return mNumElemBytes; }

	/*internal:*/
		unsigned int getTextureID() { return mTextureID; }
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
	};

} // namespace MyGUI

#endif // __MYGUI_OPENGL_TEXTURE_H__
