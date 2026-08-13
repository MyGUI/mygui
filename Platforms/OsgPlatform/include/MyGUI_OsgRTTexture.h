/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#ifndef MYGUI_OSG_RT_TEXTURE_H_
#define MYGUI_OSG_RT_TEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IRenderTarget.h"
#include "MyGUI_RenderFormat.h"

#include <cstddef>

#include <osg/ref_ptr>

namespace osg
{
	class Camera;
	class Texture2D;
}

namespace MyGUI
{
	class OsgRenderManager;
	class RTTDrawable;

	class OsgRTTexture : public IRenderTarget
	{
	public:
		OsgRTTexture(osg::Texture2D* _texture, OsgRenderManager* _manager, int _width, int _height);
		~OsgRTTexture() override;

		void begin() override;
		void end() override;
		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;

		const RenderTargetInfo& getInfo() const override;

	private:
		RenderTargetInfo mInfo;

		osg::ref_ptr<osg::Camera> mCamera;
		osg::ref_ptr<RTTDrawable> mDrawable;
		OsgRenderManager* mRenderManager;
	};

} // namespace MyGUI

#endif // MYGUI_OSG_RT_TEXTURE_H_
