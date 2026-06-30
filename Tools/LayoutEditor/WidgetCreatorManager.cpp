/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "WidgetCreatorManager.h"
#include "WidgetSelectorManager.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"
#include "GridManager.h"

namespace tools
{
	MYGUI_SINGLETON_DEFINITION(WidgetCreatorManager);

	WidgetCreatorManager::WidgetCreatorManager() :
		mSingletonHolder(this)
	{
	}

	void WidgetCreatorManager::initialise()
	{
	}

	void WidgetCreatorManager::shutdown()
	{
		resetWidget();
	}

	void WidgetCreatorManager::setCreatorInfo(std::string_view _widgetType, std::string_view _widgetSkin)
	{
		mWidgetType = _widgetType;
		mWidgetSkin = _widgetSkin;
		mCreateMode = true;

		eventChangeCreatorMode(mCreateMode);
	}

	void WidgetCreatorManager::resetCreatorInfo()
	{
		mWidgetType.clear();
		mWidgetSkin.clear();
		mCreateMode = false;

		eventChangeCreatorMode(mCreateMode);
	}

	void WidgetCreatorManager::resetAllCreatorInfo()
	{
		mWidgetType.clear();
		mWidgetSkin.clear();
		mCreateMode = false;
		mPopupMode = false;

		eventChangeCreatorMode(mCreateMode);
	}

	bool WidgetCreatorManager::getCreateMode() const
	{
		return mCreateMode;
	}

	const std::string& WidgetCreatorManager::getWidgetType() const
	{
		return mWidgetType;
	}

	const std::string& WidgetCreatorManager::getWidgetSkin() const
	{
		return mWidgetSkin;
	}

	void WidgetCreatorManager::createNewWidget(const MyGUI::IntPoint& _point)
	{
		mStartNewWidget = true;
		mStartPoint = _point;

		resetWidget();

		eventChangeSelector(false, MyGUI::IntCoord());
	}

	void WidgetCreatorManager::moveNewWidget(const MyGUI::IntPoint& _point)
	{
		if (mNewWidget == nullptr)
		{
			// widget type may be missing
			if (!MyGUI::WidgetManager::getInstance().isFactoryExist(mWidgetType))
				return;

			// select top widget
			if (!mPopupMode)
				WidgetSelectorManager::getInstance().selectWidget(mStartPoint);

			MyGUI::Widget* parent = WidgetSelectorManager::getInstance().getSelectedWidget();

			// until we find the nearest above us capable of being a parent
			while (parent != nullptr && !WidgetTypes::getInstance().findWidgetStyle(parent->getTypeName())->parent)
				parent = parent->getParent();

			if (!WidgetTypes::getInstance().findWidgetStyle(mWidgetType)->child)
				parent = nullptr;

			if (parent != nullptr)
				mNewWidget = parent->createWidgetT(
					mPopupMode ? MyGUI::WidgetStyle::Popup : MyGUI::WidgetStyle::Child,
					mWidgetType,
					EditorWidgets::getInstance().getSkinReplace(mWidgetSkin),
					MyGUI::IntCoord(),
					MyGUI::Align::Default,
					DEFAULT_EDITOR_LAYER);
			else
				mNewWidget = MyGUI::Gui::getInstance().createWidgetT(
					mWidgetType,
					EditorWidgets::getInstance().getSkinReplace(mWidgetSkin),
					MyGUI::IntCoord(),
					MyGUI::Align::Default,
					DEFAULT_EDITOR_LAYER);

			// convert start point to parent coordinates
			if (parent != nullptr && !mNewWidget->isRootWidget())
			{
				if (parent->getClientWidget())
					mStartPoint -= parent->getClientWidget()->getAbsolutePosition();
				else
					mStartPoint -= parent->getAbsolutePosition();
			}

			if (!MyGUI::InputManager::getInstance().isShiftPressed())
			{
				mStartPoint.left = GridManager::getInstance().toGrid(mStartPoint.left);
				mStartPoint.top = GridManager::getInstance().toGrid(mStartPoint.top);
			}
		}

		MyGUI::IntCoord coord = getCoordNewWidget(_point);
		mNewWidget->setCoord(coord);

		eventChangeSelector(true, mNewWidget->getAbsoluteCoord());
	}

	void WidgetCreatorManager::finishNewWidget(const MyGUI::IntPoint& _point)
	{
		if (mNewWidget != nullptr)
		{
			MyGUI::IntCoord coord = getCoordNewWidget(_point);

			if (coord.width != 0 && coord.height != 0)
			{
				mNewWidget->setCoord(coord);

				// widget created, everyone is happy
				WidgetContainer* widgetContainer = new WidgetContainer(mWidgetType, mWidgetSkin, mNewWidget);
				if (mPopupMode)
					widgetContainer->setStyle(mNewWidget->getWidgetStyle().print());
				mNewWidget = nullptr;

				EditorWidgets::getInstance().add(widgetContainer);
				UndoManager::getInstance().addValue();

				// to select the created widget
				resetAllCreatorInfo();

				WidgetSelectorManager::getInstance().setSelectedWidget(widgetContainer->getWidget());
			}
			else
			{
				// failed to create, because size is zero
				resetWidget();
			}
		}

		resetAllCreatorInfo();

		eventChangeSelector(false, MyGUI::IntCoord());
	}

	void WidgetCreatorManager::resetWidget()
	{
		// just in case
		if (mNewWidget != nullptr)
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(mNewWidget);
			mNewWidget = nullptr;
		}
	}

	MyGUI::IntCoord WidgetCreatorManager::getCoordNewWidget(const MyGUI::IntPoint& _point)
	{
		MyGUI::IntPoint point = _point;

		MyGUI::Widget* parent = mNewWidget->getParent();
		if (parent != nullptr && !mNewWidget->isRootWidget())
			point -= parent->getAbsolutePosition();

		if (!MyGUI::InputManager::getInstance().isShiftPressed())
		{
			point.left = GridManager::getInstance().toGrid(point.left);
			point.top = GridManager::getInstance().toGrid(point.top);
		}

		MyGUI::IntCoord coord = MyGUI::IntCoord(
			(std::min)(mStartPoint.left, point.left),
			(std::min)(mStartPoint.top, point.top),
			abs(point.left - mStartPoint.left),
			abs(point.top - mStartPoint.top));

		return coord;
	}

	void WidgetCreatorManager::setPopupMode(bool _value)
	{
		mPopupMode = _value;
		mCreateMode = true;

		eventChangeCreatorMode(mCreateMode);
	}

	bool WidgetCreatorManager::getPopupMode() const
	{
		return mPopupMode;
	}

}
