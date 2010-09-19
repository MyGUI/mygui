/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#ifndef __MYGUI_RESOURCE_SKIN_H__
#define __MYGUI_RESOURCE_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_SubWidgetBinding.h"
#include "MyGUI_ChildSkinInfo.h"
#include "MyGUI_MaskPickInfo.h"
#include "MyGUI_IResource.h"
#include "MyGUI_SubWidgetInfo.h"

namespace MyGUI
{

	// вспомогательный класс для инициализации одного скина
	class MYGUI_EXPORT ResourceSkin :
		public IResource
	{
		MYGUI_RTTI_DERIVED( ResourceSkin )

	public:
		ResourceSkin();
		virtual ~ResourceSkin();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		const IntSize& getSize() const;
		const std::string& getTextureName() const;
		const VectorSubWidgetInfo& getBasisInfo() const;
		const MapWidgetStateInfo& getStateInfo() const;
		const MapString& getProperties() const;
		const VectorChildSkinInfo& getChild() const;
		const std::string& getSkinName() const;

	private:
		void setInfo(const IntSize& _size, const std::string& _texture);
		void addInfo(const SubWidgetBinding& _bind);
		void addProperty(const std::string& _key, const std::string& _value);
		void addChild(const ChildSkinInfo& _child);
		void clear();

		void checkState(const MapStateInfo& _states);
		void checkState(const std::string& _name);
		void checkBasis();
		void fillState(const MapStateInfo& _states, size_t _index);

	private:
		IntSize mSize;
		std::string mTexture;
		VectorSubWidgetInfo mBasis;
		MapWidgetStateInfo mStates;
		// дополнительные параметры скина
		MapString mProperties;
		// дети скина
		VectorChildSkinInfo mChilds;
		std::string mSkinName;
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_SKIN_H__
