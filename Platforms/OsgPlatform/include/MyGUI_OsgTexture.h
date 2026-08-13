/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#ifndef MYGUI_OSG_TEXTURE_H_
#define MYGUI_OSG_TEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"

#include <osg/ref_ptr>

namespace osg
{
	class Image;
	class StateSet;
	class Texture2D;
}

namespace MyGUI
{

	class OsgRenderManager;

	class OsgTexture : public ITexture
	{
	public:
		OsgTexture(const std::string& _name, OsgRenderManager* _manager);
		OsgTexture(osg::Texture2D* _texture, osg::StateSet* _injectState = nullptr);
		~OsgTexture() override;

		const std::string& getName() const override;

		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format) override;
		void loadFromFile(const std::string& _filename) override;
		void saveToFile(const std::string& _filename) override;
		void setShader(const std::string& _shaderName) override;

		void destroy() override;

		int getWidth() const override;
		int getHeight() const override;

		void* lock(TextureUsage _access) override;
		void unlock() override;
		bool isLocked() const override;

		PixelFormat getFormat() const override;
		TextureUsage getUsage() const override;
		size_t getNumElemBytes() const override;

		IRenderTarget* getRenderTarget() override;

		/*internal:*/
		osg::Texture2D* getTexture() const;
		osg::StateSet* getInjectState();
		// Returns the StateSet carrying the texture's shader program, or nullptr
		osg::StateSet* getShaderStateSet();

	private:
		std::string mName;
		OsgRenderManager* mRenderManager;

		osg::ref_ptr<osg::Image> mLockedImage;
		osg::ref_ptr<osg::Texture2D> mTexture;
		osg::ref_ptr<osg::StateSet> mInjectState;
		osg::ref_ptr<osg::StateSet> mShaderStateSet;
		IRenderTarget* mRenderTarget{nullptr};
		PixelFormat mFormat;
		TextureUsage mUsage;
		size_t mNumElemBytes;

		int mWidth;
		int mHeight;
	};

} // namespace MyGUI

#endif // MYGUI_OSG_TEXTURE_H_
