#ifndef __WORKSPACE_CONTROL_H__
#define __WORKSPACE_CONTROL_H__

#include "TextureToolControl.h"
#include "PropertiesPanelView.h"
#include "WidgetsWindow.h"
#include "AreaSelectorControl.h"

namespace tools
{

	class WorkspaceControl :
		public TextureToolControl
	{
	public:
		WorkspaceControl(MyGUI::Widget* _parent = nullptr);
		virtual ~WorkspaceControl();

	protected:
		virtual void onMouseButtonClick(const MyGUI::IntPoint& _point);
		virtual void onMouseMove();

	private:
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);
		void notifyChangePosition();
		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);
		void notifyPropertyChangeCoord(const MyGUI::IntCoord& _coordValue);

		void updateFromCoordValue();
		void updateSelectionFromValue();
		bool checkCommand();

		int toGrid(int _value);

		void CommandMoveLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveBottom(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveBottom(const MyGUI::UString& _commandName, bool& _result);

		void CommandSizeLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandSizeRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandSizeTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandSizeBottom(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeBottom(const MyGUI::UString& _commandName, bool& _result);

		void selectWidget(const MyGUI::IntPoint& _mousePosition);
		MyGUI::Widget* getTopWidget(const MyGUI::IntPoint& _point);
		bool checkContainer(WidgetContainer* _container, MyGUI::Widget*& _result, const MyGUI::IntPoint& _point);

	private:
		AreaSelectorControl* mAreaSelectorControl;
		MyGUI::IntCoord mCoordValue;
		int mGridStep;
		MyGUI::Widget* mCurrentWidget;
		size_t mSelectDepth;
		MyGUI::IntPoint mLastClickPoint;
	};

} // namespace tools

#endif // __WORKSPACE_CONTROL_H__
