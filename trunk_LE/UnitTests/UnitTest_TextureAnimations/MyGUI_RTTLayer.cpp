/*!
	@file
	@author		Albert Semenov
	@date		02/2008
*/

#include "MyGUI_LayerItem.h"
#include "MyGUI_RTTLayer.h"
#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_FactoryManager.h"

namespace MyGUI
{

	RTTLayer::RTTLayer() :
		mData(nullptr)
	{
	}

	RTTLayer::~RTTLayer()
	{
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); )
		{
			if ((*iter) != nullptr)
			{
				// удаляем те чтобы ли приостановленны на анимацию
				RTTLayerNode* node = (*iter)->castType<RTTLayerNode>();
				if (node->getDestroy())
				{
					delete node;
					iter = mChildItems.erase(iter);
				}
				else
				{
					++iter;
				}
			}
			else
			{
				iter = mChildItems.erase(iter);
			}
		}

		delete mData;
	}

	void RTTLayer::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);

		mVersion = _version;
		mData = _node->createCopy();
	}

	ILayerNode* RTTLayer::createChildItemNode()
	{
		// создаем рутовый айтем
		RTTLayerNode* node = new RTTLayerNode(this);
		mChildItems.push_back(node);

		if (mData != nullptr)
		{
			FactoryManager& factory = FactoryManager::getInstance();

			MyGUI::xml::ElementEnumerator controller = mData->getElementEnumerator();
			while (controller.next())
			{
				IObject* object = factory.createObject(controller->getName(), controller->findAttribute("type"));
				if (object == nullptr) continue;

				LayerNodeAnimation* data = object->castType<LayerNodeAnimation>(false);
				if (data == nullptr)
				{
					factory.destroyObject(object);
					continue;
				}
				data->deserialization(controller.current(), mVersion);
				data->attach(node);
			}
		}

		return node;
	}

	void RTTLayer::destroyChildItemNode(ILayerNode* _item)
	{
		// если есть отец, то русть сам и удаляет
		ILayerNode* parent = _item->getParent();
		if (parent)
		{
			parent->destroyChildItemNode(_item);
			return;
		}

		// айтем рутовый, мы удаляем
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			if ((*iter) == _item)
			{
				RTTLayerNode* node = _item->castType<RTTLayerNode>();
				node->setDestroy(true);

				return;
			}
		}
		MYGUI_EXCEPT("item node not found");
	}

	void RTTLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); )
		{
			if ((*iter) != nullptr)
			{
				// если полное обновление и нод был отложен от удаления то удаляем
				RTTLayerNode* node = (*iter)->castType<RTTLayerNode>();

				if (_update)
				{
					if (node->getDestroy())
					{
						delete (*iter);
						*iter = nullptr;
						++iter;
						continue;
					}
				}

				node->renderToTarget(_target, _update);

				if (node->getDestroy() && !node->getAnimate())
				{
					delete (*iter);
					*iter = nullptr;
				}

				++iter;
			}
			else
			{
				iter = mChildItems.erase(iter);
			}
		}
	}

	ILayerItem* RTTLayer::getLayerItemByPoint(int _left, int _top) const
	{
		if (false == mIsPick) return nullptr;
		VectorILayerNode::const_reverse_iterator iter = mChildItems.rbegin();
		while (iter != mChildItems.rend())
		{
			if ((*iter) != nullptr)
			{
				ILayerItem* item = (*iter)->getLayerItemByPoint(_left, _top);
				if (item != nullptr) return item;
			}
			++iter;
		}
		return nullptr;
	}

} // namespace MyGUI
