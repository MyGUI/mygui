/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SEPARATOR_TEXTURE_CONTROL_H__
#define __SEPARATOR_TEXTURE_CONTROL_H__

#include "TextureControl.h"
#include "PropertyAdvisor.h"
#include "HorizontalSelectorControl.h"
#include "VerticalSelectorControl.h"
#include "HorizontalSelectorBlackControl.h"
#include "VerticalSelectorBlackControl.h"

namespace tools
{

	class SeparatorTextureControl :
		public TextureControl,
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

	private:
		void notifyChangePosition();

		void updateTextureControl();
		void updateTextureName();
		void updateRegionSize();
		void updateRegionPosition();
		void updateTextureVisible();

		void updatePosition();
		void updateVisible();

		void updateUnselectedStates();
		void addCoord(std::vector<int>& _coordsHor, std::vector<int>& _coordsVert, bool _horizont, const MyGUI::UString& _position);
		void drawUnselectedStates(std::vector<int>& _coordsHor, std::vector<int>& _coordsVert);

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
	};

} // namespace tools

#endif // __SEPARATOR_TEXTURE_CONTROL_H__
