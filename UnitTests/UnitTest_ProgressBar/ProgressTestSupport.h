#pragma once

#include "BehaviourTestSupport.h"
#include "SkinTestContext.h"

namespace unittest
{

	class TestProgress : public MyGUI::ProgressBar
	{
		MYGUI_RTTI_DERIVED(TestProgress)
	public:
		MyGUI::Widget* trackPlace()
		{
			MyGUI::Widget* result = nullptr;
			assignWidget(result, "TrackPlace");
			return result;
		}
	};

	struct ProgressFixture
	{
		SkinTestContext context;
		TestProgress* bar;

		explicit ProgressFixture(bool _fill)
		{
			context.loadSkins();
			loadMedia("MyGUI_BlueWhiteTemplates.xml");
			createInputLayer();
			MyGUI::FactoryManager::getInstance().registerFactory<TestProgress>(
				MyGUI::WidgetManager::getInstance().getCategoryName());
			bar = MyGUI::Gui::getInstance().createWidget<TestProgress>(
				_fill ? "ProgressBarFill" : "ProgressBar",
				MyGUI::IntCoord(20, 20, 123, 35),
				MyGUI::Align::Default,
				"Main");
		}

		void checkGeometry()
		{
			require(bar->getProgressPosition() <= bar->getProgressRange(), "Progress must remain within its range");
			context.renderer().render();
			auto* place = bar->trackPlace();
			for (size_t i = 0; i < place->getChildCount(); ++i)
			{
				auto* track = place->getChildAt(i);
				if (!track->getVisible())
					continue;
				require(
					track->getWidth() >= 0 && track->getHeight() >= 0,
					"Visible progress tracks must have nonnegative size");
				require(track->getAlpha() >= 0 && track->getAlpha() <= 1, "Track alpha must be normalized");
			}
		}
	};

}
