/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef _1fa6ce0c_1576_452a_98d8_ac49ed9e38d1_
#define _1fa6ce0c_1576_452a_98d8_ac49ed9e38d1_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_ExportTexture.h"
#include "MyGUI_ExportVertexBuffer.h"
#include "RenderBatchInfo.h"

namespace MyGUI
{

	class ExportRenderManager :
		public RenderManager,
		public IRenderTarget
	{
	public:
		ExportRenderManager();

		void initialise();
		void shutdown();

		static ExportRenderManager& getInstance();
		static ExportRenderManager* getInstancePtr();

		/** @see RenderManager::getViewSize */
		virtual const IntSize& getViewSize() const;

		/** @see RenderManager::getVertexFormat */
		virtual VertexColourType getVertexFormat();

		/** @see RenderManager::createVertexBuffer */
		virtual IVertexBuffer* createVertexBuffer();
		/** @see RenderManager::destroyVertexBuffer */
		virtual void destroyVertexBuffer(IVertexBuffer* _buffer);

		/** @see RenderManager::createTexture */
		virtual ITexture* createTexture(const std::string& _name);
		/** @see RenderManager::destroyTexture */
		virtual void destroyTexture(ITexture* _texture);
		/** @see RenderManager::getTexture */
		virtual ITexture* getTexture(const std::string& _name);

		/** @see RenderManager::isFormatSupported */
		virtual bool isFormatSupported(PixelFormat _format, TextureUsage _usage);

		/** @see IRenderTarget::begin */
		virtual void begin();
		/** @see IRenderTarget::end */
		virtual void end();

		/** @see IRenderTarget::doRender */
		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		/** @see IRenderTarget::getInfo */
		virtual const RenderTargetInfo& getInfo();

		/*internal:*/
		void drawOneFrame();
		void setViewSize(int _width, int _height);

		RenderBatchInfo* getBatchInfo(size_t _index);
		void addTexture(const std::string& _name, size_t _id, int _width, int _height);

		void doRenderLayer(ILayer* _layer);

	private:
		void destroyAllTextures();

	private:
		IntSize mViewSize;
		RenderTargetInfo mInfo;
		typedef std::map<std::string, ExportTexture*> MapTexture;
		MapTexture mTextures;
		bool mUpdate;
		typedef std::vector<RenderBatchInfo> VectorBatchInfo;
		VectorBatchInfo mBatchInfo;
		typedef std::vector<ExportVertexBuffer*> VectorExportVertexBuffer;
		VectorExportVertexBuffer mExportVertexBuffer;
		ILayer* mCurrentLayer;
	};

}

#endif
