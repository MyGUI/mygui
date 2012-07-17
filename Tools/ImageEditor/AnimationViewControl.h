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
		void notifyChangeDataSelector(Data* _parent, bool _changeSelectOnly);
		void notifyChangeProperty(Property* _sender);
		void notifyChangeCoord(MyGUI::Widget* _sender);
		void notifyFrameStart(float _frame);

		void connectToProperties();
		void rebuildAnimations();
		void updateImageCoord();

	private:
		MyGUI::ImageBox* mImage;
		Data* mParentData;
		AnimationInfo mAnimation;
		size_t mMaxCountFrame;
		size_t mCurrentFrame;
		float mTime;
	};

}

#endif
