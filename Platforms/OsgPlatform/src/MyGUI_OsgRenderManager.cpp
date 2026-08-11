/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#include "MyGUI_OsgRenderManager.h"
#include "MyGUI_OsgDataManager.h"
#include "MyGUI_OsgDiagnostic.h"
#include "MyGUI_OsgRTTexture.h"
#include "MyGUI_OsgTexture.h"
#include "MyGUI_OsgVertexBuffer.h"
#include "MyGUI_Timer.h"

#include <osg/Camera>
#include <osg/Drawable>
#include <osg/GL>
#include <osg/BufferObject>
#include <osg/Matrix>
#include <osg/State>
#include <osg/StateSet>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#include <algorithm>
#include <cstddef>
#include <vector>

namespace MyGUI
{

	OsgRenderManager::OsgRenderManager() = default;

	OsgRenderManager::~OsgRenderManager() = default;

	// Stage 1: collect draw calls. Run during the Cull traversal.
	class CollectDrawCalls : public osg::NodeCallback
	{
	public:
		CollectDrawCalls() :
			mRenderManager(nullptr)
		{
		}

		void setRenderManager(OsgRenderManager* renderManager)
		{
			mRenderManager = renderManager;
		}

		void operator()(osg::Node* node, osg::NodeVisitor* nv) override
		{
			mRenderManager->collectDrawCalls();
			// the camera's children are not traversed automatically when a cull callback
			// is present, so the traversal has to be continued here
			traverse(node, nv);
		}

	private:
		OsgRenderManager* mRenderManager;
	};

	class Drawable : public osg::Drawable
	{
	public:
		// Stage 0: update widget animations and controllers. Run during the Update traversal.
		class FrameUpdate : public osg::NodeCallback
		{
		public:
			FrameUpdate() :
				mRenderManager(nullptr)
			{
			}

			void setRenderManager(OsgRenderManager* renderManager)
			{
				mRenderManager = renderManager;
			}

			void operator()(osg::Node* node, osg::NodeVisitor* nv) override
			{
				mRenderManager->update();
			}

		private:
			OsgRenderManager* mRenderManager;
		};

		// Stage 2: execute the draw calls. Run during the Draw traversal. May run in parallel with the update traversal
		// of the next frame.
		void drawImplementation(osg::RenderInfo& renderInfo) const override
		{
			mReadFrom = (mReadFrom + 1) % sNumBuffers;
			const std::vector<Batch>& vec = mBatchVector[mReadFrom];
			osgDrawBatches(renderInfo.getState(), mStateSet.get(), vec, mDummyTexture.get());
		}

	public:
		Drawable(OsgRenderManager* parent = nullptr) :
			mWriteTo(0),
			mReadFrom(0),
			mParent(parent)
		{
			setSupportsDisplayList(false);

			osg::ref_ptr<FrameUpdate> frameUpdate = new FrameUpdate;
			frameUpdate->setRenderManager(mParent);
			setUpdateCallback(frameUpdate);

			mStateSet = new osg::StateSet;
			mStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
			mStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
			// fixed-function lighting would tint the vertex colours, so it must be off
			mStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
			// the fixed-function pipeline needs this enabled to sample the textures
			mStateSet->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::ON);

			mDummyTexture = new osg::Texture2D;
			mDummyTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
			mDummyTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
			mDummyTexture->setInternalFormat(GL_RGB);
			mDummyTexture->setTextureSize(1, 1);
		}

		Drawable(const Drawable& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY) :
			osg::Drawable(copy, copyop),
			mWriteTo(0),
			mReadFrom(0),
			mDummyTexture(copy.mDummyTexture),
			mParent(copy.mParent),
			mStateSet(copy.mStateSet)
		{
		}

		// Defines the necessary information for a draw call
		void addBatch(const Batch& batch)
		{
			mBatchVector[mWriteTo].push_back(batch);
		}

		void clear()
		{
			mWriteTo = (mWriteTo + 1) % sNumBuffers;
			mBatchVector[mWriteTo].clear();
		}

		osg::StateSet* getDrawableStateSet()
		{
			return mStateSet;
		}

	META_Object(osgMyGUI, Drawable)

		private :
		// 2 would be enough in most cases, use 4 to get stereo working
		static const int sNumBuffers = 4;

		// double buffering approach, to avoid the need for synchronization with the draw thread
		std::vector<Batch> mBatchVector[sNumBuffers];

