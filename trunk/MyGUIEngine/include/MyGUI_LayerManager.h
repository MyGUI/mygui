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
#include "MyGUI_Enumerator.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IUnlinkWidget.h"

#include <OgreRenderQueueListener.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class LayerItem;
	class LayerKeeper;
	typedef LayerKeeper* LayerKeeperPtr;
	typedef std::vector<LayerKeeperPtr> VectorLayerKeeperPtr;
	typedef Enumerator<VectorLayerKeeperPtr> EnumeratorLayerKeeperPtr;

	class _MyGUIExport LayerManager : public Ogre::RenderQueueListener, public IUnlinkWidget
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

		// ������� ������ ������ �� ���� ��������� ����
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

		size_t getBatch() { return mCountBatch; }
		void _addBatch() { mCountBatch ++; }

		void clear();

		bool isExist(const std::string & _name);
		EnumeratorLayerKeeperPtr getEnumerator() { return EnumeratorLayerKeeperPtr(mLayerKeepers.begin(), mLayerKeepers.end()); }

	private:

		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);

		void merge(VectorLayerKeeperPtr & _layers);
		void destroy(LayerKeeperPtr _layer);

	private:
		VectorLayerKeeperPtr mLayerKeepers;

		// ���� ��� ���������� ���� � ���
		bool mUpdate;

		// ������ ������� � ������������� �����������
		float mPixScaleX;
		float mPixScaleY;

		// �������� ��� ����, ����� ������� ����� � �������
        float mHOffset;
        float mVOffset;

		float mAspectCoef;

		// ���������� ��
		float mMaximumDepth;

		Ogre::SceneManager * mSceneManager;

		size_t mCountBatch;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_MANAGER_H__
