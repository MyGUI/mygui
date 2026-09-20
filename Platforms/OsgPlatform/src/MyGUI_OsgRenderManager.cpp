/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#include "MyGUI_OsgRenderManager.h"
#include "MyGUI_OsgDataManager.h"
#include "MyGUI_OsgDiagnostic.h"
#include "MyGUI_OsgTexture.h"
#include "MyGUI_OsgRTTexture.h"
#include "MyGUI_OsgVertexBuffer.h"
#include "MyGUI_OsgDrawablePool.h"
#include "MyGUI_Timer.h"
#include "MyGUI_DataStreamHolder.h"

#include <osg/Array>
#include <osg/BlendFunc>
#include <osg/Camera>
#include <osg/Drawable>
#include <osg/GL>
#include <osg/GLExtensions>
#include <osg/BufferObject>
#include <osg/Matrix>
#include <osg/Program>
#include <osg/PolygonMode>
#include <osg/Depth>
#include <osg/Image>
#include <osg/Shader>
#include <osg/State>
#include <osg/StateSet>
#include <osg/Texture2D>
#include <osg/Uniform>
#include <osgDB/ReadFile>

#include <algorithm>
#include <cstddef>
#include <vector>

namespace MyGUI
{

	void applyGuiDrawableStateModes(osg::StateSet* _stateSet)
	{
		_stateSet->setAttributeAndModes(
			new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA),
			osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		_stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
		_stateSet->setMode(GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		_stateSet->setMode(GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
		_stateSet->setAttributeAndModes(
			new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL),
			osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		_stateSet->setAttributeAndModes(
			new osg::Depth(osg::Depth::ALWAYS, 0.0, 1.0, false),
			osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	}

	osg::ref_ptr<osg::Texture2D> createDummyTexture()
	{
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
		texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		osg::ref_ptr<osg::Image> image = new osg::Image;
		image->allocateImage(1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, 1);
		std::fill_n(image->data(), 4, 255);
		texture->setImage(image);
		texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST);
		texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
		return texture;
	}

	void osgDrawBatches(
		osg::State* state,
		osg::StateSet* stateSet,
		const std::vector<Batch>& batches,
		osg::Texture2D* dummyTexture)
	{
		state->pushStateSet(stateSet);
		state->apply();

		// The vertex data is fed through generic attributes instead of fixed-function client
		// arrays, so the GUI is rendered by a shader on any OpenGL profile. The
		// shader is expected to use the osg_ModelViewProjectionMatrix uniform,
		// which is kept in sync here like osgText does.
		state->setUseModelViewAndProjectionUniforms(true);
		state->applyModelViewAndProjectionUniformsIfRequired();

		osg::GLExtensions* extensions = state->get<osg::GLExtensions>();

		// GL3 OSG builds override program bindings with their compact attribute aliases.
		const bool aliases = state->getUseVertexAttributeAliasing();
		const unsigned int position = aliases ? state->getVertexAlias()._location : 0;
		const unsigned int colour = aliases ? state->getColorAlias()._location : 3;
		const unsigned int texcoord = aliases ? state->getTexCoordAliasList()[0]._location : 8;

		state->disableAllVertexArrays();
		state->disableVertexAttribPointer(position);
		state->disableVertexAttribPointer(colour);
		state->disableVertexAttribPointer(texcoord);

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
			// It is worth it to use a dummy 1x1 white texture sampler instead of either adding a conditional or
			// relinking shaders.
			osg::Texture2D* texture = batch.mTexture;
			if (texture)
				state->applyTextureAttribute(0, texture);
			else
				state->applyTextureAttribute(0, dummyTexture);

			if (state->isVertexBufferObjectSupported())
			{
				bindOsgVertexBuffer(state, vbo);

				extensions->glEnableVertexAttribArray(position);
				extensions->glEnableVertexAttribArray(colour);
				extensions->glEnableVertexAttribArray(texcoord);

				extensions->glVertexAttribPointer(
					position,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(Vertex),
					reinterpret_cast<char*>(0));
				extensions->glVertexAttribPointer(
					colour,
					4,
					GL_UNSIGNED_BYTE,
					GL_TRUE,
					sizeof(Vertex),
					reinterpret_cast<void*>(offsetof(Vertex, colour)));
				extensions->glVertexAttribPointer(
					texcoord,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(Vertex),
					reinterpret_cast<void*>(offsetof(Vertex, u)));
			}
			else
			{
				state->unbindVertexBufferObject();
				const char* data = static_cast<const char*>(vbo->getArray(0)->getDataPointer());

				extensions->glEnableVertexAttribArray(position);
				extensions->glEnableVertexAttribArray(colour);
				extensions->glEnableVertexAttribArray(texcoord);

				extensions->glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), data);
				extensions->glVertexAttribPointer(
					colour,
					4,
					GL_UNSIGNED_BYTE,
					GL_TRUE,
					sizeof(Vertex),
					data + offsetof(Vertex, colour));
				extensions->glVertexAttribPointer(
					texcoord,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(Vertex),
					data + offsetof(Vertex, u));
			}

			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(batch.mVertexCount));

