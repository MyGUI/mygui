/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATE_TEXTURE_CONTROL_H__
#define __STATE_TEXTURE_CONTROL_H__

#include "TextureToolControl.h"
#include "Property.h"
#include "SkinItem.h"
#include "PositionSelectorControl.h"
#include "PropertyAdvisor.h"
#include "PositionSelectorBlackControl.h"

namespace tools
{
	class StateTextureControl :
		public TextureToolControl,
		public PropertyAdvisor
	{
	public:
		StateTextureControl(MyGUI::Widget* _parent);
		virtual ~StateTextureControl();

	protected:
		void onMouseButtonClick(const MyGUI::IntPoint& _point);

	private:
		void notifyChangePosition();
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

		virtual void updateSkinProperties();
		virtual void updateStateProperties();

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateStateProperty(Property* _sender, const MyGUI::UString& _owner);

		void updateTexture();
		void updateCoord();

		void updateVisible();
		void updatePosition();

		void updateSelectorsSize();

		void updateUnselectedStates();
		void addCoord(std::vector<MyGUI::IntCoord>& _coords, const MyGUI::UString& _coord, const MyGUI::UString& _position);
		void drawUnselectedStates(std::vector<MyGUI::IntCoord>& _coords);

		bool checkCommand();

		void updateFromPointValue();
		int toGrid(int _value);

		void CommandMoveLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveBottom(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveBottom(const MyGUI::UString& _commandName, bool& _result);

	private:
		PositionSelectorControl* mAreaSelectorControl;

		MyGUI::UString mTypeName;
		MyGUI::IntSize mSizeValue;

		std::vector<PositionSelectorBlackControl*> mBlackSelectors;
		int mGridStep;
		MyGUI::IntPoint mPointValue;
	};

} // namespace tools

#endif // __STATE_TEXTURE_CONTROL_H__
