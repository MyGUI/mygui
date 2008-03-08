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

		void attachToLayerKeeper(const std::string& _name, WidgetPtr _item);
		void detachFromLayerKeeper(WidgetPtr _item);

		void upLayerItem(WidgetPtr _item);

		bool load(const std::string & _file, const std::string & _group);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

		LayerItem * _findLayerItem(int _left, int _top, LayerItem* &_root);

		// удаляем данный виджет из всех возможных мест
		void _unlinkWidget(WidgetPtr _widget);

		void _windowResized(const FloatSize& _size);

		inline const FloatSize& getViewSize() {return mViewSize;}

		inline float getMaximumDepth() {return mMaximumDepth;}

		inline float getPixScaleX() {return mPixScaleX;}
		inline float getPixScaleY() {return mPixScaleY;}

		inline float getHOffset() {return mHOffset;}
		inline float getVOffset() {return mVOffset;}

		inline float getAspectCoef() {return mAspectCoef;}

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


	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_MANAGER_H__
