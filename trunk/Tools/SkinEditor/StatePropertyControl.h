/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATE_PROPERTY_CONTROL_H__
#define __STATE_PROPERTY_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "Property.h"
#include "SkinItem.h"

namespace tools
{

	class StatePropertyControl :
		public wraps::BaseLayout
	{
	public:
		StatePropertyControl(MyGUI::Widget* _parent);
		virtual ~StatePropertyControl();

	private:
		void notifyChangeSelection();
		void notifyStateChangeSelection();
		void notifyChangeProperty(Property* _sender, const MyGUI::UString& _owner);
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyEditTextChange(MyGUI::Edit* _sender);

		void fillVisibleBox();

		void unadvice();
		void advice();
		void unadviceState();
		void adviceState();

		void updateAllProperties();

		void updateVisible();
		void updatePosition();

		size_t getComboIndex(const MyGUI::UString& _name);

		bool isPositionValidate();
		MyGUI::UString getClearPositionValue();
		void setColourPosition(bool _validate);

	private:
		MyGUI::ComboBox* mVisible;
		MyGUI::Edit* mPosition;
		SkinItem* mCurrentSkin;
		StateItem* mCurrentState;
		MyGUI::UString mTypeName;
	};

} // namespace tools

#endif // __STATE_PROPERTY_CONTROL_H__
