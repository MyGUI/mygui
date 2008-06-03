/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_LAYER_MANAGER_H__
#define __MYGUI_LAYER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_UnlinkWidget.h"

#include <Ogre.h>
#include <OgrePrerequisites.h>
#include <OgreRenderQueueListener.h>

namespace MyGUI
{

	class LayerItem;
	class LayerKeeper;
	typedef std::vector<LayerKeeper*> VectorLayerKeeper;

	class _MyGUIExport LayerManager : public Ogre::RenderQueueListener, public UnlinkWidget
	{
		INSTANCE_HEADER(LayerManager);

	public:
		void initialise();
		void shutdown();

		/** Attach widget to specific layer
			@param
				_name of layer
			@param
				_item pointer to widget
		*/
		void attachToLayerKeeper(const std::string& _name, WidgetPtr _item);
		/** Detach widget from layer
			@param
				_item pointer to widget
		*/
		void detachFromLayerKeeper(WidgetPtr _item);

		/** Up widget to be on top of its layer
			@param
				_item pointer to widget
		*/
		void upLayerItem(WidgetPtr _item);

		/** Load additional MyGUI *.layer file */
		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

		LayerItem * _findLayerItem(int _left, int _top, LayerItem* &_root);

		// удаляем данный виджет из всех возможных мест
		void _unlinkWidget(WidgetPtr _widget);

		void _windowResized(const FloatSize& _size);

		/** Get view size of GUI area*/
		inline const FloatSize& getViewSize() {return mViewSize;}

		/** Get maximum depth */
		inline float getMaximumDepth() {return mMaximumDepth;}

		/** Get X pixel scale */
		inline float getPixScaleX() {return mPixScaleX;}
		/** Get Y pixel scale */
		inline float getPixScaleY() {return mPixScaleY;}

		/** Get horisontal texel offset divided by window width */
		inline float getHOffset() {return mHOffset;}
		/** Get vertical texel offset divided by window height */
		inline float getVOffset() {return mVOffset;}

		/** Get aspect coefficient */
		inline float getAspectCoef() {return mAspectCoef;}

		/** Set scene manager where MyGUI will be rendered */
		void setSceneManager(Ogre::SceneManager * _scene);

		inline size_t getBatch() {return mCountBatch;}
		inline void _addBatch() {mCountBatch ++;}

		bool exist(const std::string & _name);

	private:

		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);

	private:
		VectorLayerKeeper mLayerKeepers;

		FloatSize mViewSize;

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
