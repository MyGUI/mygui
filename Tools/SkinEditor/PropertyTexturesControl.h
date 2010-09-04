/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __PROPERTY_TEXTURES_CONTROL_H__
#define __PROPERTY_TEXTURES_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"

namespace tools
{

	class PropertyTexturesControl :
		public wraps::BaseLayout,
		public PropertyControl
	{
	public:
		PropertyTexturesControl(MyGUI::Widget* _parent);
		virtual ~PropertyTexturesControl();

	protected:
		virtual void updateProperty();

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		size_t getComboIndex(const MyGUI::UString& _name);

	private:
		MyGUI::ComboBox* mComboBox;
	};

} // namespace tools

#endif // __PROPERTY_TEXTURES_CONTROL_H__
