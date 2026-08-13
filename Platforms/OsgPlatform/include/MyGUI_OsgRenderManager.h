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

#include <functional>
#include <map>
#include <vector>
#include <osg/ref_ptr>

namespace osg
{
	class Camera;
	class Group;
	class Image;
	class Program;
	class StateSet;
}

namespace MyGUI
{

	class OsgTexture;

	class Drawable;

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
