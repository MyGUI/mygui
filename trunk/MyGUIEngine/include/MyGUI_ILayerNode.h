/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
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
#ifndef __MYGUI_I_LAYER_NODE_H__
#define __MYGUI_I_LAYER_NODE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_IObject.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI
{

	class ILayer;
	class ILayerItem;
	class ILayerNode;

	class RenderItem;
	class ISubWidget;

	typedef std::vector<ILayerNode*> VectorILayerNode;
	typedef Enumerator<VectorILayerNode> EnumeratorILayerNode;

	class MYGUI_EXPORT ILayerNode : public IObject
	{
		MYGUI_RTTI_DERIVED( ILayerNode )

	public:
		virtual ~ILayerNode() { }

		// ����, �������� �� �����������
		virtual ILayer* getLayer() = 0;

		// ���������� ���� ��� nullptr
		virtual ILayerNode* getParent() = 0;

		// ������� �������� ���
		virtual ILayerNode* createChildItemNode() = 0;
		// ������� �������� ���
		virtual void destroyChildItemNode(ILayerNode* _node) = 0;

		// ��������� �������� ���
		virtual void upChildItemNode(ILayerNode* _node) = 0;

		// ������ �����
		virtual EnumeratorILayerNode getEnumerator() = 0;


		// ��������� ����� � ����
		virtual void attachLayerItem(ILayerItem* _item) = 0;
		// ������� ����� �� ����
		virtual void detachLayerItem(ILayerItem* _root) = 0;

		// ��������� ��� ����� � ���������� ������ �����
		virtual RenderItem* addToRenderItem(ITexture* _texture, ISubWidget* _item) = 0;
		// ���������� ���������� ����
		virtual void outOfDate(RenderItem* _item) = 0;

		// ���������� ������ �� �������
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) = 0;

		// ������ ����
		virtual void renderToTarget(IRenderTarget* _target, bool _update) = 0;

		virtual void dumpStatisticToLog(size_t _level) { }

	};

} // namespace MyGUI

#endif // __MYGUI_I_LAYER_NODE_H__
