#include "Common.h"
#include "SelectionAreaControl.h"
#include "SettingsManager.h"
#include "EditorWidgets.h"
#include "UndoManager.h"
#include "WidgetSelectorManager.h"

namespace tools
{
	SelectionAreaControl::SelectionAreaControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SelectionAreaControl.layout", _parent),
		mWindow(nullptr),
		mGridStep(0),
		mCurrentWidget(nullptr),
		mArrowMove(false)
	{
		assignWidget(mWindow, "_Main");

		mWindow->eventWindowChangeCoord += newDelegate(this, &SelectionAreaControl::notifyRectangleResize);
		//mWindow->eventMouseButtonDoubleClick += newDelegate(mPanelItems, &PanelItems::notifyRectangleDoubleClick);
		//mWindow->eventKeyButtonPressed += newDelegate(this, &SelectionAreaControl::notifyRectangleKeyPressed);
		mWindow->eventMouseButtonClick += MyGUI::newDelegate(this, &SelectionAreaControl::notifyMouseButtonClick);

		mGridStep = SettingsManager::getInstance().getSector("SettingsWindow")->getPropertyValue<int>("Grid");
		notifyChangeSelectedWidget(nullptr);

		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &SelectionAreaControl::notifySettingsChanged);
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &SelectionAreaControl::notifyChangeSelectedWidget);
	}

	SelectionAreaControl::~SelectionAreaControl()
	{
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &SelectionAreaControl::notifyChangeSelectedWidget);
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &SelectionAreaControl::notifySettingsChanged);
	}

	void SelectionAreaControl::notifyRectangleResize(MyGUI::Window* _sender)
	{
		/*if (!_sender->getVisible())
			return;*/

		// найдем соответствующий контейнер виджета и переместим/растянем
		if (WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName())->resizeable)
		{
			MyGUI::IntCoord coord = utility::convertCoordToParentCoord(_sender->getCoord(), mCurrentWidget);
			MyGUI::IntCoord old_coord = mCurrentWidget->getCoord();
			// align to grid
			if (!MyGUI::InputManager::getInstance().isShiftPressed() && !mArrowMove)
			{
				if ((old_coord.width == coord.width) && (old_coord.height == coord.height)) // если только перемещаем
				{
					coord.left = toGrid(coord.left + mGridStep-1 - old_coord.left) + old_coord.left;
					coord.top = toGrid(coord.top + mGridStep-1 - old_coord.top) + old_coord.top;
				}
				else // если растягиваем
				{
					if (old_coord.left != coord.left)
					{
						coord.left = toGrid(coord.left + mGridStep-1);
						coord.width = old_coord.right() - coord.left;
					}
					else if (old_coord.width != coord.width)
					{
						coord.width = toGrid(coord.width + old_coord.left) - old_coord.left;
					}

					if (old_coord.top != coord.top)
					{
						coord.top = toGrid(coord.top + mGridStep-1);
						coord.height = old_coord.bottom() - coord.top;
					}
					else if (old_coord.height != coord.height)
					{
						coord.height = toGrid(coord.height + old_coord.top) - old_coord.top;
					}
				}
			}
			mArrowMove = false;

			mCurrentWidget->setCoord(coord);
			// update coord because of mCurrentWidget can have MinMax size
			coord = mCurrentWidget->getCoord();

			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			SettingsManager::getInstance().getSector("Widget")->setPropertyValue("Position", widgetContainer->position());
			//setPositionText(widgetContainer->position());

			UndoManager::getInstance().addValue(PR_POSITION);
		}

		mWindow->setCoord(mCurrentWidget->getAbsoluteCoord());
	}

	int SelectionAreaControl::toGrid(int _x)
	{
		if (mGridStep < 1)
			return _x;
		return _x / mGridStep * mGridStep;
	}

	void SelectionAreaControl::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
	{
		if (_sectorName == "SettingsWindow")
		{
			if (_propertyName == "Grid")
				mGridStep = SettingsManager::getInstance().getSector("SettingsWindow")->getPropertyValue<int>("Grid");
		}
		else if (_sectorName == "Widget")
		{
			if (_propertyName == "Position")
			{
				MyGUI::IntCoord coord = MyGUI::IntCoord::parse(SettingsManager::getInstance().getSector("Widget")->getPropertyValue("Position"));
				if (mMainWidget->getCoord() != coord)
					mMainWidget->setCoord(coord);
			}
		}
	}

	void SelectionAreaControl::notifyChangeSelectedWidget(MyGUI::Widget* _widget)
	{
		mCurrentWidget = _widget;

		mMainWidget->setVisible(mCurrentWidget != nullptr);

		if (mCurrentWidget != nullptr)
		{
			mWindow->setCoord(mCurrentWidget->getAbsoluteCoord());
		}
	}

	void SelectionAreaControl::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		int test = 0;
	}

} // namespace tools

