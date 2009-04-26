/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/

#include "Widget.h"

namespace MyGUI
{

	Widget::Widget(const std::string& _texture) :
		mTexture(_texture)
	{
		// ������� ��� ����� �� ������
		mItems.push_back(new QuadDrawItem());
		mItems.push_back(new QuadDrawItem());
	}

	ILayerItem * Widget::getLayerItemByPoint(int _left, int _top)
	{
		return 0;
	}

	void Widget::attachItemToNode(ILayer* _layer, ILayerNode* _node)
	{
		// ��������� ������
		mLayer = _layer;
		mNode = _node;

		// ����� ����� �������, �������������� �����
		for (size_t index=0; index<mItems.size(); ++index)
		{
			mItems[index]->createDrawItem(mTexture, mNode);
		}

		// ������ �������������� �����, �� ��� �����, ���� �������� ������ � ����
		for (size_t index=0; index<mChilds.size(); ++index)
		{
			mChilds[index]->attachItemToNode(0, mNode);
		}
	}

	void Widget::detachFromLayer()
	{
	}

	void Widget::upLayerItem()
	{
	}

	Widget* Widget::createChild(const std::string& _texture)
	{
		Widget* item = new Widget(_texture);
		mChilds.push_back(item);

		return item;
	}

} // namespace MyGUI
