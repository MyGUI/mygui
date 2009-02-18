/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#ifndef __MYGUI_LAYER_MANAGER_H__
#define __MYGUI_LAYER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_ResourceManager.h"

#include <OgreRenderQueueListener.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class LayerItemKeeper;
	class LayerItem;
	class LayerKeeper;
	typedef LayerKeeper* LayerKeeperPtr;
	typedef std::vector<LayerKeeperPtr> VectorLayerKeeperPtr;
	typedef Enumerator<VectorLayerKeeperPtr> EnumeratorLayerKeeperPtr;

	class MYGUI_EXPORT LayerManager :
		public Ogre::RenderQueueListener,
		public Ogre::RenderSystem::Listener,
		public IUnlinkWidget
	{
		MYGUI_INSTANCE_HEADER(LayerManager);

	public:
		void initialise();
		void shutdown();

		/** Attach widget to specific layer
			@param _name Layer name
			@param _item Widget pointer
		*/
		void attachToLayerKeeper(const std::string& _name, WidgetPtr _item);
		/** Detach widget from layer
			@param _item Widget pointer
		*/
		void detachFromLayerKeeper(WidgetPtr _item);

		/** Up widget to be on top of its layer
			@param _item Widget pointer
		*/
		void upLayerItem(WidgetPtr _item);

		/** Load additional MyGUI *_layer.xml file */
		bool load(const std::string & _file, const std::string & _group = MyGUI::ResourceManager::GUIResourceGroupName);
		void _load(xml::ElementPtr _node, const std::string & _file, Version _version);

		// удаляем данный виджет из всех возможных мест
		void _unlinkWidget(WidgetPtr _widget);

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

		/** Check is layer exist */
		bool isExist(const std::string & _name);
		/** Get layer keepers Enumerator */
		EnumeratorLayerKeeperPtr getEnumerator() { return EnumeratorLayerKeeperPtr(mLayerKeepers); }

		/** Get top visible and enabled widget at specified position */
		WidgetPtr getWidgetFromPoint(int _left, int _top);

		/** Check if LayerItemKeeper still exist */
		bool isExistItem(LayerItemKeeper * _item);

	private:
		void clear();

		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);

		void merge(VectorLayerKeeperPtr & _layers);
		void destroy(LayerKeeperPtr _layer);

		// восстанавливаем буферы
		virtual void eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters);

	private:
		VectorLayerKeeperPtr mLayerKeepers;

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
	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_MANAGER_H__
