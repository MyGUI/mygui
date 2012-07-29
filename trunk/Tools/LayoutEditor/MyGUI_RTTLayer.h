/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#ifndef _a14ff7f6_117d_4239_aafd_0dc59d7adfe1_
#define _a14ff7f6_117d_4239_aafd_0dc59d7adfe1_

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
		virtual void resizeView(const IntSize& _viewSize);

		void setTextureSize(const IntSize& _size);
		void setTextureName(const std::string& _name);

		virtual const IntSize& getSize() const;

	private:
		MyGUI::ITexture* mTexture;
		IntSize mTextureSize;
		std::string mTextureName;
		bool mOutOfDate;
	};

}

#endif
