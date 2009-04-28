/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*//*
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
#ifndef __MYGUI_RENDER_MANAGER_H__
#define __MYGUI_RENDER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_IRenderQueueListener.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_ITexture.h"

#include <OgreRenderQueueListener.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class MYGUI_EXPORT RenderManager :
		public Ogre::RenderQueueListener,
		public Ogre::RenderSystem::Listener
	{
		MYGUI_INSTANCE_HEADER(RenderManager);

	public:
		void initialise();
		void shutdown();

		void _windowResized(const IntSize& _size);

		/** Get maximum depth */
		float getMaximumDepth() { return mMaximumDepth; }

		/** Get X pixel scale */
		float getPixScaleX() { return mPixScaleX; }
		/** Get Y pixel scale */
		float getPixScaleY() { return mPixScaleY; }

		/** Get horisontal texel offset divided by window width */
		float getHOffset() { return mHOffset; }
		/** Get vertical texel offset divided by window height */
		float getVOffset() { return mVOffset; }

		/** Get aspect coefficient */
		float getAspectCoef() { return mAspectCoef; }

		/** Set scene manager where MyGUI will be rendered */
		void setSceneManager(Ogre::SceneManager * _scene);

		/** Get current batch count */
		size_t getBatch() { return mCountBatch; }
		void _addBatch() { mCountBatch ++; }

		void setRenderQueueListener(IRenderQueueListener* _listener);

		VertexFormat getVertexFormat() { return mVertexFormat; }

		ITexture* createTexture(const std::string& _name);
		void destroyTexture(ITexture* _texture);

		/** Get resource by name*/
		ITexture* getByName(const std::string& _name);

		/** Check is resource exist */
		bool isExist(const std::string& _name);

	private:
		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);

		// восстанавливаем буферы
		virtual void eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters);

		void clear();

	private:
		// флаг для обновления всех и вся
		bool mUpdate;

		// размер пикселя в относительных координатах
		float mPixScaleX;
		float mPixScaleY;

		// смещение для того, чтобы тексель попал в пиксель
        float mHOffset;
        float mVOffset;

		float mAspectCoef;

		// координата зю
		float mMaximumDepth;

		Ogre::SceneManager * mSceneManager;

		size_t mCountBatch;

		IRenderQueueListener* mListener;

		VertexFormat mVertexFormat;

		typedef std::map<std::string, ITexture*> MapTexture;
		MapTexture mTextures;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_MANAGER_H__
