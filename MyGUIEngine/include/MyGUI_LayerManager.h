/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_LAYER_MANAGER_H_
#define MYGUI_LAYER_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_ILayer.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class MYGUI_EXPORT LayerManager :
		public Singleton<LayerManager>,
		public IUnlinkWidget,
		public MemberObsolete<LayerManager>
	{
	public:
		typedef std::vector<ILayer*> VectorLayer;
		typedef Enumerator<VectorLayer> EnumeratorLayer;

	public:
		LayerManager();

		void initialise();
		void shutdown();

		/** Attach widget to specific layer
			@param _name Layer name
			@param _item Widget pointer
		*/
		void attachToLayerNode(const std::string& _name, Widget* _item);
		/** Detach widget from layer
			@param _item Widget pointer
		*/
		void detachFromLayer(Widget* _item);

		/** Up widget to be on top of its layer
			@param _item Widget pointer
		*/
		void upLayerItem(Widget* _item);

		/** Check is layer exist */
		bool isExist(const std::string& _name) const;
		/** Get layer nodes Enumerator */
		EnumeratorLayer getEnumerator() const;

		/** Create new layer;
			@param _name Layer name
			@param _type Could be OverlappedLayer, SharedLayer or any custom registered layer type.
			@param _index New layer will be placed before old layer with given index.
		*/
		ILayer* createLayerAt(const std::string& _name, const std::string& _type, size_t _index);

		/** Get layer by name */
		ILayer* getByName(const std::string& _name, bool _throw = true) const;

		size_t getLayerCount() const;

		ILayer* getLayer(size_t _index);

		/** Get top visible and enabled widget at specified position */
		Widget* getWidgetFromPoint(int _left, int _top);

		/** Render all layers to specified target */
		void renderToTarget(IRenderTarget* _target, bool _update);

		void resizeView(const IntSize& _viewSize);

		const std::string& getCategoryName() const;

	private:
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);
		void _unlinkWidget(Widget* _widget);

		ILayer* _createLayerObject(const std::string& _type);

		void clear();

		void merge(VectorLayer& _layers);
		void destroy(ILayer* _layer);

	private:
		VectorLayer mLayerNodes;

		bool mIsInitialise;
		std::string mCategoryName;
	};

} // namespace MyGUI

#endif // MYGUI_LAYER_MANAGER_H_
