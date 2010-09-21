/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SEPARATOR_TEXTURE_CONTROL_H__
#define __SEPARATOR_TEXTURE_CONTROL_H__

#include "TextureToolControl.h"
#include "PropertyAdvisor.h"
#include "HorizontalSelectorControl.h"
#include "VerticalSelectorControl.h"
#include "HorizontalSelectorBlackControl.h"
#include "VerticalSelectorBlackControl.h"

namespace tools
{
	class SeparatorTextureControl :
		public TextureToolControl,
		public PropertyAdvisor
	{
	public:
		SeparatorTextureControl(MyGUI::Widget* _parent);
		virtual ~SeparatorTextureControl();

	protected:
		virtual void updateSkinProperties();
		virtual void updateStateProperties();
		virtual void updateSeparatorProperties();

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateStateProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner);

		void onMouseButtonClick(const MyGUI::IntPoint& _point);

	private:
		void notifyChangePosition();
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

		void updateTextureControl();
		void updateTextureName();
		void updateRegionSize();
		void updateRegionPosition();
		void updateTextureVisible();
		void updateFromPointValue();

		void updatePosition();
		void updateVisible();

		void updateUnselectedStates();
		void addCoord(std::vector<int>& _coordsHor, std::vector<int>& _coordsVert, MyGUI::Align _corner, const MyGUI::UString& _position);
		void drawUnselectedStates(std::vector<int>& _coordsHor, std::vector<int>& _coordsVert);

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

		MyGUI::Align getCorner();

	private:
		MyGUI::UString mTextureName;
		MyGUI::IntCoord mTextureRegion;
		bool mTextureVisible;
		MyGUI::UString mTypeName;

		HorizontalSelectorControl* mHorizontalSelectorControl;
		VerticalSelectorControl* mVerticalSelectorControl;
		bool mHorizontal;

		std::vector<HorizontalSelectorBlackControl*> mHorizontalBlackSelectors;
		std::vector<VerticalSelectorBlackControl*> mVerticalBlackSelectors;

		int mGridStep;
		int mValue;
	};

} // namespace tools

#endif // __SEPARATOR_TEXTURE_CONTROL_H__
