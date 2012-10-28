/*!
	@file
	@author		Albert Semenov
	@date		06/2009
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourceManualFont.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_TextureUtility.h"

namespace MyGUI
{

	ResourceManualFont::ResourceManualFont() :
		mDefaultHeight(0),
		mSubstituteGlyphInfo(nullptr),
		mTexture(nullptr)
	{
	}

	ResourceManualFont::~ResourceManualFont()
	{
	}

	GlyphInfo* ResourceManualFont::getGlyphInfo(Char _id)
	{
		CharMap::iterator iter = mCharMap.find(_id);

		if (iter != mCharMap.end())
			return &iter->second;

		return mSubstituteGlyphInfo;
	}

	void ResourceManualFont::loadTexture()
	{
		if (mTexture == nullptr)
		{
			RenderManager& render = RenderManager::getInstance();
			mTexture = render.getTexture(mSource);
			if (mTexture == nullptr)
			{
				mTexture = render.createTexture(mSource);
				if (mTexture != nullptr)
					mTexture->loadFromFile(mSource);
			}
		}
	}

	void ResourceManualFont::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);

		xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Property")
			{
				const std::string& key = node->findAttribute("key");
				const std::string& value = node->findAttribute("value");
				if (key == "Source") mSource = value;
				else if (key == "DefaultHeight") mDefaultHeight = utility::parseInt(value);
			}
		}

		loadTexture();

		if (mTexture != nullptr)
		{
			int textureWidth = mTexture->getWidth();
			int textureHeight = mTexture->getHeight();

			node = _node->getElementEnumerator();
			while (node.next())
			{
				if (node->getName() == "Codes")
				{
					xml::ElementEnumerator element = node->getElementEnumerator();
					while (element.next("Code"))
					{
						std::string value;
						// описане глифов
						if (element->findAttribute("index", value))
						{
							Char id = 0;
							if (value == "cursor")
								id = static_cast<Char>(FontCodeType::Cursor);
							else if (value == "selected")
								id = static_cast<Char>(FontCodeType::Selected);
							else if (value == "selected_back")
								id = static_cast<Char>(FontCodeType::SelectedBack);
							else if (value == "substitute")
								id = static_cast<Char>(FontCodeType::NotDefined);
							else
								id = utility::parseUInt(value);

							float advance(utility::parseValue<float>(element->findAttribute("advance")));
							FloatPoint bearing(utility::parseValue<FloatPoint>(element->findAttribute("bearing")));
							FloatCoord coord(utility::parseValue<FloatCoord>(element->findAttribute("coord")));

							if (advance == 0.0f)
								advance = coord.width;

							GlyphInfo& glyphInfo = mCharMap.insert(CharMap::value_type(id, GlyphInfo(
								id,
								coord.width,
								coord.height,
								advance,
								bearing.left,
								bearing.top,
								FloatRect(
									coord.left / textureWidth,
									coord.top / textureHeight,
									coord.right() / textureWidth,
									coord.bottom() / textureHeight)
							))).first->second;

							if (id == FontCodeType::NotDefined)
								mSubstituteGlyphInfo = &glyphInfo;
						}
					}
				}
			}
		}
	}

	ITexture* ResourceManualFont::getTextureFont()
	{
		return mTexture;
	}

	int ResourceManualFont::getDefaultHeight()
	{
		return mDefaultHeight;
	}

} // namespace MyGUI
