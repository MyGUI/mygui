/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourceManualFont.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_TextureUtility.h"

namespace MyGUI
{

	const GlyphInfo* ResourceManualFont::getGlyphInfo(Char _id) const
	{
		CharMap::const_iterator iter = mCharMap.find(_id);

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

	void ResourceManualFont::deserialize(pugi::xml_node _node, Version _version)
	{
		Base::deserialize(_node, _version);

		for (auto node : _node)
		{
			if (node.name() == std::string_view("Property"))
			{
				std::string_view key = node.attribute("key").value();
				std::string_view value = node.attribute("value").value();
				if (key == "Source")
					mSource = value;
				else if (key == "DefaultHeight")
					mDefaultHeight = utility::parseInt(value);
				else if (key == "Shader")
					mShader = value;
			}
		}

		loadTexture();

		if (mTexture != nullptr)
		{
			if (!mShader.empty())
				mTexture->setShader(mShader);
			int textureWidth = mTexture->getWidth();
			int textureHeight = mTexture->getHeight();

			for (auto node2 : _node)
			{
				if (node2.name() == std::string_view("Codes"))
				{
					for (auto element : node2.children("Code"))
					{
						std::string value{element.attribute("index").value()};
						if (!value.empty())
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

							FloatPoint bearing(utility::parseValue<FloatPoint>(element.attribute("bearing").value()));

							// texture coordinates
							FloatCoord coord(utility::parseValue<FloatCoord>(element.attribute("coord").value()));

							// glyph size, default to texture coordinate size
							std::string sizeString{element.attribute("size").value()};
							FloatSize size(coord.width, coord.height);
							if (!sizeString.empty())
							{
								size = utility::parseValue<FloatSize>(sizeString);
							}

							std::string advanceAttribute{element.attribute("advance").value()};
							float advance = size.width;
							if (!advanceAttribute.empty())
								advance = utility::parseValue<float>(advanceAttribute);

							mCharMap.emplace(
								id,
								GlyphInfo{
									id,
									size.width,
									size.height,
									advance,
									bearing.left,
									bearing.top,
									FloatRect{
										coord.left / textureWidth,
										coord.top / textureHeight,
										coord.right() / textureWidth,
										coord.bottom() / textureHeight}});

							if (id == FontCodeType::NotDefined)
								mSubstituteGlyphInfo = &mCharMap.at(FontCodeType::NotDefined);
						}
					}
				}
			}
		}
	}

	ITexture* ResourceManualFont::getTextureFont() const
	{
		return mTexture;
	}

	int ResourceManualFont::getDefaultHeight() const
	{
		return mDefaultHeight;
	}

	void ResourceManualFont::setSource(std::string_view value)
	{
		mTexture = nullptr;
		mSource = value;
		loadTexture();
	}

	void ResourceManualFont::setShader(std::string_view value)
	{
		mShader = value;
		if (mTexture != nullptr)
			mTexture->setShader(mShader);
	}

	void ResourceManualFont::setTexture(ITexture* texture)
	{
		mTexture = texture;
		mSource.clear();
	}

	void ResourceManualFont::setDefaultHeight(int value)
	{
		mDefaultHeight = value;
	}

	void ResourceManualFont::addGlyphInfo(Char id, const GlyphInfo& info)
	{
		GlyphInfo& inserted = mCharMap.insert(CharMap::value_type(id, info)).first->second;

		if (id == FontCodeType::NotDefined)
			mSubstituteGlyphInfo = &inserted;
	}

} // namespace MyGUI