			if (batch.mStateSet)
			{
				state->popStateSet();
				state->apply();
			}
		}

		extensions->glDisableVertexAttribArray(position);
		extensions->glDisableVertexAttribArray(colour);
		extensions->glDisableVertexAttribArray(texcoord);

		state->popStateSet();
		state->apply();

		state->unbindVertexBufferObject();
		state->dirtyAllVertexArrays();
		state->disableAllVertexArrays();
	}

	void releaseOsgBatches(const std::vector<Batch>& _batches, osg::State* _state)
	{
		for (const Batch& batch : _batches)
		{
			batch.mVertexBuffer->releaseGLObjects(_state);
			if (batch.mTexture)
				batch.mTexture->releaseGLObjects(_state);
			if (batch.mStateSet)
				batch.mStateSet->releaseGLObjects(_state);
		}
	}

	OsgRenderManager::OsgRenderManager() = default;

	OsgRenderManager::~OsgRenderManager() = default;

	// All MyGUI mutation runs during update, before any camera's cull traversal.
	class FrameUpdate : public osg::NodeCallback
	{
	public:
		explicit FrameUpdate(OsgRenderManager* manager) :
			mRenderManager(manager)
		{
		}
		void operator()(osg::Node* node, osg::NodeVisitor* visitor) override
		{
			mRenderManager->update();
			mRenderManager->collectDrawCalls();
			traverse(node, visitor);
		}

	private:
		OsgRenderManager* mRenderManager;
	};

	// Batches stay immutable while any camera/context retains the drawable.
	// Once detached and no longer referenced by render leaves, its storage is reusable.
	class Drawable : public osg::Drawable
	{
	public:
		Drawable()
		{
			setSupportsDisplayList(false);
			setUseVertexArrayObject(true);
			setCullingActive(false);
			mStateSet = new osg::StateSet;
			applyGuiDrawableStateModes(mStateSet);
			mDummyTexture = createDummyTexture();
		}
		Drawable(const Drawable& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY) :
			osg::Drawable(copy, copyop),
			mDummyTexture(copy.mDummyTexture),
			mStateSet(copy.mStateSet)
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
			osgDrawBatches(renderInfo.getState(), mStateSet, mBatches, mDummyTexture);
		}
		void releaseGLObjects(osg::State* state = nullptr) const override
		{
			osg::Drawable::releaseGLObjects(state);
			releaseOsgBatches(mBatches, state);
			mStateSet->releaseGLObjects(state);
			mDummyTexture->releaseGLObjects(state);
		}
		void addBatch(const Batch& batch)
		{
			mBatches.push_back(batch);
		}
		void clearBatches()
		{
			mBatches.clear();
		}
		size_t getBatchCapacityBytes() const
		{
			return mBatches.capacity() * sizeof(Batch);
		}
		void setDrawableStateSet(osg::StateSet* state)
		{
			mStateSet = state;
		}
		osg::StateSet* getDrawableStateSet()
		{
			return mStateSet;
		}
	META_Object(osgMyGUI, Drawable) private : std::vector<Batch> mBatches;
		osg::ref_ptr<osg::Texture2D> mDummyTexture;
		osg::ref_ptr<osg::StateSet> mStateSet;
	};

	void OsgRenderManager::initialise(osg::Group* _sceneRoot, int _width, int _height)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mVertexFormat = VertexColourType::ColourABGR;

		mUpdate = false;
		mSceneRoot = _sceneRoot;

		mDrawable = new Drawable;

		// the GUI is rendered by a shader on all OpenGL profiles, so a default
		// program is registered and applied to the drawable's state. Textures
		// with a custom shader override it for their batches via setShader.
		registerShader("Default", "MyGUI_Osg_VP.glsl", "MyGUI_Osg_FP.glsl");
		if (osg::Program* defaultProgram = getShaderProgram("Default"))
		{
			osg::StateSet* drawableState = mDrawable->getDrawableStateSet();
			drawableState->setAttributeAndModes(
				defaultProgram,
				osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
			// bind the 'Texture' sampler to texture unit 0, matching the texture
			// bound for each batch in osgDrawBatches
			drawableState->addUniform(new osg::Uniform("Texture", 0));
		}

		mDrawablePool = new OsgDrawablePool<Drawable>;
		osg::ref_ptr<osg::Camera> camera = new OsgPoolCamera<Drawable>(mDrawablePool);
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setProjectionResizePolicy(osg::Camera::FIXED);
		camera->setProjectionMatrix(osg::Matrix::identity());
		camera->setViewMatrix(osg::Matrix::identity());
		camera->setRenderOrder(osg::Camera::POST_RENDER);
		camera->setClearMask(GL_NONE);
		mDrawable->setCullingActive(false);
		camera->addChild(mDrawable.get());

		camera->setUpdateCallback(new FrameUpdate(this));

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
		mDrawablePool = nullptr;
		mSceneRoot = nullptr;

		for (const auto& item : mTextures)
			delete item.second;
		mTextures.clear();

		// the destroyed textures have queued their RTT cameras for removal, so drop the
		// pending operations to avoid dangling camera pointers in a future flushRTTCameras
		mPendingRTTAdd.clear();
		mPendingRTTRemove.clear();
		mRegisteredShaders.clear();
		mInjectState = nullptr;
		mGuiStateSet = nullptr;

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

	bool OsgRenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		if (_usage.isValue(TextureUsage::RenderTarget))
			return !_usage.isValue(TextureUsage::Read) &&
				(_format == PixelFormat::R8G8B8 || _format == PixelFormat::R8G8B8A8);
		return _format.getBytesPerPixel() != 0;
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
		MYGUI_PLATFORM_ASSERT(mTextures.find(_name) == mTextures.end(), "Texture '" << _name << "' already exists");
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
			return nullptr;
		return item->second;
	}

	void OsgRenderManager::begin()
	{
		osg::ref_ptr<Drawable> next = mDrawablePool->acquire(*mDrawable);
		next->setDrawableStateSet(mGuiStateSet.valid() ? mGuiStateSet.get() : mDrawable->getDrawableStateSet());
		mGuiRoot->replaceChild(mDrawable, next);
		mDrawablePool->retired.push_back(mDrawable);
		mDrawable = next;
		// variance will be recomputed based on textures being rendered in this frame
		mDrawable->setDataVariance(osg::Object::STATIC);
	}

	Batch OsgRenderManager::createBatch(
		IVertexBuffer* _buffer,
		ITexture* _texture,
		size_t _count,
		osg::StateSet* _injectState) const
	{
		MYGUI_PLATFORM_ASSERT(_buffer != nullptr, "Vertex buffer is null");
		MYGUI_PLATFORM_ASSERT(_count <= _buffer->getVertexCount(), "Vertex count exceeds buffer size");
		Batch batch;
		batch.mVertexCount = _count;
		batch.mVertexBuffer = static_cast<OsgVertexBuffer*>(_buffer)->getVertexBuffer();
		batch.mArray = static_cast<OsgVertexBuffer*>(_buffer)->getVertexArray();
		MYGUI_PLATFORM_ASSERT(batch.mVertexBuffer.valid() && batch.mArray.valid(), "Vertex buffer is not created");

		if (OsgTexture* osgtexture = static_cast<OsgTexture*>(_texture))
		{
			batch.mTexture = osgtexture->getTexture();
			MYGUI_PLATFORM_ASSERT(batch.mTexture.valid(), "Texture is not created");
			if (osg::StateSet* shaderState = osgtexture->getShaderStateSet())
				batch.mStateSet = shaderState;
			else if (osgtexture->getInjectState())
				batch.mStateSet = osgtexture->getInjectState();
		}
		if (_injectState)
			batch.mStateSet = _injectState;

		return batch;
	}

	void OsgRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		if (_count == 0)
			return;
		Batch batch = createBatch(_buffer, _texture, _count, mInjectState);
		if (batch.mTexture.valid() && batch.mTexture->getDataVariance() == osg::Object::DYNAMIC)
			mDrawable->setDataVariance(osg::Object::DYNAMIC); // only for this frame, reset in begin()
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

	std::string OsgRenderManager::loadFileContent(const std::string& _file)
	{
		auto stream = DataManager::getInstance().getDataHolder(_file);
		if (!stream)
		{
			MYGUI_PLATFORM_LOG(Error, "Failed to load file content '" << _file << "'.");
			return {};
		}
		return stream->readAllText();
	}

	osg::ref_ptr<osg::Program> OsgRenderManager::createShaderProgram(
		const std::string& _vertexProgramFile,
		const std::string& _fragmentProgramFile)
	{
		std::string vertexSource = loadFileContent(_vertexProgramFile);
		if (vertexSource.empty())
			MYGUI_PLATFORM_EXCEPT("Failed to load vertex program file '" << _vertexProgramFile << "'");

		std::string fragmentSource = loadFileContent(_fragmentProgramFile);
		if (fragmentSource.empty())
			MYGUI_PLATFORM_EXCEPT("Failed to load fragment program file '" << _fragmentProgramFile << "'");

		osg::ref_ptr<osg::Program> program = new osg::Program;

		osg::ref_ptr<osg::Shader> vertexShader = new osg::Shader(osg::Shader::VERTEX);
		vertexShader->setShaderSource(vertexSource);
		program->addShader(vertexShader.get());

		osg::ref_ptr<osg::Shader> fragmentShader = new osg::Shader(osg::Shader::FRAGMENT);
		fragmentShader->setShaderSource(fragmentSource);
		program->addShader(fragmentShader.get());

		// The vertex data is set up with generic vertex attributes in osgDrawBatches.
		// The attributes are fed at the conventional fixed-function locations
		// 0 (position), 3 (colour) and 8 (texcoord 0), so bind the shader's vertex
		// attributes to the same locations.
		program->addBindAttribLocation("osg_Vertex", 0);
		program->addBindAttribLocation("osg_Color", 3);
		program->addBindAttribLocation("osg_MultiTexCoord0", 8);

		return program;
	}

	void OsgRenderManager::registerShader(
		const std::string& _shaderName,
		const std::string& _vertexProgramFile,
		const std::string& _fragmentProgramFile)
	{
		mRegisteredShaders[_shaderName] = createShaderProgram(_vertexProgramFile, _fragmentProgramFile);
	}

	osg::Program* OsgRenderManager::getShaderProgram(const std::string& _shaderName) const
	{
		const auto iter = mRegisteredShaders.find(_shaderName);
		if (iter != mRegisteredShaders.end())
			return iter->second.get();
		MYGUI_PLATFORM_LOG(
			Error,
			"Failed to get program for shader '" << _shaderName << "'. Did you forgot to register shader?");
		return nullptr;
	}

	void OsgRenderManager::update()
	{
		static MyGUI::Timer timer;
		static unsigned long lastLime = timer.getMilliseconds();
		unsigned long nowTime = timer.getMilliseconds();
		unsigned long time = nowTime - lastLime;

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
		flushRTTCameras();
		orderOsgRTTCameras(mGuiRoot);
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
		for (const osg::ref_ptr<osg::Camera>& camera : mPendingRTTRemove)
		{
			// if the camera is still waiting to be added, just cancel the pending add
			auto it = std::find_if(
				mPendingRTTAdd.begin(),
				mPendingRTTAdd.end(),
				[camera](const osg::ref_ptr<osg::Camera>& entry) { return entry.get() == camera.get(); });
			if (it != mPendingRTTAdd.end())
			{
				// The removal list retains ownership until this flush completes.
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
		{
			// State is shared by immutable drawables; copy only when it changes.
			mGuiStateSet = new osg::StateSet(mGuiStateSet.valid() ? *mGuiStateSet : *mDrawable->getDrawableStateSet());
			mGuiStateSet->merge(*_stateSet);
		}
	}

	osg::ref_ptr<osg::Image> OsgRenderManager::loadImage(const std::string& _fileName) const
	{
		if (mImageLoader)
			return mImageLoader(_fileName);

		std::string filePath = DataManager::getInstance().getDataPath(_fileName);
		if (filePath.empty())
			return nullptr;

		osg::ref_ptr<osg::Image> image = osgDB::readImageFile(filePath);
		// osg loads image data bottom-up (row 0 is the image bottom), but MyGUI
		// expects top-down data (row 0 is the image top) for its texture coordinates.
		if (image.valid())
		{
			if (image->getOrigin() == osg::Image::BOTTOM_LEFT)
				image->flipVertical();
			image->setOrigin(osg::Image::TOP_LEFT);
		}
		return image;
	}

} // namespace MyGUI
