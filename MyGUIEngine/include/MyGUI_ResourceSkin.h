/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_RESOURCE_SKIN_H_
#define MYGUI_RESOURCE_SKIN_H_

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

#endif // MYGUI_RESOURCE_SKIN_H_
