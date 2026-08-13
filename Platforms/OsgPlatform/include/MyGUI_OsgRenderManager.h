/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#ifndef MYGUI_OSG_RENDER_MANAGER_H_
#define MYGUI_OSG_RENDER_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

#include <cstddef>
#include <functional>
#include <map>
#include <vector>
#include <osg/ref_ptr>

namespace osg
{
	class Array;
	class Camera;
	class Group;
	class Image;
	class Program;
	class State;
	class StateSet;
	class Texture2D;
	class VertexBufferObject;
}

namespace MyGUI
{

	class OsgTexture;

	class Drawable;

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

	/*internal:*/
	// Applies the state modes shared by the main GUI drawable and the RTT drawable:
	// depth testing off, blending on and the blend function.
	void applyGuiDrawableStateModes(osg::StateSet* _stateSet);
	// Creates the 1x1 dummy texture sampler used for batches without an associated texture.
	osg::ref_ptr<osg::Texture2D> createDummyTexture();

	/** Image loader function type, used to load texture images by file name.
		By default images are loaded from the file system via osgDB using the resource
		locations registered with the data manager. */
	using OsgImageLoader = std::function<osg::ref_ptr<osg::Image>(const std::string& /*fileName*/)>;

	class OsgRenderManager : public RenderManager, public IRenderTarget
	{
	public:
		OsgRenderManager();
		~OsgRenderManager() override;

		void initialise(osg::Group* _sceneRoot, int _width, int _height);
		void shutdown();

		static OsgRenderManager& getInstance();
		static OsgRenderManager* getInstancePtr();

		/** @see RenderManager::getViewSize */
		const IntSize& getViewSize() const override;

		/** @see RenderManager::getVertexFormat */
		VertexColourType getVertexFormat() const override;

		/** @see RenderManager::isFormatSupported */
		bool isFormatSupported(PixelFormat _format, TextureUsage _usage) override;

		/** @see RenderManager::createVertexBuffer */
		IVertexBuffer* createVertexBuffer() override;
		/** @see RenderManager::destroyVertexBuffer */
		void destroyVertexBuffer(IVertexBuffer* _buffer) override;

		/** @see RenderManager::createTexture */
		ITexture* createTexture(const std::string& _name) override;
		/** @see RenderManager::destroyTexture */
		void destroyTexture(ITexture* _texture) override;
		/** @see RenderManager::getTexture */
		ITexture* getTexture(const std::string& _name) override;

		/** @see IRenderTarget::begin */
		void begin() override;
		/** @see IRenderTarget::end */
		void end() override;
		/** @see IRenderTarget::doRender */
		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;
		/** @see IRenderTarget::getInfo */
		const RenderTargetInfo& getInfo() const override;

		/** @see RenderManager::setViewSize */
		void setViewSize(int _width, int _height) override;

		/** @see RenderManager::registerShader */
		void registerShader(
			const std::string& _shaderName,
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile) override;

		/*internal:*/

		// Builds the batch for a draw call, resolving the texture's shader and inject state
		// sets. _injectState is the render manager's own inject state (may be nullptr) and,
		// when set, takes precedence over the texture's inject state.
		Batch createBatch(IVertexBuffer* _buffer, ITexture* _texture, size_t _count, osg::StateSet* _injectState) const;

		// Returns the program registered under _shaderName, or nullptr if it was not registered
		osg::Program* getShaderProgram(const std::string& _shaderName) const;

		// Called by the update traversal
		void update();
		// Called by the cull traversal
		void collectDrawCalls();

		// Deferred RTT camera management. The RTT cameras are added to / removed from the GUI
		// camera during the cull traversal, which would invalidate osg's children iterators.
		// The operations are queued here and applied by flushRTTCameras before the GUI camera
		// children are traversed.
		void queueRTTAdd(osg::Camera* _camera);
		void queueRTTRemove(osg::Camera* _camera);
		void flushRTTCameras();

		// Returns the GUI camera the GUI content is rendered with
		osg::Camera* getGuiRoot() const;

		/** Install a custom image loader, used to load texture images by file name. */
		void setImageLoader(OsgImageLoader _loader);

		/** Specify a StateSet to inject for rendering. The StateSet will be used by future doRender calls until you
			reset it to nullptr again. */
		void setInjectState(osg::StateSet* _stateSet);

		/** Merge a StateSet (e.g. containing an osg::Program) into the GUI drawable's state set. */
		void setGuiStateSet(osg::StateSet* _stateSet);

		osg::ref_ptr<osg::Image> loadImage(const std::string& _fileName) const;

	private:
		std::string loadFileContent(const std::string& _file);
		osg::ref_ptr<osg::Program> createShaderProgram(
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile);

		osg::ref_ptr<osg::Group> mSceneRoot;
		osg::ref_ptr<osg::Camera> mGuiRoot;
		osg::ref_ptr<Drawable> mDrawable;

		std::vector<osg::ref_ptr<osg::Camera>> mPendingRTTAdd;
		std::vector<osg::Camera*> mPendingRTTRemove;

		IntSize mViewSize;
		bool mUpdate{false};
		VertexColourType mVertexFormat;
		RenderTargetInfo mInfo;

		using MapTexture = std::map<std::string, ITexture*>;
		MapTexture mTextures;

		using MapShader = std::map<std::string, osg::ref_ptr<osg::Program>>;
		MapShader mRegisteredShaders;

		osg::StateSet* mInjectState{nullptr};
		OsgImageLoader mImageLoader;

		bool mIsInitialise{false};
	};

} // namespace MyGUI

#endif // MYGUI_OSG_RENDER_MANAGER_H_