		int mWriteTo;
		mutable int mReadFrom;

		osg::ref_ptr<osg::Texture2D> mDummyTexture;

		OsgRenderManager* mParent;
		osg::ref_ptr<osg::StateSet> mStateSet;
	};

	void OsgRenderManager::initialise(osg::Group* _sceneRoot, int _width, int _height)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mVertexFormat = VertexColourType::ColourABGR;

		mUpdate = false;
		mSceneRoot = _sceneRoot;

		mDrawable = new Drawable(this);

		osg::ref_ptr<osg::Camera> camera = new osg::Camera();
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setProjectionResizePolicy(osg::Camera::FIXED);
		camera->setProjectionMatrix(osg::Matrix::identity());
		camera->setViewMatrix(osg::Matrix::identity());
		camera->setRenderOrder(osg::Camera::POST_RENDER);
		camera->setClearMask(GL_NONE);
		mDrawable->setCullingActive(false);
		camera->addChild(mDrawable.get());

		// the collect draw calls callback runs before the GUI camera's children are traversed,
		// so the deferred RTT camera operations (flushRTTCameras) can safely modify the children
		osg::ref_ptr<CollectDrawCalls> collectDrawCalls = new CollectDrawCalls;
		collectDrawCalls->setRenderManager(this);
		camera->setCullCallback(collectDrawCalls.get());

		mGuiRoot = camera;
		mSceneRoot->addChild(mGuiRoot.get());

		setViewSize(_width, _height);

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OsgRenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		if (mGuiRoot.valid())
			mSceneRoot->removeChild(mGuiRoot.get());
		mGuiRoot = nullptr;
		mDrawable = nullptr;
		mSceneRoot = nullptr;

		for (const auto& item : mTextures)
			delete item.second;
		mTextures.clear();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	OsgRenderManager& OsgRenderManager::getInstance()
	{
		return *getInstancePtr();
	}

	OsgRenderManager* OsgRenderManager::getInstancePtr()
	{
		return static_cast<OsgRenderManager*>(RenderManager::getInstancePtr());
	}

	const IntSize& OsgRenderManager::getViewSize() const
	{
		return mViewSize;
	}

	osg::Camera* OsgRenderManager::getGuiRoot() const
	{
		return mGuiRoot.get();
	}

	VertexColourType OsgRenderManager::getVertexFormat() const
	{
		return mVertexFormat;
	}

	bool OsgRenderManager::isFormatSupported(PixelFormat /*_format*/, TextureUsage /*_usage*/)
	{
		return true;
	}

	IVertexBuffer* OsgRenderManager::createVertexBuffer()
	{
		return new OsgVertexBuffer();
	}

	void OsgRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	ITexture* OsgRenderManager::createTexture(const std::string& _name)
	{
		OsgTexture* tex = new OsgTexture(_name, this);
		mTextures[_name] = tex;
		return tex;
	}

	void OsgRenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr)
			return;

		const auto item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		delete item->second;
		mTextures.erase(item);
	}

	ITexture* OsgRenderManager::getTexture(const std::string& _name)
	{
		if (_name.empty())
			return nullptr;

		const auto item = mTextures.find(_name);
		if (item == mTextures.end())
		{
			ITexture* tex = createTexture(_name);
			tex->loadFromFile(_name);
			return tex;
		}
		return item->second;
	}

	void OsgRenderManager::begin()
	{
		mDrawable->clear();
		// variance will be recomputed based on textures being rendered in this frame
		mDrawable->setDataVariance(osg::Object::STATIC);
	}

	void OsgRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		Batch batch;
		batch.mVertexCount = _count;
		batch.mVertexBuffer = static_cast<OsgVertexBuffer*>(_buffer)->getVertexBuffer();
		batch.mArray = static_cast<OsgVertexBuffer*>(_buffer)->getVertexArray();
		static_cast<OsgVertexBuffer*>(_buffer)->markUsed();

		if (OsgTexture* osgtexture = static_cast<OsgTexture*>(_texture))
		{
			batch.mTexture = osgtexture->getTexture();
			if (batch.mTexture.valid() && batch.mTexture->getDataVariance() == osg::Object::DYNAMIC)
				mDrawable->setDataVariance(osg::Object::DYNAMIC); // only for this frame, reset in begin()
			if (!mInjectState && osgtexture->getInjectState())
				batch.mStateSet = osgtexture->getInjectState();
		}
		if (mInjectState)
			batch.mStateSet = mInjectState;

		mDrawable->addBatch(batch);
	}

	void OsgRenderManager::end()
	{
	}

	const RenderTargetInfo& OsgRenderManager::getInfo() const
	{
		return mInfo;
	}

	void OsgRenderManager::setViewSize(int _width, int _height)
	{
		if (_width < 1)
			_width = 1;
		if (_height < 1)
			_height = 1;

		mGuiRoot->setViewport(0, 0, _width, _height);

		mViewSize.set(_width, _height);

		mInfo.maximumDepth = 1;
		mInfo.hOffset = 0;
		mInfo.vOffset = 0;
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0f / float(mViewSize.width);
		mInfo.pixScaleY = 1.0f / float(mViewSize.height);

		onResizeView(mViewSize);
		mUpdate = true;
	}

	void OsgRenderManager::registerShader(
		const std::string& /*_shaderName*/,
		const std::string& /*_vertexProgramFile*/,
		const std::string& /*_fragmentProgramFile*/)
	{
		MYGUI_PLATFORM_LOG(Warning, "OsgRenderManager::registerShader is not implemented");
	}

	void OsgRenderManager::update()
	{
		static MyGUI::Timer timer;
		static unsigned long lastLime = timer.getMilliseconds();
		unsigned long nowTime = timer.getMilliseconds();
		unsigned long time = nowTime - lastLime;

		std::cerr << "OSG update()" << std::endl;
		onFrameEvent(static_cast<float>(static_cast<double>(time) / 1000));

		lastLime = nowTime;
	}

	void OsgRenderManager::collectDrawCalls()
	{
		// apply the queued RTT camera operations before the GUI camera's children are
		// traversed, so the children list is not modified while it is being iterated
		flushRTTCameras();

		begin();
		onRenderToTarget(this, mUpdate);
		end();

		mUpdate = false;
	}

	void OsgRenderManager::queueRTTAdd(osg::Camera* _camera)
	{
		mPendingRTTAdd.emplace_back(_camera);
	}

	void OsgRenderManager::queueRTTRemove(osg::Camera* _camera)
	{
		mPendingRTTRemove.push_back(_camera);
	}

	void OsgRenderManager::flushRTTCameras()
	{
		for (osg::Camera* camera : mPendingRTTRemove)
		{
			// if the camera is still waiting to be added, just cancel the pending add
			auto it = std::find_if(
				mPendingRTTAdd.begin(),
				mPendingRTTAdd.end(),
				[camera](const osg::ref_ptr<osg::Camera>& entry) { return entry.get() == camera; });
			if (it != mPendingRTTAdd.end())
			{
				// dropping the queued reference may destroy the camera, so do not touch it afterwards
				mPendingRTTAdd.erase(it);
				continue;
			}

			if (camera->getNumParents() != 0)
				camera->getParent(0)->removeChild(camera);
		}
		mPendingRTTRemove.clear();

		for (const osg::ref_ptr<osg::Camera>& camera : mPendingRTTAdd)
		{
			if (camera.valid() && camera->getNumParents() == 0)
				mGuiRoot->insertChild(0, camera.get());
		}
		mPendingRTTAdd.clear();
	}

	void OsgRenderManager::setImageLoader(OsgImageLoader _loader)
	{
		mImageLoader = std::move(_loader);
	}

	void OsgRenderManager::setInjectState(osg::StateSet* _stateSet)
	{
		mInjectState = _stateSet;
	}

	void OsgRenderManager::setGuiStateSet(osg::StateSet* _stateSet)
	{
		if (_stateSet != nullptr)
			mDrawable->getDrawableStateSet()->merge(*_stateSet);
	}

	osg::ref_ptr<osg::Image> OsgRenderManager::loadImage(const std::string& _fileName) const
	{
		if (mImageLoader)
			return mImageLoader(_fileName);

		std::string filePath = DataManager::getInstance().getDataPath(_fileName);
		if (filePath.empty())
			return nullptr;

		osg::ref_ptr<osg::Image> image = osgDB::readImageFile(filePath);
		if (!image.valid())
			return nullptr;

		// osg loads image data bottom-up (row 0 is the image bottom), but MyGUI
		// expects top-down data (row 0 is the image top) for its texture coordinates.
		image->flipVertical();
		return image;
	}

} // namespace MyGUI
