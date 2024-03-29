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

	void ResourceSkin::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);

		const std::string& stateCategory = SubWidgetManager::getInstance().getStateCategoryName();

		// парсим атрибуты скина
		std::string name;
		std::string texture;
		std::string tmp;
		IntSize size;
		_node->findAttribute("name", name);
		_node->findAttribute("texture", texture);
		if (_node->findAttribute("size", tmp))
			size = IntSize::parse(tmp);

		LanguageManager& localizator = LanguageManager::getInstance();

		// вспомогательный класс для биндинга сабскинов
		SubWidgetBinding bind;

		// tags replacement support for Skins
		if (_version >= Version(1, 1))
		{
			texture = localizator.replaceTags(texture);
		}

		setInfo(size, texture);

		// проверяем маску
		if (_node->findAttribute("mask", tmp))
			addProperty("MaskPick", tmp);

		// берем детей и крутимся, цикл с саб скинами
		xml::ElementEnumerator basis = _node->getElementEnumerator();
		while (basis.next())
		{
			if (basis->getName() == "Property")
			{
				// загружаем свойства
				std::string key;
				std::string value;
				if (!basis->findAttribute("key", key))
					continue;
				if (!basis->findAttribute("value", value))
					continue;

				// tags replacement support for Skins
				if (_version >= Version(1, 1))
				{
					value = localizator.replaceTags(value);
				}

				// добавляем свойство
				addProperty(key, value);
			}
			else if (basis->getName() == "Child")
			{
				ChildSkinInfo child(
					basis->findAttribute("type"),
					WidgetStyle::parse(basis->findAttribute("style")),
					basis->findAttribute("skin"),
					IntCoord::parse(basis->findAttribute("offset")),
					Align::parse(basis->findAttribute("align")),
					basis->findAttribute("layer"),
					basis->findAttribute("name"));

				xml::ElementEnumerator child_params = basis->getElementEnumerator();
				while (child_params.next("Property"))
					child.addParam(child_params->findAttribute("key"), child_params->findAttribute("value"));

				addChild(child);
				//continue;
			}
			else if (basis->getName() == "BasisSkin")
			{
				// парсим атрибуты
				std::string basisSkinType;
				std::string tmp_str;
				IntCoord offset;
				Align align = Align::Default;
				basis->findAttribute("type", basisSkinType);
				if (basis->findAttribute("offset", tmp_str))
					offset = IntCoord::parse(tmp_str);
				if (basis->findAttribute("align", tmp_str))
					align = Align::parse(tmp_str);

				bind.create(offset, align, basisSkinType);

				// берем детей и крутимся, цикл со стейтами
				xml::ElementEnumerator state = basis->getElementEnumerator();

				// проверяем на новый формат стейтов
				bool new_format = false;
				// если версия меньше 1.0 то переименовываем стейты
				if (_version < Version(1, 0))
				{
					while (state.next())
					{
						if (state->getName() == "State")
						{
							std::string_view name_state = state->findAttribute("name");
							if ((name_state == "normal_checked") || (state->findAttribute("name") == "normal_check"))
							{
								new_format = true;
								break;
							}
						}
					}
					// обновляем
					state = basis->getElementEnumerator();
				}

				while (state.next())
				{
					if (state->getName() == "State")
					{
						// парсим атрибуты стейта
						std::string basisStateName;
						state->findAttribute("name", basisStateName);

						// если версия меньше 1.0 то переименовываем стейты
						if (_version < Version(1, 0))
						{
							// это обсолет новых типов
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

						// конвертируем инфу о стейте
						IStateInfo* data = nullptr;
						IObject* object = FactoryManager::getInstance().createObject(stateCategory, basisSkinType);
						if (object != nullptr)
						{
							data = object->castType<IStateInfo>();
							data->deserialization(state.current(), _version);
						}

						// добавляем инфо о стайте
						bind.add(basisStateName, data, name);
					}
				}

				// теперь всё вместе добавляем в скин
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
		// ищем такой же ключ
		MapWidgetStateInfo::const_iterator iter = mStates.find(_name);
		if (iter == mStates.end())
		{
			// добавляем новый стейт
			mStates.emplace(_name, VectorStateInfo());
		}
	}

	void ResourceSkin::checkBasis()
	{
		// и увеличиваем размер смещений по колличеству сабвиджетов
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
