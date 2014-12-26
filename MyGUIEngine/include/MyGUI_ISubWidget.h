/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_SUB_WIDGET_H_
#define MYGUI_I_SUB_WIDGET_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ICroppedRectangle.h"
#include "MyGUI_ILayerNode.h"
#include "MyGUI_Types.h"
#include "MyGUI_IRenderTarget.h"
#include "MyGUI_IStateInfo.h"
#include "MyGUI_IObject.h"

namespace MyGUI
{

	class ISubWidget;
	typedef std::vector<ISubWidget*> VectorSubWidget;

	class MYGUI_EXPORT ISubWidget :
		public ICroppedRectangle,
		public IObject
	{
		MYGUI_RTTI_DERIVED( ISubWidget )

	public:
		ISubWidget() : mVisible(true) { }
		virtual ~ISubWidget() { }

		virtual void createDrawItem(ITexture* _texture, ILayerNode* _node) = 0;
		virtual void destroyDrawItem() = 0;

		virtual void setAlpha(float _alpha) { }

		virtual void setStateData(IStateInfo* _data) { }

		virtual void doRender() = 0;

		virtual void setAlign(Align _value)
		{
			mAlign = _value;
		}
		virtual void setVisible(bool _value)
		{
			mVisible = _value;
		}

		virtual void _updateView() { }
		virtual void _correctView() { }

		virtual void _setAlign(const IntSize& _oldsize)  { }

		virtual void doManualRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) { }

	protected:
		Align mAlign;
		bool mVisible;
	};

} // namespace MyGUI

#endif // MYGUI_I_SUB_WIDGET_H_
