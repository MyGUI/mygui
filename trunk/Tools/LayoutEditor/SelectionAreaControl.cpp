#include "Common.h"
#include "SelectionAreaControl.h"
#include "PropertiesPanelView.h"
#include "SettingsManager.h"
#include "WidgetSelectorManager.h"
#include "UndoManager.h"

//FIXME
template <> tools::SelectionAreaControl* MyGUI::Singleton<tools::SelectionAreaControl>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::SelectionAreaControl>::mClassTypeName("SelectionAreaControl");

namespace tools
{
	SelectionAreaControl::SelectionAreaControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SelectionAreaControl.layout", _parent),
		mCurrentWidgetRectangle(nullptr),
		mCurrentWidget(nullptr),
		mGridStep(0),
		mArrowMove(false)
	{
		assignWidget(mCurrentWidgetRectangle, "_Main");

		mCurrentWidgetRectangle->eventWindowChangeCoord += newDelegate(this, &SelectionAreaControl::notifyRectangleResize);
		mCurrentWidgetRectangle->eventKeyButtonPressed += newDelegate(this, &SelectionAreaControl::notifyRectangleKeyPressed);

		mGridStep = SettingsManager::getInstance().getSector("SettingsWindow")->getPropertyValue<int>("Grid");

		notifyChangeSelectedWidget(nullptr);

		PropertiesPanelView::getInstance().eventChangeCoord += MyGUI::newDelegate(this, &SelectionAreaControl::notifyChangeCoord);
		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &SelectionAreaControl::notifySettingsChanged);
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &SelectionAreaControl::notifyChangeSelectedWidget);
	}

	SelectionAreaControl::~SelectionAreaControl()
	{
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &SelectionAreaControl::notifyChangeSelectedWidget);
		PropertiesPanelView::getInstance().eventChangeCoord -= MyGUI::newDelegate(this, &SelectionAreaControl::notifyChangeCoord);
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &SelectionAreaControl::notifySettingsChanged);
	}

	void SelectionAreaControl::notifyChangeCoord(const MyGUI::IntCoord& _coord)
	{
		mCurrentWidgetRectangle->setCoord(_coord);
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
	}

	MyGUI::Window* SelectionAreaControl::getWidgetRectangle()
	{
		return mCurrentWidgetRectangle;
	}

	void SelectionAreaControl::notifyRectangleResize(MyGUI::Window* _sender)
	{
		if (!_sender->getVisible())
			return;

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

			PropertiesPanelView::getInstance().setCoord(coord);

			UndoManager::getInstance().addValue(PR_POSITION);
		}
		mCurrentWidgetRectangle->setCoord(mCurrentWidget->getAbsoluteCoord());
	}

	void SelectionAreaControl::notifyRectangleKeyPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
	{
		MyGUI::IntPoint delta;
		int k = MyGUI::InputManager::getInstance().isShiftPressed() ? 1 : mGridStep;
		if (MyGUI::KeyCode::Tab == _key)
		{
			if ((nullptr != mCurrentWidget) && (nullptr != mCurrentWidget->getParent()) && (mCurrentWidget->getParent()->getTypeName() == "Tab"))
				notifyChangeSelectedWidget(mCurrentWidget->getParent());
			if (mCurrentWidget && mCurrentWidget->getTypeName() == "Tab")
			{
				MyGUI::Tab* tab = mCurrentWidget->castType<MyGUI::Tab>();
				size_t sheet = tab->getIndexSelected();
				sheet++;
				if (sheet >= tab->getItemCount()) sheet = 0;
				if (tab->getItemCount()) tab->setIndexSelected(sheet);
			}
		}
		else if (MyGUI::KeyCode::Delete == _key)
		{
			if (mCurrentWidget)
			{
				EditorWidgets::getInstance().remove(mCurrentWidget);
				WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
				UndoManager::getInstance().addValue();
			}
		}
		else if (MyGUI::KeyCode::ArrowLeft == _key)
		{
			delta = MyGUI::IntPoint(-k, 0);
		}
		else if (MyGUI::KeyCode::ArrowRight == _key)
		{
			delta = MyGUI::IntPoint(k, 0);
		}
		else if (MyGUI::KeyCode::ArrowUp == _key)
		{
			delta = MyGUI::IntPoint(0, -k);
		}
		else if (MyGUI::KeyCode::ArrowDown == _key)
		{
			delta = MyGUI::IntPoint(0, k);
		}

		if (delta != MyGUI::IntPoint())
		{
			mArrowMove = true;
			mCurrentWidgetRectangle->setPosition(mCurrentWidgetRectangle->getPosition() + delta);
			notifyRectangleResize(mCurrentWidgetRectangle);
			UndoManager::getInstance().addValue(PR_KEY_POSITION);
		}
	}

	void SelectionAreaControl::notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;

		mCurrentWidgetRectangle->setVisible(mCurrentWidget != nullptr);
	}

} // namespace tools

