/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourceImageSetPointer.h"
#include "MyGUI_ImageBox.h"
#include "MyGUI_ResourceManager.h"

namespace MyGUI
{

	ResourceImageSetPointer::ResourceImageSetPointer() :
		mImageSet(nullptr)
	{
	}

	ResourceImageSetPointer::~ResourceImageSetPointer()
	{
	}

	void ResourceImageSetPointer::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);

		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator info = _node->getElementEnumerator();
		while (info.next("Property"))
		{
			const std::string& key = info->findAttribute("key");
			const std::string& value = info->findAttribute("value");

			if (key == "Point")
				mPoint = IntPoint::parse(value);
			else if (key == "Size")
				mSize = IntSize::parse(value);
			else if (key == "Resource")
				mImageSet = ResourceManager::getInstance().getByName(value)->castType<ResourceImageSet>();
		}
	}

	void ResourceImageSetPointer::setImage(ImageBox* _image)
	{
		if (mImageSet != nullptr)
			_image->setItemResourceInfo(mImageSet->getIndexInfo(0, 0));
	}

	void ResourceImageSetPointer::setPosition(ImageBox* _image, const IntPoint& _point)
	{
		_image->setCoord(_point.left - mPoint.left, _point.top - mPoint.top, mSize.width, mSize.height);
	}

} // namespace MyGUI
