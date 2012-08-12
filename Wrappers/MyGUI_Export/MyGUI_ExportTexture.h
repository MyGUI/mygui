/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef _16ba0d4a_d7f9_41a3_b671_cb0aa8ca2276_
#define _16ba0d4a_d7f9_41a3_b671_cb0aa8ca2276_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{

	class ExportTexture :
		public ITexture
	{
	public:
		ExportTexture(const std::string& _name);
		virtual ~ExportTexture();

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
		void setId(size_t _id);
		size_t getId() const;

		void setWidth(int _value);
		void setHeight(int _value);

	private:
		std::string mName;
		int mWidth;
		int mHeight;
		size_t mNumElemBytes;
		PixelFormat mOriginalFormat;
		TextureUsage mOriginalUsage;
		size_t mId;
	};

}

#endif
