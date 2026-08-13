/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#include "MyGUI_OsgRTTexture.h"
#include "MyGUI_OsgDiagnostic.h"
#include "MyGUI_OsgRenderManager.h"
#include "MyGUI_OsgTexture.h"
#include "MyGUI_OsgVertexBuffer.h"

#include <osg/BufferObject>
#include <osg/Camera>
#include <osg/Drawable>
#include <osg/GL>
#include <osg/GLExtensions>
#include <osg/Matrix>
#include <osg/Program>
#include <osg/State>
#include <osg/StateSet>
#include <osg/Texture2D>
#include <osg/Uniform>

namespace MyGUI
{

	class RTTDrawable : public osg::Drawable
	{
	public:
		RTTDrawable() :
			RTTDrawable(nullptr, 256, 256)
		{
		}

		RTTDrawable(OsgRenderManager* _manager, int _width, int _height) :
			mWidth(_width),
			mHeight(_height)
		{
			setSupportsDisplayList(false);
			setCullingActive(false);

			mStateSet = new osg::StateSet;
			// the RTT camera's viewport is not applied by osg before the drawable
			// draws, so it has to be applied as part of the drawable's state
			mStateSet->setAttribute(new osg::Viewport(0, 0, _width, _height));
			applyGuiDrawableStateModes(mStateSet);

			// render the RTT content with the same default shader as the main GUI
			if (_manager)
			{
				if (osg::Program* program = _manager->getShaderProgram("Default"))
				{
					mStateSet->setAttributeAndModes(program, osg::StateAttribute::ON);
					mStateSet->addUniform(new osg::Uniform("Texture", 0));
				}
			}

			mDummyTexture = createDummyTexture();
		}

		RTTDrawable(const RTTDrawable& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY) :
			osg::Drawable(copy, copyop),
			mDummyTexture(copy.mDummyTexture),
			mStateSet(copy.mStateSet),
			mWidth(copy.mWidth),
			mHeight(copy.mHeight)
		{
		}

		void drawImplementation(osg::RenderInfo& renderInfo) const override
		{
			osgDrawBatches(renderInfo.getState(), mStateSet.get(), mBatches, mDummyTexture.get());
		}

		void addBatch(const Batch& batch)
		{
			mBatches.push_back(batch);
		}

		void clear()
		{
			mBatches.clear();
		}

	META_Object(osgMyGUI, RTTDrawable)

		private :
		// the batches from the last redraw are kept until the next redraw, so the
		// RTT content stays valid even when the layer is not redrawn for a frame
		std::vector<Batch> mBatches;

		osg::ref_ptr<osg::Texture2D> mDummyTexture;
		osg::ref_ptr<osg::StateSet> mStateSet;
		int mWidth;
		int mHeight;
	};

	OsgRTTexture::OsgRTTexture(osg::Texture2D* _texture, OsgRenderManager* _manager, int _width, int _height) :
		mRenderManager(_manager)
	{
		mInfo.maximumDepth = 1.0f;
		mInfo.hOffset = 0;
		mInfo.vOffset = 0;
		mInfo.aspectCoef = float(_height) / float(_width);
		mInfo.pixScaleX = 1.0f / float(_width);
		mInfo.pixScaleY = 1.0f / float(_height);

		// the RTT content is stored in the texture with an RGBA8 internal format so
		// that it can be sampled directly by the main GUI drawable
		_texture->setInternalFormat(GL_RGBA8);

		mCamera = new osg::Camera;
		mCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
		mCamera->attach(osg::Camera::COLOR_BUFFER, _texture);
		mCamera->setViewport(0, 0, _width, _height);
		mCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		mCamera->setViewMatrix(osg::Matrix::identity());
		// render upside down so the texture can be sampled top-down, matching the way
		// the engine stores its textures (see the OpenGL3 backend's YScale handling)
		mCamera->setProjectionMatrix(osg::Matrix::scale(1.0, -1.0, 1.0));
		// a PRE_RENDER camera is given its own render stage (and therefore its own
		// framebuffer object), which is required for render-to-texture; a nested
		// camera would render into the parent's framebuffer instead
		mCamera->setRenderOrder(osg::Camera::PRE_RENDER);
		mCamera->setClearColor(osg::Vec4(0, 0, 0, 0));
		mCamera->setClearMask(GL_COLOR_BUFFER_BIT);
		mCamera->setCullingActive(false);

		mDrawable = new RTTDrawable(_manager, _width, _height);
		mCamera->addChild(mDrawable.get());
		mRenderManager = _manager;

		// the camera is added to the GUI camera by the render manager at the next collect draw
		// calls, so the scene graph is not modified while the GUI camera's children are being
		// traversed (which would invalidate osg's traversal iterators)
		_manager->queueRTTAdd(mCamera.get());
	}

	OsgRTTexture::~OsgRTTexture()
	{
		// defer the removal to the next collect draw calls for the same reason the insertion is deferred
		if (mCamera.valid())
			mRenderManager->queueRTTRemove(mCamera.get());
		mCamera = nullptr;
		mDrawable = nullptr;
	}

	void OsgRTTexture::begin()
	{
		mDrawable->clear();
	}

	void OsgRTTexture::end()
	{
	}

	void OsgRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		mDrawable->addBatch(mRenderManager->createBatch(_buffer, _texture, _count, nullptr));
	}

	const RenderTargetInfo& OsgRTTexture::getInfo() const
	{
		return mInfo;
	}

} // namespace MyGUI
