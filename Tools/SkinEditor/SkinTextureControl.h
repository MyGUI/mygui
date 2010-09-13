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

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateSkinProperties();

		void notifyChangePosition();

		void updateTexture();
		void updateCoord();

		void updateRegionCoord();
		void updateFromCoordValue();
		bool checkCommand();

		void CommandMoveLeft(const MyGUI::UString& _commandName);
		void CommandMoveRight(const MyGUI::UString& _commandName);
		void CommandMoveTop(const MyGUI::UString& _commandName);
		void CommandMoveBottom(const MyGUI::UString& _commandName);
		void CommandSizeLeft(const MyGUI::UString& _commandName);
		void CommandSizeRight(const MyGUI::UString& _commandName);
		void CommandSizeTop(const MyGUI::UString& _commandName);
		void CommandSizeBottom(const MyGUI::UString& _commandName);

	private:
		AreaSelectorControl* mAreaSelectorControl;

		MyGUI::UString mTypeName;
		MyGUI::IntCoord mCoordValue;
	};

} // namespace tools

#endif // __SKIN_TEXTURE_CONTROL_H__
