/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_CommonStateInfo.h"
#include "MyGUI_XmlDocument.h"

#include <pugixml.hpp>

namespace MyGUI
{

	void SubSkinStateInfo::deserialization(xml::ElementPtr _node, Version _version)
	{
		std::string texture{_node.node().parent().parent().attribute("texture").value()};

		// tags replacement support for Skins
		if (_version >= Version(1, 1))
		{
			texture = LanguageManager::getInstance().replaceTags(texture);
		}

		const IntSize& size = texture_utility::getTextureSize(texture);
		IntCoord coord = IntCoord::parse(_node.node().attribute("offset").value());
		mRect = CoordConverter::convertTextureCoord(coord, size);
	}

	void TileRectStateInfo::deserialization(xml::ElementPtr _node, Version _version)
	{
		std::string texture{_node.node().parent().parent().attribute("texture").value()};

		// tags replacement support for Skins
		if (_version >= Version(1, 1))
		{
			texture = LanguageManager::getInstance().replaceTags(texture);
		}

		const IntSize& size = texture_utility::getTextureSize(texture);
		IntCoord coord = IntCoord::parse(_node.node().attribute("offset").value());
		mRect = CoordConverter::convertTextureCoord(coord, size);

		for (auto prop : _node.node().children("Property"))
		{
			std::string_view key = prop.attribute("key").value();
			std::string_view value = prop.attribute("value").value();
			if (key == "TileH")
				mTileH = utility::parseBool(value);
			else if (key == "TileV")
				mTileV = utility::parseBool(value);
			else if (key == "TileSize")
				mTileSize = IntSize::parse(value);
		}
	}

	void RotatingSkinStateInfo::deserialization(xml::ElementPtr _node, Version _version)
	{
		for (auto prop : _node.node().children("Property"))
		{
			std::string_view key = prop.attribute("key").value();
			std::string_view value = prop.attribute("value").value();
			if (key == "Angle")
				mAngle = utility::parseFloat(value);
			if (key == "Center")
				mCenter = IntPoint::parse(value);
		}

		std::string texture{_node.node().parent().parent().attribute("texture").value()};

		// tags replacement support for Skins
		if (_version >= Version(1, 1))
		{
			texture = LanguageManager::getInstance().replaceTags(texture);
		}

		const IntSize& size = texture_utility::getTextureSize(texture);
		IntCoord coord = IntCoord::parse(_node.node().attribute("offset").value());
		mRect = CoordConverter::convertTextureCoord(coord, size);
	}

	void EditTextStateInfo::deserialization(xml::ElementPtr _node, Version _version)
	{
		mShift = utility::parseBool(_node.node().attribute("shift").value());

		std::string colour{_node.node().attribute("colour").value()};
		if (_version >= Version(1, 1))
		{
			colour = LanguageManager::getInstance().replaceTags(colour);
		}

		mColour = Colour::parse(colour);
	}

} // namespace MyGUI
