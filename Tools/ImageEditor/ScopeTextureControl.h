/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _31513eda_9f1b_4986_a5a5_466942db581b_
#define _31513eda_9f1b_4986_a5a5_466942db581b_

#include "TextureToolControl.h"
#include "AreaSelectorControl.h"
#include "Property.h"

namespace tools
{

	class ScopeTextureControl :
		public TextureToolControl,
		public sigslot::has_slots<>
	{
	public:
		ScopeTextureControl(MyGUI::Widget* _parent);
		virtual ~ScopeTextureControl();

	protected:
		virtual void onMouseButtonClick(const MyGUI::IntPoint& _point);

		virtual void onChangeScale();
		virtual void onChangeActivate();

	private:
		void notifyChangeSelection();
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyChangeDataSelector(Data* _data, bool _changeOnlySelection);
		void notifyChangeProperty(Property* _sender);
		void notifyChangePosition(SelectorControl* _sender);

		void updateCoord(const std::string& _value);
		void updateFromCoordValue();

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

		void updateCaption();

		void setValue(const std::string& _value);

	private:
		AreaSelectorControl* mAreaSelectorControl;

		MyGUI::IntCoord mCoordValue;

		Data* mParentData;
		std::string mParentTypeName;
	};

}

#endif
