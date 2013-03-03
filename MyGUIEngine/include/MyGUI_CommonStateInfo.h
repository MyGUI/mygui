/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_COMMON_STATE_INFO_H__
#define __MYGUI_COMMON_STATE_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IStateInfo.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_TextureUtility.h"

namespace MyGUI
{

	class MYGUI_EXPORT SubSkinStateInfo :
		public IStateInfo
	{
		MYGUI_RTTI_DERIVED( SubSkinStateInfo )

	public:
		virtual ~SubSkinStateInfo() { }

		const FloatRect& getRect() const
		{
			return mRect;
		}

	private:
		virtual void deserialization(xml::ElementPtr _node, Version _version)
		{
			std::string texture = _node->getParent()->getParent()->findAttribute("texture");

			// поддержка замены тегов в скинах
			if (_version >= Version(1, 1))
			{
				texture = LanguageManager::getInstance().replaceTags(texture);
			}

			const IntSize& size = texture_utility::getTextureSize(texture);
			const IntCoord& coord = IntCoord::parse(_node->findAttribute("offset"));
			mRect = CoordConverter::convertTextureCoord(coord, size);
		}

	private:
		FloatRect mRect;
	};

	class MYGUI_EXPORT TileRectStateInfo :
		public IStateInfo
	{
		MYGUI_RTTI_DERIVED( TileRectStateInfo )

	public:
		TileRectStateInfo() :
			mTileH(true),
			mTileV(true)
		{
		}

		virtual ~TileRectStateInfo() { }

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
		virtual void deserialization(xml::ElementPtr _node, Version _version)
		{
			std::string texture = _node->getParent()->getParent()->findAttribute("texture");

			// поддержка замены тегов в скинах
			if (_version >= Version(1, 1))
			{
				texture = LanguageManager::getInstance().replaceTags(texture);
			}

			const IntSize& size = texture_utility::getTextureSize(texture);
			const IntCoord& coord = IntCoord::parse(_node->findAttribute("offset"));
			mRect = CoordConverter::convertTextureCoord(coord, size);

			xml::ElementEnumerator prop = _node->getElementEnumerator();
			while (prop.next("Property"))
			{
				const std::string& key = prop->findAttribute("key");
				const std::string& value = prop->findAttribute("value");
				if (key == "TileH") mTileH = utility::parseBool(value);
				else if (key == "TileV") mTileV = utility::parseBool(value);
				else if (key == "TileSize") mTileSize = IntSize::parse(value);
			}
		}

	private:
		FloatRect mRect;
		IntSize mTileSize;
		bool mTileH;
		bool mTileV;
	};

	class MYGUI_EXPORT EditTextStateInfo :
		public IStateInfo
	{
		MYGUI_RTTI_DERIVED( EditTextStateInfo )

	public:
		EditTextStateInfo() :
			mColour(Colour::White),
			mShift(false)
		{
		}

		virtual ~EditTextStateInfo() { }

		const Colour& getColour() const
		{
			return mColour;
		}

		bool getShift() const
		{
			return mShift;
		}

	private:
		virtual void deserialization(xml::ElementPtr _node, Version _version)
		{
			mShift = utility::parseBool(_node->findAttribute("shift"));

			std::string colour = _node->findAttribute("colour");
			if (_version >= Version(1, 1))
			{
				colour = LanguageManager::getInstance().replaceTags(colour);
			}

			mColour = Colour::parse(colour);
		}

	private:
		Colour mColour;
		bool mShift;
	};

} // namespace MyGUI

#endif // __MYGUI_COMMON_STATE_INFO_H__
