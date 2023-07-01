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

	class MYGUI_EXPORT LayerManager : public IUnlinkWidget, public MemberObsolete<LayerManager>
	{
		MYGUI_SINGLETON_DECLARATION(LayerManager);

	public:
		using VectorLayer = std::vector<ILayer*>;
		using EnumeratorLayer = Enumerator<VectorLayer>;

	public:
		LayerManager();

		void initialise();
		void shutdown();

		/** Attach widget to specific layer
			@param _name Layer name
			@param _item Widget pointer
		*/
		void attachToLayerNode(std::string_view _name, Widget* _item);
		/** Detach widget from layer
			@param _item Widget pointer
		*/
		void detachFromLayer(Widget* _item);

		/** Up widget to be on top of its layer
			@param _item Widget pointer
		*/
		void upLayerItem(Widget* _item);

		/** Check is layer exist */
		bool isExist(std::string_view _name) const;
		/** Get layer nodes Enumerator */
		EnumeratorLayer getEnumerator() const;

		/** Create new layer;
			@param _name Layer name
			@param _type Could be OverlappedLayer, SharedLayer or any custom registered layer type.
			@param _index New layer will be placed before old layer with given index.
		*/
		ILayer* createLayerAt(std::string_view _name, std::string_view _type, size_t _index);

		/** Get layer by name */
		ILayer* getByName(std::string_view _name, bool _throw = true) const;

		size_t getLayerCount() const;

		ILayer* getLayer(size_t _index) const;

		/** Get top visible and enabled widget at specified position */
		Widget* getWidgetFromPoint(int _left, int _top) const;

		/** Render all layers to specified target */
		void renderToTarget(IRenderTarget* _target, bool _update);

		void resizeView(const IntSize& _viewSize);

		const std::string& getCategoryName() const;

	private:
		void _load(xml::ElementPtr _node, std::string_view _file, Version _version);
		void _unlinkWidget(Widget* _widget) override;

		ILayer* _createLayerObject(std::string_view _type);

		void clear();

		void merge(VectorLayer& _layers);
		void destroy(ILayer* _layer);

	private:
		VectorLayer mLayerNodes;

		bool mIsInitialise{false};
		std::string mCategoryName;
	};

} // namespace MyGUI

#endif // MYGUI_LAYER_MANAGER_H_
