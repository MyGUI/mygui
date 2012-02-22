/*!
	@file
	@author		Evmenov Georgiy
	@date		09/2009
	@module
*/

#ifndef __RENDER_BOX_H__
#define __RENDER_BOX_H__

#include <Ogre.h>
#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

namespace wraps
{

	class RenderBox
	{
	public:
		RenderBox() :
			mCanvas(nullptr),
			mCamera(nullptr),
			mRenderTarget(nullptr),
			mColour(Ogre::ColourValue::ZERO)
		{
		}

		virtual ~RenderBox()
		{
			destroy();
		}

		void setViewport(Ogre::Camera* _value)
		{
			removeViewport();
			mCamera = _value;

			if (mCanvas)
				mCanvas->updateTexture();
		}

		void removeViewport()
		{
			if (mCamera)
			{
				removeTexture();
				mCamera = nullptr;
			}
		}

		virtual void setCanvas(MyGUI::Canvas* _value)
		{
			destroy();

			mCanvas = _value;
			mCanvas->createTexture(MyGUI::Canvas::TRM_PT_VIEW_ALL, MyGUI::TextureUsage::RenderTarget);
			mCanvas->eventPreTextureChanges += MyGUI::newDelegate(this, &RenderBox::eventPreTextureChanges);
			mCanvas->requestUpdateCanvas = MyGUI::newDelegate(this, &RenderBox::requestUpdateCanvas);

			mCanvas->updateTexture();
		}

		virtual void destroy()
		{
			if (mCanvas)
			{
				mCanvas->eventPreTextureChanges -= MyGUI::newDelegate(this, &RenderBox::eventPreTextureChanges);
				mCanvas->requestUpdateCanvas = nullptr;
				mCanvas->destroyTexture();
				mCanvas = nullptr;
			}
		}

		void setBackgroundColour(const Ogre::ColourValue& _value)
		{
			mColour = _value;

			if (mCanvas)
			{
				mRenderTarget = nullptr;
				mCanvas->updateTexture();
			}
		}

		void setBackgroundColour(const MyGUI::Colour& _value)
		{
			setBackgroundColour(Ogre::ColourValue(_value.red, _value.green, _value.blue, _value.alpha));
		}

	protected:
		void removeTexture()
		{
			if (mRenderTarget != nullptr)
			{
				mRenderTarget->removeAllViewports();
				mRenderTarget = nullptr;
				if (mCanvas)
					Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTexture(mCanvas->getTexture()->getName());
			}
		}

		void eventPreTextureChanges(MyGUI::Canvas* _canvas)
		{
			removeTexture();
		}

		virtual void requestUpdateCanvas(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event)
		{
			if (!mCamera)
				return;

			if (!(_event.textureChanged || _event.requested))
				return;

			Ogre::TexturePtr texture = static_cast<MyGUI::OgreTexture*>(mCanvas->getTexture())->getOgreTexture();
			Ogre::RenderTexture* target = texture->getBuffer()->getRenderTarget();

			if (mRenderTarget != target
				&& target != nullptr
				&& mCamera != nullptr )
			{
				mRenderTarget = target;

				mRenderTarget->removeAllViewports();
				Ogre::Viewport* viewport = mRenderTarget->addViewport(mCamera);
				viewport->setBackgroundColour(mColour);
				viewport->setClearEveryFrame(true);
				viewport->setOverlaysEnabled(false);

			}
		}

	protected:
		MyGUI::Canvas* mCanvas;
		Ogre::Camera* mCamera;
		Ogre::RenderTarget* mRenderTarget;
		Ogre::ColourValue mColour;
	};

} // namespace wraps

#endif // __RENDER_BOX_H__
