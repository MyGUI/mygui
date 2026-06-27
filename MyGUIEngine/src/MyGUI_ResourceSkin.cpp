/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_SubWidgetManager.h"

namespace MyGUI
{

	ResourceSkin::~ResourceSkin()
	{
		clear();
		mStates.clear();
	}

	void ResourceSkin::deserialize(pugi::xml_node _node, Version _version)
	{
		Base::deserialize(_node, _version);

		const std::string& stateCategory = SubWidgetManager::getInstance().getStateCategoryName();

		std::string name{_node.attribute("name").value()};
		std::string texture{_node.attribute("texture").value()};
		IntSize size;
		std::string tmp{_node.attribute("size").value()};
		if (!tmp.empty())
			size = IntSize::parse(tmp);

		LanguageManager& localizator = LanguageManager::getInstance();

		// helper class for binding subskins
		SubWidgetBinding bind;

		// tags replacement support for Skins
		if (_version >= Version(1, 1))
		{
			texture = localizator.replaceTags(texture);
		}

		setInfo(size, texture);

		tmp = _node.attribute("mask").value();
		if (!tmp.empty())
			addProperty("MaskPick", tmp);

		for (auto basis : _node)
		{
			if (basis.name() == std::string_view("Property"))
			{
				std::string key{basis.attribute("key").value()};
				if (key.empty())
					continue;
				std::string value{basis.attribute("value").value()};
				if (value.empty())
					continue;

				// tags replacement support for Skins
				if (_version >= Version(1, 1))
				{
					value = localizator.replaceTags(value);
				}

				addProperty(key, value);
			}
			else if (basis.name() == std::string_view("Child"))
			{
				ChildSkinInfo child(
					basis.attribute("type").value(),
					WidgetStyle::parse(basis.attribute("style").value()),
					basis.attribute("skin").value(),
					IntCoord::parse(basis.attribute("offset").value()),
					Align::parse(basis.attribute("align").value()),
					basis.attribute("layer").value(),
					basis.attribute("name").value());

				for (auto child_params : basis.children("Property"))
					child.addParam(child_params.attribute("key").value(), child_params.attribute("value").value());

				addChild(child);
			}
			else if (basis.name() == std::string_view("BasisSkin"))
			{
				std::string basisSkinType{basis.attribute("type").value()};
				IntCoord offset;
				Align align = Align::Default;
				tmp = basis.attribute("offset").value();
				if (!tmp.empty())
					offset = IntCoord::parse(tmp);
				tmp = basis.attribute("align").value();
				if (!tmp.empty())
					align = Align::parse(tmp);

				bind.create(offset, align, basisSkinType);

				// check for new state format
				bool new_format = false;
				// if version < 1.0 rename states
				if (_version < Version(1, 0))
				{
					for (auto state : basis)
					{
						if (state.name() == std::string_view("State"))
						{
							std::string_view name_state = state.attribute("name").value();
							if (name_state == "normal_checked" || name_state == "normal_check")
							{
								new_format = true;
								break;
							}
						}
					}
				}

				for (auto state : basis)
				{
					if (state.name() == std::string_view("State"))
					{
						std::string basisStateName{state.attribute("name").value()};

						// if version < 1.0 rename states
						if (_version < Version(1, 0))
						{
							// obsolete new types
							if (basisStateName == "disable_check")
								basisStateName = "disabled_checked";
							else if (basisStateName == "normal_check")
								basisStateName = "normal_checked";
							else if (basisStateName == "active_check")
								basisStateName = "highlighted_checked";
							else if (basisStateName == "pressed_check")
								basisStateName = "pushed_checked";
							else if (basisStateName == "disable")
								basisStateName = "disabled";
							else if (basisStateName == "active")
								basisStateName = "highlighted";
							else if (basisStateName == "select")
								basisStateName = "pushed";
							else if (basisStateName == "pressed")
							{
								if (new_format)
									basisStateName = "pushed";
								else
									basisStateName = "normal_checked";
							}
						}

						IStateInfo* data = nullptr;
						IObject* object = FactoryManager::getInstance().createObject(stateCategory, basisSkinType);
						if (object != nullptr)
						{
							data = object->castType<IStateInfo>();
							data->deserializationWrapper(state, _version);
						}

						bind.add(basisStateName, data, name);
					}
				}

				addInfo(bind);
			}
		}
	}

	void ResourceSkin::setInfo(const IntSize& _size, std::string_view _texture)
	{
		mSize = _size;
		mTexture = _texture;
	}

	void ResourceSkin::addInfo(const SubWidgetBinding& _bind)
	{
		checkState(_bind.mStates);
		mBasis.emplace_back(_bind.mType, _bind.mOffset, _bind.mAlign);
		checkBasis();
		fillState(_bind.mStates, mBasis.size() - 1);
	}

	void ResourceSkin::addProperty(std::string_view _key, std::string_view _value)
	{
		mapSet(mProperties, _key, _value);
	}

	void ResourceSkin::addChild(const ChildSkinInfo& _child)
	{
		mChilds.push_back(_child);
	}

	void ResourceSkin::clear()
	{
		for (auto& state : mStates)
		{
			for (auto& info : state.second)
				delete info;
		}
	}

	void ResourceSkin::checkState(const MapStateInfo& _states)
	{
		for (const auto& state : _states)
		{
			checkState(state.first);
		}
	}

	void ResourceSkin::checkState(std::string_view _name)
	{
		MapWidgetStateInfo::const_iterator iter = mStates.find(_name);
		if (iter == mStates.end())
		{
			mStates.emplace(_name, VectorStateInfo());
		}
	}

	void ResourceSkin::checkBasis()
	{
		// increase offset array size by number of subwidgets
		for (auto& state : mStates)
		{
			state.second.resize(mBasis.size());
		}
	}

	void ResourceSkin::fillState(const MapStateInfo& _states, size_t _index)
	{
		for (const auto& state : _states)
		{
			mStates[state.first][_index] = state.second;
		}
	}

	const IntSize& ResourceSkin::getSize() const
	{
		return mSize;
	}

	const std::string& ResourceSkin::getTextureName() const
	{
		return mTexture;
	}

	const VectorSubWidgetInfo& ResourceSkin::getBasisInfo() const
	{
		return mBasis;
	}

	const MapWidgetStateInfo& ResourceSkin::getStateInfo() const
	{
		return mStates;
	}

	const MapString& ResourceSkin::getProperties() const
	{
		return mProperties;
	}

	const VectorChildSkinInfo& ResourceSkin::getChild() const
	{
		return mChilds;
	}

	const std::string& ResourceSkin::getSkinName() const
	{
		return mSkinName;
	}

} // namespace MyGUI
