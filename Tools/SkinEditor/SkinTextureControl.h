/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_TEXTURE_CONTROL_H__
#define __SKIN_TEXTURE_CONTROL_H__

#include "TextureToolControl.h"
#include "Property.h"
#include "SkinItem.h"
#include "AreaSelectorControl.h"
#include "PropertyAdvisor.h"

namespace tools
{
	class SkinTextureControl :
		public TextureToolControl,
		public PropertyAdvisor
	{
	public:
		SkinTextureControl(MyGUI::Widget* _parent);
		virtual ~SkinTextureControl();

	protected:
		virtual void onMouseButtonClick(const MyGUI::IntPoint& _point);

	private:
		void notifyChangeSelection();
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateSkinProperties();

		void notifyChangePosition();

		void updateTexture();
		void updateCoord();

		void updateFromCoordValue();
		bool checkCommand();

		int toGrid(int _value);

		void CommandMoveLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveBottom(const MyGUI::UString& _commandName, bool& _result);
		void CommandSizeLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandSizeRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandSizeTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandSizeBottom(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveBottom(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeBottom(const MyGUI::UString& _commandName, bool& _result);

	private:
		AreaSelectorControl* mAreaSelectorControl;

		MyGUI::UString mTypeName;
		MyGUI::IntCoord mCoordValue;
		int mGridStep;
	};

} // namespace tools

#endif // __SKIN_TEXTURE_CONTROL_H__
