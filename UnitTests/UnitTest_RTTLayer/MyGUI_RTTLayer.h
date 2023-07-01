/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#ifndef MYGUI_RTT_LAYER_H_
#define MYGUI_RTT_LAYER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_OverlappedLayer.h"

namespace MyGUI
{

	class RTTLayer : public OverlappedLayer
	{
		MYGUI_RTTI_DERIVED(RTTLayer)

	public:
		~RTTLayer() override;

		void deserialization(xml::ElementPtr _node, Version _version) override;
		void renderToTarget(IRenderTarget* _target, bool _update) override;

		void setTextureSize(const IntSize& _size);
		void setTextureName(std::string_view _name);

	private:
		MyGUI::ITexture* mTexture{nullptr};
		IntSize mTextureSize;
		std::string mTextureName;
		bool mOutOfDateRtt{false};
	};

} // namespace MyGUI

#endif // MYGUI_RTT_LAYER_H_
