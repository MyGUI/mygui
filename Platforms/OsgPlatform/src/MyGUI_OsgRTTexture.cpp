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
#include <osg/BlendFunc>
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
			RTTDrawable(256, 256)
		{
		}

		RTTDrawable(int _width, int _height) :
			mWidth(_width),
			mHeight(_height)
		{
			setSupportsDisplayList(false);
			setCullingActive(false);

			mStateSet = new osg::StateSet;
			// the RTT camera's viewport is not applied by osg before the drawable
			// draws, so it has to be applied as part of the drawable's state
			mStateSet->setAttribute(new osg::Viewport(0, 0, _width, _height));
			// the RGBA8 RTT texture must be rendered with the same blend function
			// as the other backends (SRC_ALPHA for the colour channels but ONE for
			// the alpha channel); otherwise the alpha of semi-transparent elements
			// is multiplied twice and the sampled result is too dark
			mStateSet->setAttributeAndModes(
				new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
			mStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
			mStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

			// render the RTT content with the same default shader as the main GUI
			if (osg::Program* program = OsgRenderManager::getInstance().getShaderProgram("Default"))
			{
				mStateSet->setAttributeAndModes(program, osg::StateAttribute::ON);
				mStateSet->addUniform(new osg::Uniform("Texture", 0));
			}

			mDummyTexture = new osg::Texture2D;
			mDummyTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
			mDummyTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
			mDummyTexture->setInternalFormat(GL_RGB);
			mDummyTexture->setTextureSize(1, 1);
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

	void osgDrawBatches(
		osg::State* state,
		osg::StateSet* stateSet,
		const std::vector<Batch>& batches,
		osg::Texture2D* dummyTexture)
	{
		state->pushStateSet(stateSet);
		state->apply();

		// the vertex data is fed through generic vertex attributes 0 (position),
		// 3 (colour) and 8 (texcoord 0) instead of the fixed-function client
		// arrays, so the GUI is rendered by a shader on any OpenGL profile. The
		// shader is expected to use the osg_ModelViewProjectionMatrix uniform,
		// which is kept in sync here like osgText does.
		state->setUseModelViewAndProjectionUniforms(true);
		state->applyModelViewAndProjectionUniformsIfRequired();

		osg::GLExtensions* extensions = state->get<osg::GLExtensions>();

		state->disableAllVertexArrays();
		state->disableVertexAttribPointer(0);
		state->disableVertexAttribPointer(3);
		state->disableVertexAttribPointer(8);

		for (const Batch& batch : batches)
		{
			osg::VertexBufferObject* vbo = batch.mVertexBuffer;

			if (batch.mStateSet)
			{
				state->pushStateSet(batch.mStateSet);
				state->apply();
				state->applyModelViewAndProjectionUniformsIfRequired();
			}

			// A GUI element without an associated texture would be extremely rare.
			// It is worth it to use a dummy 1x1 black texture sampler instead of either adding a conditional or
			// relinking shaders.
			osg::Texture2D* texture = batch.mTexture;
			if (texture)
				state->applyTextureAttribute(0, texture);
			else
				state->applyTextureAttribute(0, dummyTexture);

			osg::GLBufferObject* bufferobject = state->isVertexBufferObjectSupported()
				? vbo->getOrCreateGLBufferObject(state->getContextID())
				: nullptr;
			if (bufferobject)
			{
				state->bindVertexBufferObject(bufferobject);

				extensions->glEnableVertexAttribArray(0);
				extensions->glEnableVertexAttribArray(3);
				extensions->glEnableVertexAttribArray(8);

				extensions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<char*>(0));
				extensions->glVertexAttribPointer(
					3,
					4,
					GL_UNSIGNED_BYTE,
					GL_TRUE,
					sizeof(Vertex),
					reinterpret_cast<char*>(12));
				extensions
					->glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<char*>(16));
			}
			else
			{
				const char* data = static_cast<const char*>(vbo->getArray(0)->getDataPointer());

				extensions->glEnableVertexAttribArray(0);
				extensions->glEnableVertexAttribArray(3);
				extensions->glEnableVertexAttribArray(8);

				extensions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), data);
				extensions->glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), data + 12);
				extensions->glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), data + 16);
			}

			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(batch.mVertexCount));

			if (batch.mStateSet)
			{
				state->popStateSet();
				state->apply();
			}
		}

		state->disableVertexAttribPointer(0);
		state->disableVertexAttribPointer(3);
		state->disableVertexAttribPointer(8);

		state->popStateSet();

		state->unbindVertexBufferObject();
		state->dirtyAllVertexArrays();
		state->disableAllVertexArrays();
	}

	OsgRTTexture::OsgRTTexture(osg::Texture2D* _texture, OsgRenderManager* _manager, int _width, int _height)
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

		mDrawable = new RTTDrawable(_width, _height);
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
		if (mCamera.valid() && mRenderManager != nullptr)
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
		Batch batch;
		batch.mVertexCount = _count;
		batch.mVertexBuffer = static_cast<OsgVertexBuffer*>(_buffer)->getVertexBuffer();
		batch.mArray = static_cast<OsgVertexBuffer*>(_buffer)->getVertexArray();
		static_cast<OsgVertexBuffer*>(_buffer)->markUsed();

		if (OsgTexture* osgtexture = static_cast<OsgTexture*>(_texture))
		{
			batch.mTexture = osgtexture->getTexture();
			if (osg::StateSet* shaderState = osgtexture->getShaderStateSet())
				batch.mStateSet = shaderState;
			else if (osgtexture->getInjectState())
				batch.mStateSet = osgtexture->getInjectState();
		}

		mDrawable->addBatch(batch);
	}

	const RenderTargetInfo& OsgRTTexture::getInfo() const
	{
		return mInfo;
	}

} // namespace MyGUI
