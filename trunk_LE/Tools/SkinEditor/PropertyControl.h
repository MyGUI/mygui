/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __PROPERTY_CONTROL_H__
#define __PROPERTY_CONTROL_H__

#include <MyGUI.h>
#include "Property.h"

namespace tools
{

	class PropertyControl
	{
	public:
		PropertyControl();
		virtual ~PropertyControl();

		void setProperty(Property* _value);
		Property* getProperty();

		const MyGUI::UString& getTypeName();

	protected:
		virtual void updateProperty();

	private:
		void notifyChangeProperty(Property* _sender, const MyGUI::UString& _owner);

		void advice();
		void unadvice();

	private:
		Property* mProperty;
		MyGUI::UString mTypeName;
	};

} // namespace tools

#endif // __PROPERTY_CONTROL_H__
