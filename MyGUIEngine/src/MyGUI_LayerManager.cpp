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

	template <> LayerManager* Singleton<LayerManager>::msInstance = nullptr;
	template <> const char* Singleton<LayerManager>::mClassTypeName = "LayerManager";

	LayerManager::LayerManager() :
		mIsInitialise(false),
		mCategoryName("Layer")
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
		for (VectorLayer::iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
		{
			destroy(*iter);
		}
		mLayerNodes.clear();
	}

	void LayerManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		VectorLayer layers;
		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator layer = _node->getElementEnumerator();
		while (layer.next(mCategoryName))
		{

			std::string name;

			if ( !layer->findAttribute("name", name))
			{
				MYGUI_LOG(Warning, "Attribute 'name' not found (file : " << _file << ")");
				continue;
			}

			for (VectorLayer::iterator iter = layers.begin(); iter != layers.end(); ++iter)
			{
				MYGUI_ASSERT((*iter)->getName() != name, "Layer '" << name << "' already exist (file : " << _file << ")");
			}

			std::string type = layer->findAttribute("type");
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

	ILayer* LayerManager::_createLayerObject(const std::string& _type)
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
	void LayerManager::attachToLayerNode(const std::string& _name, Widget* _item)
	{
		MYGUI_ASSERT(nullptr != _item, "pointer must be valid");
		MYGUI_ASSERT(_item->isRootWidget(), "attached widget must be root");

		// сначала отсоединяем
		_item->detachFromLayer();

		// а теперь аттачим
		for (VectorLayer::iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
		{
			if (_name == (*iter)->getName())
			{
				ILayerNode* node = (*iter)->createChildItemNode();
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

	bool LayerManager::isExist(const std::string& _name) const
	{
		return getByName(_name, false) != nullptr;
	}

	void LayerManager::merge(VectorLayer& _layers)
	{
		for (VectorLayer::iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
		{
			if ((*iter) == nullptr) continue;
			bool find = false;
			std::string name = (*iter)->getName();
			for (VectorLayer::iterator iter2 = _layers.begin(); iter2 != _layers.end(); ++iter2)
			{
				if (name == (*iter2)->getName())
				{
					// заменяем новый слой, на уже существующий
					delete (*iter2);
					(*iter2) = (*iter);
					(*iter) = nullptr;
					find = true;
					break;
				}
			}
			if (!find)
			{
				destroy(*iter);
				(*iter) = nullptr;
			}
		}

		// теперь в основной
		mLayerNodes = _layers;
	}

	ILayer* LayerManager::createLayerAt(const std::string& _name, const std::string& _type, size_t _index)
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

	Widget* LayerManager::getWidgetFromPoint(int _left, int _top)
	{
		VectorLayer::reverse_iterator iter = mLayerNodes.rbegin();
		while (iter != mLayerNodes.rend())
		{
			ILayerItem* item = (*iter)->getLayerItemByPoint(_left, _top);
			if (item != nullptr) return static_cast<Widget*>(item);
			++iter;
		}
		return nullptr;
	}

	void LayerManager::renderToTarget(IRenderTarget* _target, bool _update)
	{
		for (VectorLayer::iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
		{
			(*iter)->renderToTarget(_target, _update);
		}
	}

	ILayer* LayerManager::getByName(const std::string& _name, bool _throw) const
	{
		for (VectorLayer::const_iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
		{
			if (_name == (*iter)->getName())
				return (*iter);
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
		for (VectorLayer::const_iterator iter = mLayerNodes.begin(); iter != mLayerNodes.end(); ++iter)
			(*iter)->resizeView(_viewSize);
	}

	size_t LayerManager::getLayerCount() const
	{
		return mLayerNodes.size();
	}

	ILayer* LayerManager::getLayer(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mLayerNodes.size(), "LayerManager::getLayer");
		return mLayerNodes[_index];
	}

	const std::string& LayerManager::getCategoryName() const
	{
		return mCategoryName;
	}

} // namespace MyGUI
