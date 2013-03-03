/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_RENDER_MANAGER_H__
#define __MYGUI_RENDER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI
{

	class MYGUI_EXPORT RenderManager :
		public Singleton<RenderManager>
	{
	public:

		/** Create vertex buffer.
			This method should create vertex buffer with triangles list type,
			each vertex have position, colour, texture coordinates.
		*/
		virtual IVertexBuffer* createVertexBuffer() = 0;
		/** Destroy vertex buffer */
		virtual void destroyVertexBuffer(IVertexBuffer* _buffer) = 0;

		/** Create empty texture instance */
		virtual ITexture* createTexture(const std::string& _name) = 0;
		/** Destroy texture */
		virtual void destroyTexture(ITexture* _texture) = 0;
		/** Get texture by name */
		virtual ITexture* getTexture(const std::string& _name) = 0;

		//FIXME возможно перенести в структуру о рендер таргете
		virtual const IntSize& getViewSize() const = 0;

		/** Get current vertex colour type */
		virtual VertexColourType getVertexFormat() = 0;

		/** Check if texture format supported by hardware */
		virtual bool isFormatSupported(PixelFormat _format, TextureUsage _usage);

#if MYGUI_DEBUG_MODE == 1
		/** Check if texture is valid */
		virtual bool checkTexture(ITexture* _texture);
#endif

	protected:
		virtual void onResizeView(const IntSize& _viewSize);
		virtual void onRenderToTarget(IRenderTarget* _target, bool _update);
		virtual void onFrameEvent(float _time);
	};

} // namespace MyGUI

#endif // __MYGUI_RENDER_MANAGER_H__
