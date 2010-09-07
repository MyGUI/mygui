#include "Common.h"
#include "SelectionAreaControl.h"
#include "PropertiesPanelView.h"
#include "SettingsManager.h"
#include "WidgetSelectorManager.h"
#include "UndoManager.h"
#include "WidgetsWindow.h"

namespace tools
{
	SelectionAreaControl::SelectionAreaControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SelectionAreaControl.layout", _parent),
		mWindow(nullptr),
		mCurrentWidget(nullptr),
		mGridStep(0),
		mArrowMove(false),
		mSelectDepth(0),
		mMouseButtonPressed(false),
		mLastClickX(0),
		mLastClickY(0)
	{
		assignWidget(mWindow, "_Main");

		mWindow->eventWindowChangeCoord += newDelegate(this, &SelectionAreaControl::notifyRectangleResize);
		mWindow->eventKeyButtonPressed += newDelegate(this, &SelectionAreaControl::notifyRectangleKeyPressed);

		mWindow->eventMouseButtonPressed += newDelegate(this, &SelectionAreaControl::notifyMouseButtonPressed);
		mWindow->eventMouseButtonReleased += newDelegate(this, &SelectionAreaControl::notifyMouseButtonReleased);
		mWindow->eventMouseMove += newDelegate(this, &SelectionAreaControl::notifyMouseMouseMove);
		mWindow->eventMouseDrag += newDelegate(this, &SelectionAreaControl::notifyMouseMouseDrag);

		mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");

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
		mWindow->setCoord(_coord);
	}

	int SelectionAreaControl::toGrid(int _x)
	{
		if (mGridStep < 1)
			return _x;
		return _x / mGridStep * mGridStep;
	}

	void SelectionAreaControl::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
	{
		if (_sectorName == "Settings")
		{
			if (_propertyName == "Grid")
				mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		}
	}

	void SelectionAreaControl::notifyRectangleResize(MyGUI::Window* _sender)
	{
		if (WidgetsWindow::getInstance().getCreateStatus() == 2 || WidgetsWindow::getInstance().getCreateStatus() == 1)
		{
			if (mWindow->getVisible())
				mWindow->setVisible(false);
			return;
		}

		if (WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName())->resizeable)
		{
			MyGUI::IntCoord coord = utility::convertCoordToParentCoord(_sender->getCoord(), mCurrentWidget);
			MyGUI::IntCoord old_coord = mCurrentWidget->getCoord();
			// align to grid
			if (!MyGUI::InputManager::getInstance().isShiftPressed() && !mArrowMove)
			{
				if ((old_coord.width == coord.width) && (old_coord.height == coord.height)) // если только перемещаем
				{
					coord.left = toGrid(coord.left + mGridStep - 1 - old_coord.left) + old_coord.left;
					coord.top = toGrid(coord.top + mGridStep - 1 - old_coord.top) + old_coord.top;
				}
				else // если раст€гиваем
				{
					if (old_coord.left != coord.left)
					{
						coord.left = toGrid(coord.left + mGridStep - 1);
						coord.width = old_coord.right() - coord.left;
					}
					else if (old_coord.width != coord.width)
					{
						coord.width = toGrid(coord.width + old_coord.left) - old_coord.left;
					}

					if (old_coord.top != coord.top)
					{
						coord.top = toGrid(coord.top + mGridStep - 1);
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
		mWindow->setCoord(mCurrentWidget->getAbsoluteCoord());
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

				WidgetSelectorManager::getInstance().setSelectedWidget(tab->getItemSelected());
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
			mWindow->setPosition(mWindow->getPosition() + delta);
			notifyRectangleResize(mWindow);
			UndoManager::getInstance().addValue(PR_KEY_POSITION);
		}
	}

	void SelectionAreaControl::notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;

		mWindow->setVisible(mCurrentWidget != nullptr);

		if (mCurrentWidget == nullptr)
		{
			mSelectDepth = 0;
		}
	}

	void SelectionAreaControl::notifyMouseMouseMove(MyGUI::Widget* _sender, int _left, int _top)
	{
		mMouseButtonPressed = false;

		const int distance = 2;
		if ((abs(mLastClickX - _left) > distance) || (abs(mLastClickY - _top) > distance))
		{
			mSelectDepth = 0;
		}
	}

	void SelectionAreaControl::notifyMouseMouseDrag(MyGUI::Widget* _sender, int _left, int _top)
	{
		mMouseButtonPressed = false;
	}

	void SelectionAreaControl::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			mLastClickX = _left;
			mLastClickY = _top;
			mMouseButtonPressed = true;
		}
	}

	void SelectionAreaControl::notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			if (mMouseButtonPressed)
			{
				mMouseButtonPressed = false;

				// здесь кликать вглубь
				MyGUI::Widget* item = getTopWidget(MyGUI::InputManager::getInstance().getLastLeftPressed());
				if (nullptr != item)
				{
					// find widget registered as container
					while ((nullptr == EditorWidgets::getInstance().find(item)) && (nullptr != item))
						item = item->getParent();
					MyGUI::Widget* oldItem = item;

					// try to selectin depth
					int depth = mSelectDepth;
					while (depth && (nullptr != item))
					{
						item = item->getParent();
						while ((nullptr == EditorWidgets::getInstance().find(item)) && (nullptr != item))
							item = item->getParent();
						depth--;
					}
					if (nullptr == item)
					{
						item = oldItem;
						mSelectDepth = 0;
					}

					// found widget
					if (nullptr != item)
					{
						WidgetSelectorManager::getInstance().setSelectedWidget(item);
						mSelectDepth++;
					}
				}
			}
		}
	}

	MyGUI::Widget* SelectionAreaControl::getTopWidget(const MyGUI::IntPoint& _point)
	{
		MyGUI::Widget* result = nullptr;

		EnumeratorWidgetContainer container = EditorWidgets::getInstance().getWidgets();
		while (container.next())
		{
			if (checkContainer(container.current(), result, _point))
				break;
		}

		return result;
	}

	bool SelectionAreaControl::checkContainer(WidgetContainer* _container, MyGUI::Widget*& _result, const MyGUI::IntPoint& _point)
	{
		if (_container->widget->getAbsoluteCoord().inside(_point))
		{
			_result = _container->widget;

			for (std::vector<WidgetContainer*>::iterator item = _container->childContainers.begin(); item != _container->childContainers.end(); ++item)
			{
				if (checkContainer(*item, _result, _point))
					break;
			}

			return true;
		}
		return false;
	}

} // namespace tools
