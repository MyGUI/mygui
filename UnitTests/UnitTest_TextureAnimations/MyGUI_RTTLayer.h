/*!
	@file
	@author		Albert Semenov
	@date		06/2008
*/

#ifndef MYGUI_RTT_LAYER_H_
#define MYGUI_RTT_LAYER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_OverlappedLayer.h"

namespace MyGUI
{

	class /*MYGUI_EXPORT */ RTTLayer : public OverlappedLayer
	{
		MYGUI_RTTI_DERIVED(RTTLayer)

	public:
		~RTTLayer() override;

		void deserialization(xml::ElementPtr _node, Version _version) override;

		// создаем дочерний нод
		ILayerNode* createChildItemNode() override;
		// удаляем дочерний нод
		void destroyChildItemNode(ILayerNode* _item) override;

		// возвращает виджет по позиции
		ILayerItem* getLayerItemByPoint(int _left, int _top) const override;

		// рисует леер
		void renderToTarget(IRenderTarget* _target, bool _update) override;

	private:
		Version mVersion;
		std::unique_ptr<xml::Element> mData;
	};

} // namespace MyGUI

#endif // MYGUI_RTT_LAYER_H_
