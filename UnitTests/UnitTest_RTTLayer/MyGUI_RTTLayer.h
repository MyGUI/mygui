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

	class RTTLayer :
		public OverlappedLayer
	{
		MYGUI_RTTI_DERIVED( RTTLayer )

	public:
		RTTLayer();
		virtual ~RTTLayer();

		virtual void deserialization(xml::ElementPtr _node, Version _version);
		virtual void renderToTarget(IRenderTarget* _target, bool _update);

		void setTextureSize(const IntSize& _size);
		void setTextureName(const std::string& _name);

	private:
		MyGUI::ITexture* mTexture;
		IntSize mTextureSize;
		std::string mTextureName;
		bool mOutOfDate;
	};

} // namespace MyGUI

#endif // MYGUI_RTT_LAYER_H_
