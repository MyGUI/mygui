/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _75ac36ab_9bfe_4e08_bcc0_8f0ad272ac6c_
#define _75ac36ab_9bfe_4e08_bcc0_8f0ad272ac6c_

#include "Control.h"
#include "Data.h"
#include "Property.h"
#include "AnimationInfo.h"
#include "sigslot.h"

namespace tools
{

	class AnimationViewControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		AnimationViewControl();
		virtual ~AnimationViewControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyChangeDataSelector(DataPtr _parent, bool _changeSelectOnly);
		void notifyChangeProperty(PropertyPtr _sender);
		void notifyChangeCoord(MyGUI::Widget* _sender);
		void notifyFrameStart(float _frame);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

		void connectToProperties();
		void rebuildAnimations();
		void updateImageCoord();
		void updateFrame();
		void updateSelectedFrame();

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(AnimationViewControl, mImage, "Image");
		MyGUI::ImageBox* mImage;

		ATTRIBUTE_FIELD_WIDGET_NAME(AnimationViewControl, mFrameInfo, "Info");
		MyGUI::TextBox* mFrameInfo;

		ATTRIBUTE_FIELD_WIDGET_NAME(AnimationViewControl, mButtonPlay, "Play");
		MyGUI::Button* mButtonPlay;

		ATTRIBUTE_FIELD_WIDGET_NAME(AnimationViewControl, mButtonLeft, "Left");
		MyGUI::Button* mButtonLeft;

		ATTRIBUTE_FIELD_WIDGET_NAME(AnimationViewControl, mButtonRight, "Right");
		MyGUI::Button* mButtonRight;

		DataPtr mParentData;
		AnimationInfo mAnimation;
		size_t mCurrentFrame;
		float mTime;
		bool mPlay;
	};

}

#endif
