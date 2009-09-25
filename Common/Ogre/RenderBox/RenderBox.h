/*!
	@file
	@author		Evmenov Georgiy
	@date		09/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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

		virtual void setCanvas(MyGUI::CanvasPtr _value)
		{
			destroy();

			mCanvas = _value;
			mCanvas->createTexture(MyGUI::Canvas::TRM_PT_VIEW_ALL, MyGUI::TextureUsage::RenderTarget);
			mCanvas->eventPreTextureChanges = MyGUI::newDelegate(this, &RenderBox::eventPreTextureChanges);
			mCanvas->requestUpdateCanvas = MyGUI::newDelegate(this, &RenderBox::requestUpdateCanvas);

			mCanvas->updateTexture();
		}

		virtual void destroy()
		{
			if (mCanvas)
			{
				mCanvas->eventPreTextureChanges = nullptr;
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

		void eventPreTextureChanges(MyGUI::CanvasPtr _canvas)
		{
			removeTexture();
		}

		virtual void requestUpdateCanvas(MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _event)
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
		MyGUI::CanvasPtr mCanvas;
		Ogre::Camera* mCamera;
		Ogre::RenderTarget* mRenderTarget;
		Ogre::ColourValue mColour;
	};

} // namespace wraps

#endif // __RENDER_BOX_H__
