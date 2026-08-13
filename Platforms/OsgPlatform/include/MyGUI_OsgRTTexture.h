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
#include <vector>

#include <osg/ref_ptr>

namespace osg
{
	class Array;
	class Camera;
	class State;
	class StateSet;
	class Texture2D;
	class VertexBufferObject;
}

namespace MyGUI
{
	class OsgRenderManager;
	class RTTDrawable;

	// Defines the necessary information for a draw call
	struct Batch
	{
		// May be empty
		osg::ref_ptr<osg::Texture2D> mTexture;

		osg::ref_ptr<osg::VertexBufferObject> mVertexBuffer;
		// need to hold on to this too as the mVertexBuffer does not hold a ref to its own array
		osg::ref_ptr<osg::Array> mArray;

		// optional
		osg::ref_ptr<osg::StateSet> mStateSet;

		size_t mVertexCount;
	};

	// Draws the given batches with generic vertex attributes 0 (position), 3 (colour)
	// and 8 (texcoord 0) through the state's shader. Used by both the main GUI
	// drawable and the render-to-texture drawable.
	void osgDrawBatches(
		osg::State* _state,
		osg::StateSet* _stateSet,
		const std::vector<Batch>& _batches,
		osg::Texture2D* _dummyTexture);

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
		OsgRenderManager* mRenderManager{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_OSG_RT_TEXTURE_H_
