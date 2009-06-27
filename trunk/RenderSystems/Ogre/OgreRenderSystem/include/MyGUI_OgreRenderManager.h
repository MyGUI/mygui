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
#ifndef __MYGUI_OGRE_RENDER_MANAGER_H__
#define __MYGUI_OGRE_RENDER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_IRenderQueueListener.h"
#include "MyGUI_RenderFormat.h"
//#include "MyGUI_ITexture.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

#include <Ogre.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class OgreRenderManager :
		public RenderManager,
		public Ogre::WindowEventListener,
		public Ogre::RenderQueueListener,
		public Ogre::RenderSystem::Listener
	{
		MYGUI_INSTANCE_HEADER(OgreRenderManager);

	public:
		void initialise(Ogre::RenderWindow* _window);
		void shutdown();

		virtual const IntSize& getViewSize() { return mViewSize; }

		virtual VertexColourType getVertexFormat() { return mVertexFormat; }

		//virtual ITexture* createTexture(const std::string& _name);
		//virtual void destroyTexture(ITexture* _texture);

		/** Get resource by name*/
		//virtual ITexture* getByName(const std::string& _name);

		virtual IVertexBuffer* createVertexBuffer();
		virtual void destroyVertexBuffer(IVertexBuffer* _buffer);

		virtual void begin();
		virtual void end();

		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);
		virtual void doRender(IVertexBuffer* _buffer, const std::string& _texture, size_t _count);

		virtual const RenderTargetInfo& getInfo() { return mRenderTargetInfo; }

		/** Set scene manager where MyGUI will be rendered */
		void setSceneManager(Ogre::SceneManager * _scene);

		void setRenderQueueListener(IRenderQueueListener* _listener);

		/** Get GUI viewport index */
		size_t getActiveViewport() { return mActiveViewport; }

		/** Set GUI viewport index */
		void setActiveViewport(size_t _num);

		Ogre::RenderWindow * getRenderWindow() { return mWindow; }

	private:
		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);
		virtual void windowResized(Ogre::RenderWindow* rw);

		// ��������������� ������
		virtual void eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters);

		//void clearTextures();

	private:
		// ���� ��� ���������� ���� � ���
		bool mUpdate;

		IntSize mViewSize;

		Ogre::SceneManager * mSceneManager;

		IRenderQueueListener* mListener;

		VertexColourType mVertexFormat;

		//typedef std::map<std::string, ITexture*> MapTexture;
		//MapTexture mTextures;

		// ����, �� ������� �� ������������� ��� ��������� ��������
		Ogre::RenderWindow* mWindow;

		// �������, � ������� �������� �������
		size_t mActiveViewport;

		Ogre::RenderSystem* mRenderSystem;
		Ogre::TextureUnitState::UVWAddressingMode mTextureAddressMode;
		Ogre::LayerBlendModeEx mColorBlendMode, mAlphaBlendMode;

		RenderTargetInfo mRenderTargetInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_RENDER_MANAGER_H__
