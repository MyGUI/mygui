#ifndef _ccbbea0f_9b0d_4e19_9404_4b91b70a6477_
#define _ccbbea0f_9b0d_4e19_9404_4b91b70a6477_

#include "TextureToolControl.h"
#include "PropertiesPanelView.h"
#include "WidgetsWindow.h"
#include "AreaSelectorControl.h"
#include "PositionSelectorControl.h"

namespace tools
{

	class WorkspaceControl : public TextureToolControlLE
	{
	public:
		WorkspaceControl(MyGUI::Widget* _parent = nullptr);
		~WorkspaceControl() override;

	protected:
		void onMouseMove() override;
		void onMouseButtonPressed(const MyGUI::IntPoint& _point) override;
		void onMouseButtonReleased(const MyGUI::IntPoint& _point) override;
		void onMouseButtonClick(const MyGUI::IntPoint& _point) override;
		void onMouseDrag(const MyGUI::IntPoint& _point) override;

		void onChangeScale() override;

	private:
		void notifyChangePosition();
		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);
		void notifyPropertyChangeCoord(
			MyGUI::Widget* _widget,
			const MyGUI::IntCoord& _coordValue,
			std::string_view _owner);
		void notifyChangeCreatorMode(bool _createMode);
		void notifyChangeSelectorCreator(bool _visible, const MyGUI::IntCoord& _coord);
		void notifyFrameStart(float _time);

		void updateFromCoordValue();
		void updateSelectionFromValue();

		void updateSelectorEnabled();

		void updateCaption();
		void updateMenu();

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

		void Command_FreeChildMode(const MyGUI::UString& _commandName, bool& _result);

		void setWidgetCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _coord) const;

		void setRttLayerSize(const MyGUI::IntSize& _size);

	private:
		AreaSelectorControlLE* mAreaSelectorControl{nullptr};
		MyGUI::IntCoord mCoordValue;
		MyGUI::Widget* mCurrentWidget{nullptr};
		bool mMoveableWidget{false};
		PositionSelectorControlLE* mPositionSelectorCreatorControl{nullptr};
		bool mFreeChildMode{false};
	};

}

#endif
