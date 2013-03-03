/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourceManualPointer.h"
#include "MyGUI_ImageBox.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_TextureUtility.h"

namespace MyGUI
{

	ResourceManualPointer::ResourceManualPointer()
	{
	}

	ResourceManualPointer::~ResourceManualPointer()
	{
	}

	void ResourceManualPointer::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);

		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator info = _node->getElementEnumerator();
		while (info.next("Property"))
		{
			const std::string& key = info->findAttribute("key");
			const std::string& value = info->findAttribute("value");

			if (key == "Point") mPoint = IntPoint::parse(value);
			else if (key == "Size") mSize = IntSize::parse(value);
			else if (key == "Texture") mTexture = value;
			else if (key == "Coord") mTextureCoord = IntCoord::parse(value);
		}
	}

	void ResourceManualPointer::setImage(ImageBox* _image)
	{
		_image->deleteAllItems();
		_image->setImageInfo(mTexture, mTextureCoord, mTextureCoord.size());
	}

	void ResourceManualPointer::setPosition(ImageBox* _image, const IntPoint& _point)
	{
		_image->setCoord(_point.left - mPoint.left, _point.top - mPoint.top, mSize.width, mSize.height);
	}

} // namespace MyGUI
