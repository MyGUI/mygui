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
		virtual void onMouseMove();
		virtual void onMouseButtonPressed(const MyGUI::IntPoint& _point);
		virtual void onMouseButtonReleased(const MyGUI::IntPoint& _point);
		virtual void onMouseButtonClick(const MyGUI::IntPoint& _point);
		virtual void onMouseDrag(const MyGUI::IntPoint& _point);

	private:
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);
		void notifyChangePosition();
		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);
		void notifyPropertyChangeCoord(const MyGUI::IntCoord& _coordValue);
		void notifyChangeCreatorMode(bool _createMode);

		void updateFromCoordValue();
		void updateSelectionFromValue();
		bool checkCommand();

		int toGrid(int _value);
		void updateSelectorEnabled();

		void Command_MoveLeft(const MyGUI::UString& _commandName, bool& _result);
		void Command_MoveRight(const MyGUI::UString& _commandName, bool& _result);
		void Command_MoveTop(const MyGUI::UString& _commandName, bool& _result);
		void Command_MoveBottom(const MyGUI::UString& _commandName, bool& _result);
		void Command_GridMoveLeft(const MyGUI::UString& _commandName, bool& _result);
		void Command_GridMoveRight(const MyGUI::UString& _commandName, bool& _result);
		void Command_GridMoveTop(const MyGUI::UString& _commandName, bool& _result);
		void Command_GridMoveBottom(const MyGUI::UString& _commandName, bool& _result);

		void Command_SizeLeft(const MyGUI::UString& _commandName, bool& _result);
		void Command_SizeRight(const MyGUI::UString& _commandName, bool& _result);
		void Command_SizeTop(const MyGUI::UString& _commandName, bool& _result);
		void Command_SizeBottom(const MyGUI::UString& _commandName, bool& _result);
		void Command_GridSizeLeft(const MyGUI::UString& _commandName, bool& _result);
		void Command_GridSizeRight(const MyGUI::UString& _commandName, bool& _result);
		void Command_GridSizeTop(const MyGUI::UString& _commandName, bool& _result);
		void Command_GridSizeBottom(const MyGUI::UString& _commandName, bool& _result);

		void Command_Delete(const MyGUI::UString& _commandName, bool& _result);
		void Command_NextItem(const MyGUI::UString& _commandName, bool& _result);

	private:
		AreaSelectorControl* mAreaSelectorControl;
		MyGUI::IntCoord mCoordValue;
		int mGridStep;
		MyGUI::Widget* mCurrentWidget;
		bool mMoveableWidget;
	};

} // namespace tools

#endif // __WORKSPACE_CONTROL_H__
