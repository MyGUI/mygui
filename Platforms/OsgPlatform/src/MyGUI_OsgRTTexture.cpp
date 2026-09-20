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
#include "MyGUI_OsgDrawablePool.h"

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
#include <osg/Image>
#include <osg/observer_ptr>
#include <osgUtil/CullVisitor>
#include <cstdint>
#include <map>
#include <unordered_map>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>

namespace MyGUI
{

	// Shared by all retained generations of this target. Completion is recorded
	// by draw, not cull: another context (or a delayed draw) may still need it.
	class RTTCache : public osg::Referenced
	{
	public:
		explicit RTTCache(osg::Texture2D* texture) :
			mTexture(texture)
		{
		}

		bool needsRender(osg::State* state, std::uint64_t revision, const std::vector<Batch>& batches)
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mMutex);
			const unsigned int context = state->getContextID();
			const auto found = mRendered.find(context);
			if (found == mRendered.end() || found->second.state.get() != state || found->second.revision != revision ||
				!mTexture->getTextureObject(context))
				return true;
			const auto& images = found->second.images;
			for (size_t i = 0; i < batches.size(); ++i)
			{
				const Batch& batch = batches[i];
				// Custom uniforms/shaders and GPU-produced textures may change without
				// another MyGUI submission. Retain their original refresh behavior.
				if (batch.mStateSet)
					return true;
				if (const osg::Texture2D* texture = batch.mTexture.get())
				{
					const osg::Image* image = texture->getImage();
					if (texture->getDataVariance() == osg::Object::DYNAMIC || !image ||
						image->getDataVariance() == osg::Object::DYNAMIC ||
						texture->getTextureParameterDirty(context) || !texture->getTextureObject(context) ||
						images[i].first != image || images[i].second != image->getModifiedCount())
						return true;
				}
			}
			return false;
		}

		void rendered(osg::State* state, std::uint64_t revision, const std::vector<Batch>& batches)
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mMutex);
			auto& entry = mRendered[state->getContextID()];
			entry.state = state;
			entry.revision = revision;
			entry.images.clear();
			for (const Batch& batch : batches)
			{
				const osg::Image* image = batch.mTexture ? batch.mTexture->getImage() : nullptr;
				entry.images.emplace_back(image, image ? image->getModifiedCount() : 0);
			}
		}

		void invalidate(osg::State* state)
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mMutex);
			if (state)
				mRendered.erase(state->getContextID());
			else
				mRendered.clear();
		}

	private:
		struct Rendered
		{
			osg::observer_ptr<osg::State> state;
			std::uint64_t revision{};
			std::vector<std::pair<const osg::Image*, unsigned int>> images;
		};
		osg::ref_ptr<osg::Texture2D> mTexture;
		OpenThreads::Mutex mMutex;
		std::map<unsigned int, Rendered> mRendered;
	};

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
			setUseVertexArrayObject(true);
			setCullingActive(false);

			mStateSet = new osg::StateSet;
			// the RTT camera's viewport is not applied by osg before the drawable
			// draws, so it has to be applied as part of the drawable's state
			mStateSet->setAttribute(new osg::Viewport(0, 0, _width, _height));
			applyGuiDrawableStateModes(mStateSet);

			// render the RTT content with the same default shader as the main GUI
			if (_manager != nullptr)
			{
				if (osg::Program* program = _manager->getShaderProgram("Default"))
				{
					mStateSet->setAttributeAndModes(program, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
					mStateSet->addUniform(new osg::Uniform("Texture", 0));
				}
			}

			mDummyTexture = createDummyTexture();
		}

		RTTDrawable(const RTTDrawable& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY) :
			osg::Drawable(copy, copyop),
			mWidth(copy.mWidth),
			mHeight(copy.mHeight),
			mDummyTexture(copy.mDummyTexture),
			mStateSet(copy.mStateSet),
			mCache(copy.mCache)
		{
		}

		osg::VertexArrayState* createVertexArrayStateImplementation(osg::RenderInfo& renderInfo) const override
		{
			osg::VertexArrayState* arrays = osg::Drawable::createVertexArrayStateImplementation(renderInfo);
			arrays->generateVertexArrayObject();
			return arrays;
		}

		void drawImplementation(osg::RenderInfo& renderInfo) const override
		{
			// Keep this immutable drawable until the layer submits a replacement.
			osgDrawBatches(renderInfo.getState(), mStateSet.get(), mBatches, mDummyTexture.get());
			if (mCache)
				mCache->rendered(renderInfo.getState(), mRevision, mBatches);
		}
		bool needsRender(osg::State* state) const
		{
			return !mCache || mCache->needsRender(state, mRevision, mBatches);
		}
		void setRevision(RTTCache* cache, std::uint64_t revision)
		{
			mCache = cache;
			mRevision = revision;
		}
		void releaseGLObjects(osg::State* state = nullptr) const override
		{
			osg::Drawable::releaseGLObjects(state);
			releaseOsgBatches(mBatches, state);
			mStateSet->releaseGLObjects(state);
			mDummyTexture->releaseGLObjects(state);
			if (mCache)
				mCache->invalidate(state);
		}

		void addBatch(const Batch& batch)
		{
			mBatches.push_back(batch);
		}
		const std::vector<Batch>& getBatches() const
		{
			return mBatches;
		}
		void clearBatches()
		{
			mBatches.clear();
		}
		size_t getBatchCapacityBytes() const
		{
			return mBatches.capacity() * sizeof(Batch);
		}

	META_Object(osgMyGUI, RTTDrawable)

		private : std::vector<Batch> mBatches;
		int mWidth;
		int mHeight;

		osg::ref_ptr<osg::Texture2D> mDummyTexture;
		osg::ref_ptr<osg::StateSet> mStateSet;
		osg::ref_ptr<RTTCache> mCache;
		std::uint64_t mRevision{};
	};

	class RTTCamera : public OsgPoolCamera<RTTDrawable>
	{
	public:
		explicit RTTCamera(OsgDrawablePool<RTTDrawable>* pool) :
			OsgPoolCamera<RTTDrawable>(pool)
		{
		}
		void accept(osg::NodeVisitor& visitor) override
		{
			if (auto* cull = dynamic_cast<osgUtil::CullVisitor*>(&visitor))
			{
				if (cull->getState() && getNumChildren() == 1)
				{
					auto* drawable = dynamic_cast<RTTDrawable*>(getChild(0));
					if (drawable && !drawable->needsRender(cull->getState()))
						return;
				}
			}
			// Skip before CullVisitor::apply(Camera), otherwise OSG still creates
			// a render stage, binds the framebuffer and clears its cached pixels.
			osg::Camera::accept(visitor);
		}
	};

	void orderOsgRTTCameras(osg::Group* _root)
	{
		std::unordered_map<const osg::Texture*, RTTCamera*> producers;
		for (unsigned int i = 0; i < _root->getNumChildren(); ++i)
		{
			if (auto* camera = dynamic_cast<RTTCamera*>(_root->getChild(i)))
			{
				const auto& attachments = camera->getBufferAttachmentMap();
				producers.emplace(attachments.at(osg::Camera::COLOR_BUFFER)._texture.get(), camera);
			}
		}

		// Creation and submission order need not match dependencies, and retained
		// passes can sample another RTT without being resubmitted this frame.
		enum class Visit
		{
			Active,
			Complete
		};
		std::unordered_map<RTTCamera*, Visit> visited;
		int order = 0;
		const auto visit = [&](auto&& self, RTTCamera* camera) -> void
		{
			const auto [entry, inserted] = visited.emplace(camera, Visit::Active);
			if (!inserted)
			{
				MYGUI_PLATFORM_ASSERT(entry->second == Visit::Complete, "Cyclic render-to-texture dependency");
				return;
			}
			const auto* drawable = static_cast<const RTTDrawable*>(camera->getChild(0));
			for (const Batch& batch : drawable->getBatches())
			{
				const auto producer = producers.find(batch.mTexture.get());
				if (producer != producers.end())
					self(self, producer->second);
			}
			camera->setRenderOrder(osg::Camera::PRE_RENDER, order++);
			visited[camera] = Visit::Complete;
		};
		for (const auto& [texture, camera] : producers)
			visit(visit, camera);
	}

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

		mDrawablePool = new OsgDrawablePool<RTTDrawable>;
		mCache = new RTTCache(_texture);
		mCamera = new RTTCamera(mDrawablePool);
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
		mDrawable->setRevision(mCache, ++mRevision);
		mCamera->addChild(mDrawable.get());

		// Attach during update, before any camera starts culling.
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
		osg::ref_ptr<RTTDrawable> next = mDrawablePool->acquire(*mDrawable);
		next->setRevision(mCache, ++mRevision);
		mCamera->replaceChild(mDrawable, next);
		mDrawablePool->retired.push_back(mDrawable);
		mDrawable = next;
	}

	void OsgRTTexture::end()
	{
	}

	void OsgRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		if (_count == 0)
			return;
		mDrawable->addBatch(mRenderManager->createBatch(_buffer, _texture, _count, nullptr));
	}

	const RenderTargetInfo& OsgRTTexture::getInfo() const
	{
		return mInfo;
	}

} // namespace MyGUI
