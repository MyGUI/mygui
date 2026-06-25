/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_COMMON_STATE_INFO_H_
#define MYGUI_COMMON_STATE_INFO_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IStateInfo.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_TextureUtility.h"

namespace MyGUI
{

	class MYGUI_EXPORT SubSkinStateInfo : public IStateInfo
	{
		MYGUI_RTTI_DERIVED(SubSkinStateInfo)

	public:
		const FloatRect& getRect() const
		{
			return mRect;
		}

	private:
		void deserialization(pugi::xml_node _node, Version _version) override
		{
			std::string texture{_node.parent().parent().attribute("texture").value()};

			// tags replacement support for Skins
			if (_version >= Version(1, 1))
			{
				texture = LanguageManager::getInstance().replaceTags(texture);
			}

			const IntSize& size = texture_utility::getTextureSize(texture);
			IntCoord coord = IntCoord::parse(_node.attribute("offset").value());
			mRect = CoordConverter::convertTextureCoord(coord, size);
		}

	private:
		FloatRect mRect;
	};

	class MYGUI_EXPORT TileRectStateInfo : public IStateInfo
	{
		MYGUI_RTTI_DERIVED(TileRectStateInfo)

	public:
		const FloatRect& getRect() const
		{
			return mRect;
		}

		const IntSize& getTileSize() const
		{
			return mTileSize;
		}

		bool getTileH() const
		{
			return mTileH;
		}

		bool getTileV() const
		{
			return mTileV;
		}

	private:
		void deserialization(pugi::xml_node _node, Version _version) override
		{
			std::string texture{_node.parent().parent().attribute("texture").value()};

			// tags replacement support for Skins
			if (_version >= Version(1, 1))
			{
				texture = LanguageManager::getInstance().replaceTags(texture);
			}

			const IntSize& size = texture_utility::getTextureSize(texture);
			IntCoord coord = IntCoord::parse(_node.attribute("offset").value());
			mRect = CoordConverter::convertTextureCoord(coord, size);

			for (auto prop : _node.children("Property"))
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

	private:
		FloatRect mRect;
		IntSize mTileSize;
		bool mTileH{true};
		bool mTileV{true};
	};

	class MYGUI_EXPORT RotatingSkinStateInfo : public IStateInfo
	{
		MYGUI_RTTI_DERIVED(RotatingSkinStateInfo)

	public:
		float getAngle() const
		{
			return mAngle;
		}

		const IntPoint& getCenter() const
		{
			return mCenter;
		}

		const FloatRect& getRect() const
		{
			return mRect;
		}

	private:
		void deserialization(pugi::xml_node _node, Version _version) override
		{
			for (auto prop : _node.children("Property"))
			{
				std::string_view key = prop.attribute("key").value();
				std::string_view value = prop.attribute("value").value();
				if (key == "Angle")
					mAngle = utility::parseFloat(value);
				if (key == "Center")
					mCenter = IntPoint::parse(value);
			}

			std::string texture{_node.parent().parent().attribute("texture").value()};

			// tags replacement support for Skins
			if (_version >= Version(1, 1))
			{
				texture = LanguageManager::getInstance().replaceTags(texture);
			}

			const IntSize& size = texture_utility::getTextureSize(texture);
			IntCoord coord = IntCoord::parse(_node.attribute("offset").value());
			mRect = CoordConverter::convertTextureCoord(coord, size);
		}

	private:
		FloatRect mRect;
		IntPoint mCenter;
		float mAngle{0}; // Angle in radians
	};


	class MYGUI_EXPORT EditTextStateInfo : public IStateInfo
	{
		MYGUI_RTTI_DERIVED(EditTextStateInfo)

	public:
		const Colour& getColour() const
		{
			return mColour;
		}

		bool getShift() const
		{
			return mShift;
		}

	private:
		void deserialization(pugi::xml_node _node, Version _version) override
		{
			mShift = utility::parseBool(_node.attribute("shift").value());

			std::string colour{_node.attribute("colour").value()};
			if (_version >= Version(1, 1))
			{
				colour = LanguageManager::getInstance().replaceTags(colour);
			}

			mColour = Colour::parse(colour);
		}

	private:
		Colour mColour{Colour::White};
		bool mShift{false};
	};

} // namespace MyGUI

#endif // MYGUI_COMMON_STATE_INFO_H_
