/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_FactoryManager.h"

#include "MyGUI_SharedLayer.h"
#include "MyGUI_OverlappedLayer.h"

namespace MyGUI
{

	MYGUI_SINGLETON_DEFINITION(LayerManager);

	LayerManager::LayerManager() :
		mCategoryName("Layer"),
		mSingletonHolder(this)
	{
	}

	void LayerManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		WidgetManager::getInstance().registerUnlinker(this);
		ResourceManager::getInstance().registerLoadXmlDelegate(mCategoryName) = newDelegate(this, &LayerManager::_load);

		FactoryManager::getInstance().registerFactory<SharedLayer>(mCategoryName);
		FactoryManager::getInstance().registerFactory<OverlappedLayer>(mCategoryName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void LayerManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		FactoryManager::getInstance().unregisterFactory<SharedLayer>(mCategoryName);
		FactoryManager::getInstance().unregisterFactory<OverlappedLayer>(mCategoryName);

		// удаляем все хранители слоев
		clear();

		WidgetManager::getInstance().unregisterUnlinker(this);
		ResourceManager::getInstance().unregisterLoadXmlDelegate(mCategoryName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void LayerManager::clear()
	{
		for (auto& layerNode : mLayerNodes)
		{
			destroy(layerNode);
		}
		mLayerNodes.clear();
	}

	void LayerManager::_load(xml::ElementPtr _node, std::string_view _file, Version _version)
	{
		VectorLayer layers;
		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator layer = _node->getElementEnumerator();
		while (layer.next(mCategoryName))
		{
			std::string name;

			if (!layer->findAttribute("name", name))
			{
				MYGUI_LOG(Warning, "Attribute 'name' not found (file : " << _file << ")");
				continue;
			}

			for (const auto& item : layers)
			{
				MYGUI_ASSERT(item->getName() != name, "Layer '" << name << "' already exist (file : " << _file << ")");
			}

			std::string_view type = layer->findAttribute("type");
			if (type.empty() && _version <= Version(1, 0))
			{
				bool overlapped = utility::parseBool(layer->findAttribute("overlapped"));
				type = overlapped ? "OverlappedLayer" : "SharedLayer";
			}

			ILayer* item = _createLayerObject(type);
			item->deserialization(layer.current(), _version);

			layers.push_back(item);
		}

		// теперь мержим новые и старые слои
		merge(layers);
	}

	ILayer* LayerManager::_createLayerObject(std::string_view _type)
	{
		IObject* object = FactoryManager::getInstance().createObject(mCategoryName, _type);
		MYGUI_ASSERT(object != nullptr, "factory '" << _type << "' is not found");

		return object->castType<ILayer>();
	}

	void LayerManager::_unlinkWidget(Widget* _widget)
	{
		detachFromLayer(_widget);
	}

	// поправить на виджет и проверять на рутовость
	void LayerManager::attachToLayerNode(std::string_view _name, Widget* _item)
	{
		MYGUI_ASSERT(nullptr != _item, "pointer must be valid");
		MYGUI_ASSERT(_item->isRootWidget(), "attached widget must be root");

		// сначала отсоединяем
		_item->detachFromLayer();

		// а теперь аттачим
		for (auto& layerNode : mLayerNodes)
		{
			if (_name == layerNode->getName())
			{
				ILayerNode* node = layerNode->createChildItemNode();
				node->attachLayerItem(_item);

				return;
			}
		}
		MYGUI_LOG(Error, "Layer '" << _name << "' is not found");
		//MYGUI_EXCEPT("Layer '" << _name << "' is not found");
	}

	void LayerManager::detachFromLayer(Widget* _item)
	{
		MYGUI_ASSERT(nullptr != _item, "pointer must be valid");
		_item->detachFromLayer();
	}

	void LayerManager::upLayerItem(Widget* _item)
	{
		MYGUI_ASSERT(nullptr != _item, "pointer must be valid");
		_item->upLayerItem();
	}

	bool LayerManager::isExist(std::string_view _name) const
	{
		return getByName(_name, false) != nullptr;
	}

	void LayerManager::merge(VectorLayer& _layers)
	{
		for (auto& layerNode : mLayerNodes)
		{
			if (layerNode == nullptr)
				continue;
			bool find = false;
			const std::string& name = layerNode->getName();
			for (auto& layer : _layers)
			{
				if (name == layer->getName())
				{
					// заменяем новый слой, на уже существующий
					delete layer;
					layer = layerNode;
					layerNode = nullptr;
					find = true;
					break;
				}
			}
			if (!find)
			{
				destroy(layerNode);
				layerNode = nullptr;
			}
		}

		// теперь в основной
		mLayerNodes = _layers;
	}

	ILayer* LayerManager::createLayerAt(std::string_view _name, std::string_view _type, size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mLayerNodes.size() + 1, "LayerManager::getLayer");

		ILayer* item = _createLayerObject(_type);
		item->setName(_name);

		mLayerNodes.insert(mLayerNodes.begin() + _index, item);

		return item;
	}

	void LayerManager::destroy(ILayer* _layer)
	{
		MYGUI_LOG(Info, "destroy layer '" << _layer->getName() << "'");
		delete _layer;
	}

	Widget* LayerManager::getWidgetFromPoint(int _left, int _top) const
	{
		VectorLayer::const_reverse_iterator iter = mLayerNodes.rbegin();
		while (iter != mLayerNodes.rend())
		{
			ILayerItem* item = (*iter)->getLayerItemByPoint(_left, _top);
			if (item != nullptr)
				return static_cast<Widget*>(item);
			++iter;
		}
		return nullptr;
	}

	void LayerManager::renderToTarget(IRenderTarget* _target, bool _update)
	{
		for (auto& layerNode : mLayerNodes)
		{
			layerNode->renderToTarget(_target, _update);
		}
	}

	ILayer* LayerManager::getByName(std::string_view _name, bool _throw) const
	{
		for (const auto& layerNode : mLayerNodes)
		{
			if (_name == layerNode->getName())
				return layerNode;
		}
		MYGUI_ASSERT(!_throw, "Layer '" << _name << "' not found");
		return nullptr;
	}

	LayerManager::EnumeratorLayer LayerManager::getEnumerator() const
	{
		return EnumeratorLayer(mLayerNodes);
	}

	void LayerManager::resizeView(const IntSize& _viewSize)
	{
		for (const auto& node : mLayerNodes)
			node->resizeView(_viewSize);
	}

	size_t LayerManager::getLayerCount() const
	{
		return mLayerNodes.size();
	}

	ILayer* LayerManager::getLayer(size_t _index) const
	{
		MYGUI_ASSERT_RANGE(_index, mLayerNodes.size(), "LayerManager::getLayer");
		return mLayerNodes[_index];
	}

	const std::string& LayerManager::getCategoryName() const
	{
		return mCategoryName;
	}

} // namespace MyGUI
