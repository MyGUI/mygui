#include "Interaction.h"
#include "BehaviourTestSupport.h"
#include "FixedFont.h"
#include "SkinTestContext.h"
#include <vector>

void testMenuMutations()
{
	using unittest::require;
	std::vector<std::string> accepted;
	unittest::SkinTestContext context;
	context.loadSkins();
	unittest::registerFixedFont();
	unittest::loadMedia("MyGUI_BlueWhiteTemplates.xml");
	unittest::createInputLayer();
	auto* layer = MyGUI::LayerManager::getInstance().createLayerAt("Popup", "OverlappedLayer", 1);
	layer->castType<MyGUI::OverlappedLayer>()->setPick(true);
	auto& gui = MyGUI::Gui::getInstance();
	auto& input = MyGUI::InputManager::getInstance();
	auto* menu = gui.createWidget<MyGUI::MenuControl>(
		"PopupMenu",
		MyGUI::IntCoord(20, 20, 150, 100),
		MyGUI::Align::Default,
		"Main");
	menu->eventMenuCtrlAccept += MyGUI::newDelegate(
		[&](MyGUI::MenuControl*, MyGUI::MenuItem* _item) { accepted.emplace_back(_item->getItemId()); },
		1);
	auto click = [&](MyGUI::MenuItem* _item)
	{
		menu->setVisible(true);
		const auto p = _item->getAbsolutePosition() + MyGUI::IntPoint(4, 4);
		unittest::clickAt(p.left, p.top);
		gui.eventFrameStart(1.0f);
	};
	auto* owner = menu->addItem("File", MyGUI::MenuItemType::Popup, "file");
	auto* leaf = owner->createItemChild()->addItem("Open", MyGUI::MenuItemType::Normal, "open");
	auto* retained = menu->addItem("Help", MyGUI::MenuItemType::Normal, "help");
	click(owner);
	input.setKeyFocusWidget(leaf);
	require(menu->findItemById("open", true) == leaf, "Recursive lookup must find an open submenu item");
	menu->removeItemChildAt(0);
	gui.eventFrameStart(1.0f);
	require(
		owner->getItemChild() == nullptr && menu->findItemById("open", true) == nullptr &&
			input.getKeyFocusWidget() != leaf && accepted.empty(),
		"Removing a focused submenu must clear its lookup and focus without accepting it");
	leaf = owner->createItemChild()->addItem("New", MyGUI::MenuItemType::Normal, "new");
	owner->setItemChildVisible(true);
	input.setKeyFocusWidget(leaf);
	menu->removeItemAt(0);
	gui.eventFrameStart(1.0f);
	require(
		menu->getItemCount() == 1 && menu->getItemAt(0) == retained && menu->getItemIndexById("help") == 0 &&
			menu->findItemById("file", true) == nullptr && input.getKeyFocusWidget() != leaf,
		"Removing an open popup owner must preserve remaining items and release descendant focus");
	click(retained);
	require(accepted == std::vector<std::string>{"help"}, "A surviving item must accept exactly once after removal");
	menu->removeAllItems();
	require(menu->getItemCount() == 0, "Clearing the menu must remove every item");
	auto* reused = menu->addItem("Again", MyGUI::MenuItemType::Normal, "again");
	menu->setItemNameAt(0, "Renamed");
	menu->setItemTypeAt(0, MyGUI::MenuItemType::Separator);
	require(menu->getItemNameAt(0) == "Renamed", "A separator must retain its stored name for later type changes");
	click(reused);
	require(accepted.size() == 1, "A separator must not accept clicks after an item type change");
	menu->setItemTypeAt(0, MyGUI::MenuItemType::Normal);
	click(reused);
	require(menu->getItemNameAt(0) == "Renamed", "Changing item types must preserve the item name");
	require(
		accepted == std::vector<std::string>({"help", "again"}),
		"Restoring a normal item must preserve its ID and retain one callback subscription");
	menu->setItemTypeAt(0, MyGUI::MenuItemType::Popup);
	leaf = reused->createItemChild()->addItem("Child", MyGUI::MenuItemType::Normal, "child");
	click(reused);
	require(reused->getItemChild()->getVisible(), "An item converted to a popup must open its child");
	click(leaf);
	require(
		accepted == std::vector<std::string>({"help", "again", "child"}),
		"A recreated popup must propagate acceptance once through the original root subscription");
	click(reused);
	input.setKeyFocusWidget(leaf);
	menu->removeAllItems();
	gui.eventFrameStart(1.0f);
	require(
		menu->getItemCount() == 0 && input.getKeyFocusWidget() != leaf && accepted.size() == 3,
		"Clearing an open menu must release focus and pending animations without accepting an item");
}

void testMenuInteractions()
{
	using unittest::require;
	std::vector<MyGUI::MenuItem*> accepted;
	unittest::SkinTestContext context;
	context.loadSkins();
	unittest::registerFixedFont();
	unittest::loadMedia("MyGUI_BlueWhiteTemplates.xml");
	unittest::createInputLayer();
	auto* layer = MyGUI::LayerManager::getInstance().createLayerAt("Popup", "OverlappedLayer", 1);
	layer->castType<MyGUI::OverlappedLayer>()->setPick(true);
	auto* menu = MyGUI::Gui::getInstance().createWidget<MyGUI::MenuBar>(
		"MenuBar",
		MyGUI::IntCoord(20, 20, 400, 26),
		MyGUI::Align::Default,
		"Main");
	auto* first = menu->addItem("File", MyGUI::MenuItemType::Popup);
	auto* second = menu->addItem("Edit", MyGUI::MenuItemType::Popup);
	auto* child = first->createItemChild();
	auto* leaf = child->addItem("Open");
	auto* other = second->createItemChild();
	other->addItem("Copy");
	menu->eventMenuCtrlAccept +=
		MyGUI::newDelegate([&](MyGUI::MenuControl*, MyGUI::MenuItem* _item) { accepted.push_back(_item); }, 1);
	auto& input = MyGUI::InputManager::getInstance();
	auto p = first->getAbsolutePosition() + MyGUI::IntPoint(4, 4);
	input.injectMouseMove(p.left, p.top, 0);
	require(!child->getVisible(), "Hovering a closed menu bar must not open its submenu");
	unittest::clickAt(p.left, p.top);
	MyGUI::Gui::getInstance().eventFrameStart(1.0f);
	require(child->getVisible() && first->getStateSelected(), "Click must open and select the popup owner");
	auto q = second->getAbsolutePosition() + MyGUI::IntPoint(4, 4);
	input.injectMouseMove(q.left, q.top, 0);
	MyGUI::Gui::getInstance().eventFrameStart(1.0f);
	require(other->getVisible() && !child->getVisible(), "Hovering another item while open must switch submenus");
	input.injectMouseMove(p.left, p.top, 0);
	MyGUI::Gui::getInstance().eventFrameStart(1.0f);
	auto l = leaf->getAbsolutePosition() + MyGUI::IntPoint(4, 4);
	unittest::clickAt(l.left, l.top);
	MyGUI::Gui::getInstance().eventFrameStart(1.0f);
	require(
		accepted == std::vector<MyGUI::MenuItem*>{leaf},
		"Leaf click must propagate exactly one acceptance to the root menu");
	require(!child->getVisible(), "Accepting a leaf must close its popup");
	unittest::clickAt(p.left, p.top);
	MyGUI::Gui::getInstance().eventFrameStart(1.0f);
	unittest::clickAt(p.left, p.top);
	MyGUI::Gui::getInstance().eventFrameStart(1.0f);
	require(!child->getVisible(), "Clicking an open popup owner must toggle it closed");
}
