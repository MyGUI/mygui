#include "MyGUI.h"
#include "TestSupport.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

namespace
{
	using Style = MyGUI::WidgetStyle;

	using unittest::require;

	class AttachmentTest : public MyGUI::IUnlinkWidget
	{
		struct Expected
		{
			MyGUI::Widget* widget;
			MyGUI::Widget* parent;
			Style style;
			std::string layer;
		};

	public:
		AttachmentTest()
		{
			MyGUI::WidgetManager::getInstance().registerUnlinker(this);
		}

		~AttachmentTest() override
		{
			MyGUI::Gui::getInstance().destroyAllChildWidget();
			MyGUI::WidgetManager::getInstance().unregisterUnlinker(this);
		}

		void _unlinkWidget(MyGUI::Widget* _widget) override
		{
			mLive.erase(_widget);
		}

		MyGUI::Widget* create(MyGUI::Widget* _parent, Style _style, std::string_view _layer)
		{
			mOperation = "create";
			const MyGUI::IntCoord coord(10, 10, 100, 100);
			auto* widget = _parent == nullptr
				? MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>("Default", coord, MyGUI::Align::Default, _layer)
				: _parent->createWidget<MyGUI::Widget>(
					  _style,
					  "Default",
					  coord,
					  MyGUI::Align::Default,
					  _style == Style::Popup ? _layer : "");
			mExpected.push_back(
				{widget,
				 _parent,
				 _parent == nullptr ? Style::Overlapped : _style,
				 std::string(_parent == nullptr || _style == Style::Popup ? _layer : "")});
			mLive.insert(widget);
			check();
			return widget;
		}

		void attach(MyGUI::Widget* _widget, MyGUI::Widget* _parent, Style _style)
		{
			mOperation = "attach";
			_widget->attachToWidget(_parent, _style, "Popup");
			auto& expected = find(_widget);
			expected.parent = _parent;
			expected.style = _style;
			expected.layer = _style == Style::Popup ? "Popup" : "";
			check();
		}

		void detach(MyGUI::Widget* _widget)
		{
			mOperation = "detach";
			_widget->detachFromWidget("Overlapped");
			auto& expected = find(_widget);
			if (expected.parent != nullptr && expected.style != Style::Popup)
				expected.style = Style::Overlapped;
			expected.parent = nullptr;
			expected.layer = "Overlapped";
			check();
		}

		void changeStyle(MyGUI::Widget* _widget, Style _style)
		{
			mOperation = "changeStyle";
			_widget->setWidgetStyle(_style, "Popup");
			auto& expected = find(_widget);
			if (expected.parent != nullptr && expected.style != _style)
			{
				expected.style = _style;
				expected.layer = _style == Style::Popup ? "Popup" : "";
			}
			check();
		}

		void changeLayer(MyGUI::Widget* _widget, bool _attach)
		{
			mOperation = "changeLayer";
			auto& expected = find(_widget);
			const bool root = expected.parent == nullptr || expected.style == Style::Popup;
			if (_attach)
			{
				if (root)
				{
					MyGUI::LayerManager::getInstance().attachToLayerNode("Main", _widget);
					expected.layer = "Main";
				}
			}
			else
			{
				MyGUI::LayerManager::getInstance().detachFromLayer(_widget);
				if (root)
					expected.layer.clear();
			}
			check();
		}

		void destroy(MyGUI::Widget* _widget)
		{
			mOperation = "destroy";
			std::unordered_set<MyGUI::Widget*> removed;
			for (const auto& expected : mExpected)
			{
				if (descendsFrom(expected.widget, _widget))
					removed.insert(expected.widget);
			}
			MyGUI::Gui::getInstance().destroyWidget(_widget);
			mExpected.erase(
				std::remove_if(
					mExpected.begin(),
					mExpected.end(),
					[&](const Expected& _expected) { return removed.count(_expected.widget) != 0; }),
				mExpected.end());
			check();
		}

		void scenarios()
		{
			auto* first = create(nullptr, Style::Overlapped, "Main");
			auto* second = create(nullptr, Style::Overlapped, "Overlapped");
			auto* child = create(first, Style::Child, "");
			create(child, Style::Overlapped, "");
			create(child, Style::Popup, "Popup");
			attach(child, second, Style::Child);
			// Reordering child drawing must retain the existing overlapped descendant nodes.
			child->setDepth(1);
			check();
			destroy(first); // The reparented subtree must survive its former parent's destruction.
			changeStyle(child, Style::Overlapped);
			changeStyle(child, Style::Popup);
			changeLayer(second, false); // Popup descendants retain their independent layer.
			changeStyle(child, Style::Child);
			changeLayer(second, true);
			detach(child);
			detach(child); // Detaching an already top-level widget remains valid.
			changeStyle(child, Style::Child); // Top-level style changes are ignored.
			attach(child, second, Style::Overlapped);
			attach(child, second, Style::Overlapped);
			changeLayer(second, false);
			changeLayer(second, false);
			changeLayer(second, true);
			destroy(second); // Includes child, grandchild, and independently layered popup.
			require(mExpected.empty(), "Destroying the parent must remove its entire subtree");
		}

