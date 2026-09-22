#include "TestSupport.h"
#include "TestRunner.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_SharedLayer.h"
#include "MyGUI_SharedLayerNode.h"
#include "MyGUI_DataFileStream.h"
#include <fstream>

namespace
{

	using unittest::require;

	MyGUI::Widget* createWidget(std::string_view _layer, std::string_view _name, int _left = 10)
	{
		return MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(_left, 10, 80, 60),
			MyGUI::Align::Default,
			_layer,
			_name);
	}

	void testRegistryAndDestruction()
	{
		unittest::TestContext context;
		auto& layers = MyGUI::LayerManager::getInstance();
		auto* lower = layers.createLayerAt("Lower", "OverlappedLayer", 0);
		auto* upper = layers.createLayerAt("Upper", "OverlappedLayer", 1);
		auto* middle = layers.createLayerAt("Middle", "SharedLayer", 1);
		require(
			layers.getLayers() == MyGUI::LayerManager::VectorLayer({lower, middle, upper}),
			"Layer insertion must preserve the specified order");
		require(
			layers.getLayerCount() == 3 && layers.getLayer(1) == middle && layers.getByName("Middle") == middle,
			"Index and name lookups must refer to the same layer");
		require(
			!layers.isExist("Missing") && layers.getByName("Missing", false) == nullptr,
			"Nonthrowing lookup must report missing layers");
		unittest::requireThrows([&] { layers.getByName("Missing"); }, "Throwing lookup must reject a missing layer");
		unittest::requireThrows([&] { layers.getLayer(3); }, "Index lookup must reject the end index");
		unittest::requireThrows(
			[&] { layers.createLayerAt("Invalid", "SharedLayer", 4); },
			"Insertion beyond the end must fail");
		auto* retained = createWidget("Lower", "Retained");
		auto* removed = createWidget("Middle", "Removed");
		createWidget("Middle", "AlsoRemoved", 100);
		removed->createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(0, 0, 10, 10),
			MyGUI::Align::Default,
			"RemovedChild");
		layers.destroyLayer("Middle");
		auto& gui = context.getGui();
		for (const char* name : {"Removed", "AlsoRemoved", "RemovedChild"})
			require(
				gui.findWidget<MyGUI::Widget>(name, false) == nullptr,
				"Destroying a layer must destroy its widget trees");
		require(
			!layers.isExist("Middle") && layers.getLayerCount() == 2 && layers.getLayer(1) == upper,
			"Destroying a layer must update registry order");
		require(
			gui.findWidget<MyGUI::Widget>("Retained") == retained && retained->getLayer() == lower,
			"Destroying a layer must preserve widgets on other layers");
	}

	void testSharedLifetimeAndPicking()
	{
		unittest::TestContext context;
		auto& layers = MyGUI::LayerManager::getInstance();
		auto* layer = layers.createLayerAt("Shared", "SharedLayer", 0)->castType<MyGUI::SharedLayer>();
		layer->setPick(true);
		auto* first = createWidget("Shared", "First");
		auto* second = createWidget("Shared", "Second", 100);
		auto* node = static_cast<MyGUI::SharedLayerNode*>(first->getLayerNode());
		require(
			node == second->getLayerNode() && node->countUsing() == 2 && layer->getLayerNodeCount() == 1,
			"Shared roots must retain one reference-counted node");
		require(
			layer->getLayerNodeAt(0) == node && node->getLayer() == layer && node->getParent() == nullptr,
			"Shared node must expose its owner and root position");
		require(
			layers.getWidgetFromPoint(20, 20) == first && layers.getWidgetFromPoint(110, 20) == second,
			"Shared-layer picking must reach every attached root");
		layer->setPick(false);
		require(
			layers.getWidgetFromPoint(20, 20) == nullptr,
			"Disabling shared-layer picking must hide its widgets from picking");
		layer->setPick(true);
		layers.detachFromLayer(first);
		require(
			first->getLayerNode() == nullptr && node->countUsing() == 1 && layer->getLayerNodeAt(0) == node,
			"Detaching one root must retain the node used by another root");
		require(
			layers.getWidgetFromPoint(20, 20) == nullptr && layers.getWidgetFromPoint(110, 20) == second,
			"Detachment must remove only the detached root from picking");
		layers.attachToLayerNode("Shared", first);
		require(
			first->getLayerNode() == node && node->countUsing() == 2,
			"Reattachment must reuse the live shared node");
		context.getGui().destroyWidget(second);
		require(node->countUsing() == 1, "Widget destruction must release its shared-node reference");
		context.getGui().destroyWidget(first);
		require(
			layer->getLayerNodeCount() == 0 && layer->getChildItems().empty(),
			"The last root must release the shared node");
		require(layer->getLayerItemByPoint(20, 20) == nullptr, "An empty shared layer must not produce a hit");
		createWidget("Shared", "Replacement");
		require(layer->getLayerNodeCount() == 1, "An emptied shared layer must support new roots");
	}

	void testNestedNodes()
	{
		unittest::TestContext context;
		for (const char* type : {"OverlappedLayer", "SharedLayer"})
		{
			auto& layers = MyGUI::LayerManager::getInstance();
			auto* layer = layers.createLayerAt(type, type, layers.getLayerCount());
			auto* root = layer->createChildItemNode();
			auto* first = root->createChildItemNode();
			auto* second = root->createChildItemNode();
			require(
				first->getParent() == root && first->getLayer() == layer,
				"Child nodes must retain parent and layer ownership");
			layer->upChildItemNode(first);
			require(
				root->getChildItems() == MyGUI::VectorILayerNode({second, first}),
				"Raising a nested node through its layer must reorder siblings");
			require(
				root->getLayerNodeCount() == 2 && root->getLayerNodeAt(1) == first,
				"Child enumeration must reflect the new order");
			auto& renderer = MyGUI::DummyRenderManager::getInstance();
			layer->renderToTarget(&renderer, false);
			auto isOutOfDate = [&]
			{
				return layer->isType<MyGUI::SharedLayer>()
					? layer->castType<MyGUI::SharedLayer>()->isOutOfDate()
					: layer->castType<MyGUI::OverlappedLayer>()->isOutOfDate();
			};
			require(!isOutOfDate(), "Rendering must clear structural invalidation");
			first->outOfDate(nullptr);
			require(isOutOfDate(), "Nested node invalidation must be visible through the layer");
			layer->renderToTarget(&renderer, false);
			if (std::string_view(type) == "OverlappedLayer")
				layer->destroyChildItemNode(first);
			else
				root->destroyChildItemNode(first);
			require(
				root->getLayerNodeCount() == 1 && root->getLayerNodeAt(0) == second,
				"Removing a child node must preserve its siblings");
			layer->destroyChildItemNode(root);
			require(layer->getLayerNodeCount() == 0, "Root-node destruction must remove its entire subtree");
		}
	}

	void testViewResize()
	{
		unittest::TestContext context;
		auto& layers = MyGUI::LayerManager::getInstance();
		for (const char* type : {"OverlappedLayer", "SharedLayer"})
		{
			auto* layer = layers.createLayerAt(type, type, layers.getLayerCount());
			require(layer->getSize() == MyGUI::IntSize(800, 600), "New layers must inherit the renderer's view size");
			auto* right = createWidget(type, std::string(type) + "Right");
			right->setAlign(MyGUI::Align::Right | MyGUI::Align::Bottom);
			auto* stretch = createWidget(type, std::string(type) + "Stretch", 100);
			stretch->setAlign(MyGUI::Align::Stretch);
		}
		MyGUI::DummyRenderManager::getInstance().setViewSize(1000, 700);
		for (const char* type : {"OverlappedLayer", "SharedLayer"})
		{
			auto* layer = layers.getByName(type);
			require(layer->getSize() == MyGUI::IntSize(1000, 700), "Renderer resize must propagate to every layer");
			require(
				layer->getPosition(23, 45) == MyGUI::IntPoint(23, 45),
				"Screen layers must preserve target coordinates");
			auto* right = context.getGui().findWidget<MyGUI::Widget>(std::string(type) + "Right");
			auto* stretch = context.getGui().findWidget<MyGUI::Widget>(std::string(type) + "Stretch");
			require(
				right->getCoord() == MyGUI::IntCoord(210, 110, 80, 60),
				"Anchored roots must move by the view-size delta");
			require(
				stretch->getCoord() == MyGUI::IntCoord(100, 10, 280, 160),
				"Stretched roots must grow by the view-size delta");
		}
	}

	// Exercise the public XML loader with real files; DummyDataManager has no file access.
	class LayerDataManager : public MyGUI::DummyDataManager
	{
	public:
		MyGUI::IDataStream* getData(const std::string& _name) const override
		{
			auto stream = std::make_unique<std::ifstream>(unittest::getResourcePath("UnitTest_Layers/" + _name));
			if (!*stream)
				return nullptr;
			return new MyGUI::DataFileStream(std::move(stream));
		}
	};

	class LayerXmlContext
	{
	public:
		LayerXmlContext()
		{
			mRenderer.initialise();
			mRenderer.setViewSize(800, 600);
			mGui.initialise("");
		}
		~LayerXmlContext()
		{
			mGui.shutdown();
			mRenderer.shutdown();
		}
		MyGUI::Gui& getGui()
		{
			return mGui;
		}

	private:
		MyGUI::LogManager mLog;
		MyGUI::DummyRenderManager mRenderer;
		LayerDataManager mData;
		MyGUI::Gui mGui;
	};

	void testXmlLoadingAndMerge()
	{
		LayerXmlContext context;
		auto& layers = MyGUI::LayerManager::getInstance();
		require(MyGUI::ResourceManager::getInstance().load("Layers.xml"), "Layer XML must load successfully");
		auto* retainedLayer = layers.getByName("Retained");
		require(
			retainedLayer->isType<MyGUI::OverlappedLayer>() && layers.getByName("Shared")->isType<MyGUI::SharedLayer>(),
			"Layer XML must create the declared layer types");
		auto* retained = createWidget("Retained", "RetainedWidget");
		auto* shared = createWidget("Shared", "SharedWidget", 100);
		createWidget("Unpickable", "UnpickableWidget", 200);
		require(
			layers.getWidgetFromPoint(20, 20) == retained && layers.getWidgetFromPoint(110, 20) == shared &&
				layers.getWidgetFromPoint(210, 20) == nullptr,
			"XML Pick properties must control layer hit testing");
		// Reload may remove empty layers; keep live widgets only on retained layers.
		context.getGui().destroyWidget(context.getGui().findWidget<MyGUI::Widget>("UnpickableWidget"));
		auto* retainedNode = retained->getLayerNode();
		require(
			MyGUI::ResourceManager::getInstance().load("ReloadedLayers.xml"),
			"Replacement layer XML must load successfully");
		require(
			layers.getLayerCount() == 3 && layers.getLayer(0)->getName() == "Shared" &&
				layers.getLayer(1) == retainedLayer && layers.getLayer(2)->getName() == "Added",
			"Reload must apply declared layer order, remove omitted layers, and append new layers");
		require(
			!layers.isExist("Unpickable") && retained->getLayerNode() == retainedNode &&
				retained->getLayer() == retainedLayer,
			"Reload must preserve matching layer objects and attached nodes");
		require(
			layers.getWidgetFromPoint(20, 20) == retained && layers.getWidgetFromPoint(110, 20) == shared,
			"Reload must preserve hit testing for retained widgets");
	}

	void testLegacyXml()
	{
		for (const char* file : {"LegacyLayers.xml", "UnversionedLayers.xml", "Version09Layers.xml"})
		{
			LayerXmlContext context;
			require(MyGUI::ResourceManager::getInstance().load(file), "Legacy layer XML must load successfully");
			auto& layers = MyGUI::LayerManager::getInstance();
			require(
				layers.getByName("Overlapped")->isType<MyGUI::OverlappedLayer>() &&
					layers.getByName("Shared")->isType<MyGUI::SharedLayer>(),
				"Legacy overlapped attributes must select the layer type");
			auto* first = createWidget("Overlapped", "First");
			auto* second = createWidget("Shared", "Second", 100);
			require(
				layers.getWidgetFromPoint(20, 20) == first && layers.getWidgetFromPoint(110, 20) == second,
				"Legacy pick and peek attributes must enable hit testing for both layer types");
		}
	}

}

int main()
{
	return unittest::runTests({
		{"Layer registry and widget destruction", testRegistryAndDestruction},
		{"Shared-layer lifetime and picking", testSharedLifetimeAndPicking},
		{"Nested layer-node ordering and invalidation", testNestedNodes},
		{"Renderer view-size propagation", testViewResize},
		{"Layer XML loading and merge", testXmlLoadingAndMerge},
		{"Legacy layer XML", testLegacyXml},
	});
}
