/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_MASK_PEEK_INFO_H__
#define __MYGUI_MASK_PEEK_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include <OgreCodec.h>
#include <OgreImageCodec.h>

namespace MyGUI
{

	class _MyGUIExport MaskPeekInfo
	{
	public:
		MaskPeekInfo() : width(0), height(0) { }

		bool load(const std::string& _file)
		{
			std::string ext;

			size_t pos = _file.find_last_of(".");
			if ( pos == std::string::npos ) {
				MYGUI_LOG(Error, "Unable to load image file '" << _file << "' - invalid extension.");
				return false;
			}

			while ( pos != (_file.length()-1) ) ext += _file[++pos];

			Ogre::Codec * pCodec = Ogre::Codec::getCodec(ext);
			if ( ! pCodec ) {
				MYGUI_LOG(Error, "Unable to load image file '" << _file << "' - can`t find codec for '" << ext << "' extension.");
				return false;
			}

			Ogre::DataStreamPtr encoded = Ogre::ResourceGroupManager::getSingleton().openResource(_file);
			Ogre::Codec::DecodeResult res = pCodec->decode(encoded);
			Ogre::ImageCodec::ImageData* pData = static_cast<Ogre::ImageCodec::ImageData*>(res.second.getPointer());

			unsigned char * buffer = res.first->getPtr();
			unsigned char pixel_size = static_cast<unsigned char>(Ogre::PixelUtil::getNumElemBytes( pData->format ));

			width = (int)pData->width;
			height = (int)pData->height;
			size_t size = pData->width * pData->height;
			data.resize(size);

			pos = 0;
			for (size_t pos_pix=0; pos_pix<size; pos_pix++) {
				char is_null = 0;
				for (size_t in_pix=0; in_pix<pixel_size; in_pix++) {
					if (0xFF != buffer[pos]) is_null = 1;
					pos++;
				}
				data[pos_pix] = is_null;
			}

			encoded.setNull();
			res.first.setNull();
			res.second.setNull();
			return true;
		}

		inline bool peek(const IntPoint& _point, const IntCoord& _coord) const
		{
			if ((0 == _coord.width) || (0 == _coord.height)) return false;

			int x = ((_point.left * width)-1) / _coord.width;
			int y = ((_point.top * height)-1) / _coord.height;

			return 0 != data[y * width + x];
		}

		inline bool empty() const
		{
			return data.empty();
		}

	private:
		std::vector<char> data;
		int width, height;
	};

} // namespace MyGUI

#endif // __MYGUI_MASK_PEEK_INFO_H__
