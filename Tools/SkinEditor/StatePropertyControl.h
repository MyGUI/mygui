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
#include "PropertyAdvisor.h"

namespace tools
{

	class StatePropertyControl :
		public wraps::BaseLayout,
		public PropertyAdvisor
	{
	public:
		StatePropertyControl(MyGUI::Widget* _parent);
		virtual ~StatePropertyControl();

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyEditTextChange(MyGUI::Edit* _sender);

		virtual void updateStateProperties();
		virtual void updateStateProperty(Property* _sender, const MyGUI::UString& _owner);

		void fillVisibleBox();

		void updateVisible();
		void updatePosition();

		size_t getComboIndex(const MyGUI::UString& _name);

		bool isPositionValidate();
		MyGUI::UString getClearPositionValue();
		void setColourPosition(bool _validate);

	private:
		MyGUI::ComboBox* mVisible;
		MyGUI::Edit* mPosition;
		MyGUI::UString mTypeName;
	};

} // namespace tools

#endif // __STATE_PROPERTY_CONTROL_H__