		void randomStep(std::mt19937& _random)
		{
			const Style styles[] = {Style::Child, Style::Popup, Style::Overlapped};
			const auto style = styles[_random() % 3];
			if (mExpected.empty())
			{
				create(nullptr, Style::Overlapped, "Main");
				return;
			}
			auto* widget = mExpected[_random() % mExpected.size()].widget;
			switch (mExpected.size() > 100 ? 4 : _random() % 8)
			{
			case 0: changeLayer(widget, false); break;
			case 1: changeLayer(widget, true); break;
			case 2:
			{
				auto* parent = mExpected[_random() % mExpected.size()].widget;
				if (!descendsFrom(parent, widget))
					attach(widget, parent, style);
				break;
			}
			case 3: detach(widget); break;
			case 4: destroy(widget); break;
			case 5: create(_random() % 4 == 0 ? nullptr : widget, style, "Popup"); break;
			case 6:
				mOperation = "changeWidgetSkin";
				widget->changeWidgetSkin("Default");
				check();
				break;
			case 7: changeStyle(widget, style); break;
			}
			// Preserve the original stress app's input/lifetime regression exercise.
			MyGUI::InputManager::getInstance().injectMousePress(50, 50, MyGUI::MouseButton::Left);
			MyGUI::InputManager::getInstance().injectMouseRelease(50, 50, MyGUI::MouseButton::Left);
			check();
		}

	private:
		Expected& find(MyGUI::Widget* _widget)
		{
			const auto found = std::find_if(
				mExpected.begin(),
				mExpected.end(),
				[&](const Expected& _expected) { return _expected.widget == _widget; });
			require(found != mExpected.end(), "Widget missing from expected hierarchy");
			return *found;
		}

		bool descendsFrom(MyGUI::Widget* _widget, MyGUI::Widget* _ancestor)
		{
			for (auto* current = _widget; current != nullptr; current = find(current).parent)
			{
				if (current == _ancestor)
					return true;
			}
			return false;
		}

		void check()
		{
			require(mLive.size() == mExpected.size(), "Unexpected number of surviving widgets");
			for (const auto& expected : mExpected)
			{
				auto* widget = expected.widget;
				require(mLive.count(widget) == 1, "A surviving widget was unexpectedly destroyed");
				require(widget->getParent() == expected.parent, "Incorrect parent after attachment operation");
				require(widget->getWidgetStyle() == expected.style, "Incorrect widget style");
				const bool root = expected.parent == nullptr || expected.style == Style::Popup;
				require(widget->isRootWidget() == root, "Incorrect layer-root status");
				const auto* layerOwner = &expected;
				while (layerOwner->parent != nullptr && layerOwner->style != Style::Popup)
					layerOwner = &find(layerOwner->parent);
				auto* node = widget->getLayerNode();
				require(
					(node == nullptr) == layerOwner->layer.empty(),
					"Incorrect layer-node attachment after " + mOperation + ": expected layer '" + layerOwner->layer +
						"'");
				if (node != nullptr)
					require(node->getLayer()->getName() == layerOwner->layer, "Widget belongs to the wrong layer");
				require(
					(widget->getLayer() != nullptr) == (root && !expected.layer.empty()),
					"Incorrect direct layer ownership");
				if (!root && node != nullptr)
					require(
						(node == expected.parent->getLayerNode()) == (expected.style == Style::Child),
						"Child must share its parent's node; overlapped child must have its own");
				std::unordered_set<MyGUI::Widget*> children;
				for (const auto& candidate : mExpected)
				{
					if (candidate.parent == widget)
						children.insert(candidate.widget);
				}
				require(widget->getChildCount() == children.size(), "Incorrect child count");
				for (auto* child : widget->getChildWidgets())
					require(children.erase(child) == 1, "Unexpected or duplicate child");
				require(children.empty(), "Child missing from parent enumeration");
			}
		}

		std::string mOperation;
		std::vector<Expected> mExpected;
		std::unordered_set<MyGUI::Widget*> mLive;
	};
}

int main()
{
	unsigned int seed = 0;
	int step = -1;
	try
	{
		unittest::TestContext context;
		auto& layers = MyGUI::LayerManager::getInstance();
		layers.createLayerAt("Main", "OverlappedLayer", 0);
		layers.createLayerAt("Overlapped", "OverlappedLayer", 1);
		layers.createLayerAt("Popup", "OverlappedLayer", 2);
		{
			AttachmentTest test;
			test.scenarios();
		}
		for (const unsigned int currentSeed : {0u, 42u, 12345u})
		{
			seed = currentSeed;
			std::mt19937 random(seed);
			AttachmentTest test;
			for (step = 0; step < 2000; ++step)
				test.randomStep(random);
		}
	}
	catch (const std::exception& error)
	{
		std::cerr << "AttachDetach failed (seed " << seed << ", step " << step << "): " << error.what() << '\n';
		return 1;
	}
	return 0;
}
