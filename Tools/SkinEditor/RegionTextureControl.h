/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __REGION_TEXTURE_CONTROL_H__
#define __REGION_TEXTURE_CONTROL_H__

#include "TextureToolControl.h"
#include "PropertyAdvisor.h"
#include "AreaSelectorControl.h"
#include "PositionSelectorControl.h"
#include "PositionSelectorBlackControl.h"

namespace tools
{
	class RegionTextureControl :
		public TextureToolControl,
		public PropertyAdvisor
	{
	public:
		RegionTextureControl(MyGUI::Widget* _parent);
		virtual ~RegionTextureControl();

	protected:
		virtual void updateSkinProperties();
		virtual void updateStateProperties();
		virtual void updateRegionProperties();
		virtual void updateSeparatorProperties();

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateStateProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateRegionProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner);

		virtual void onChangeScale();
		virtual void onChangeActivate();

	private:
		void notifyChangePosition();

		void onMouseButtonClick(const MyGUI::IntPoint& _point);

		void updateTextureControl();
		void updateTextureName();
		void updateRegionSize();
		void updateRegionPosition();
		void updateTextureVisible();
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

		void updatePosition();
		void updateVisible();

		void updateUnselectedStates();
		void addCoord(std::vector<MyGUI::IntCoord>& _coords, const MyGUI::UString& _coord);
		void drawUnselectedStates(std::vector<MyGUI::IntCoord>& _coords);

		void updateFromCoordValue();

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

		void updateCaption();

	private:
		MyGUI::UString mTextureName;
		MyGUI::IntCoord mTextureRegion;
		bool mTextureVisible;
		MyGUI::UString mTypeName;

		AreaSelectorControl* mAreaSelectorControl;
		PositionSelectorControl* mPositionSelectorControl;

		std::vector<PositionSelectorBlackControl*> mBlackSelectors;
		MyGUI::IntCoord mCoordValue;
		int mGridStep;
	};

} // namespace tools

#endif // __REGION_TEXTURE_CONTROL_H__
