/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#ifndef __MYGUI_I_LAYER_H__
#define __MYGUI_I_LAYER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_IRenderTarget.h"
#include "MyGUI_ILayerNode.h"
#include "MyGUI_ISerializable.h"

namespace MyGUI
{

	class ILayerItem;

	class MYGUI_EXPORT ILayer :
		public ISerializable
	{
		MYGUI_RTTI_DERIVED( ILayer )

	public:
		ILayer() { }
		virtual ~ILayer() { }

		// ��� �����
		const std::string& getName() const
		{
			return mName;
		}

		// ������� �������� ���
		virtual ILayerNode* createChildItemNode() = 0;
		// ������� �������� ���
		virtual void destroyChildItemNode(ILayerNode* _node) = 0;

		// ��������� �������� ���
		virtual void upChildItemNode(ILayerNode* _node) = 0;

		// ������ �����
		virtual EnumeratorILayerNode getEnumerator() const = 0;

		// ���������� ������ �� �������
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const = 0;

		// ���������� ������� � ����������� �����
		virtual IntPoint getPosition(int _left, int _top) const = 0;

		// ���������� ������ �����
		virtual const IntSize& getSize() const = 0;

		// ������ ����
		virtual void renderToTarget(IRenderTarget* _target, bool _update) = 0;

	protected:
		std::string mName;
	};

} // namespace MyGUI

#endif // __MYGUI_I_LAYER_H__
