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

	void ResourceImageSetPointer::deserialize(pugi::xml_node _node, Version _version)
	{
		Base::deserialize(_node, _version);

		// iterate children, main loop
		for (auto info : _node.children("Property"))
		{
			std::string_view key = info.attribute("key").value();
			std::string_view value = info.attribute("value").value();

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
