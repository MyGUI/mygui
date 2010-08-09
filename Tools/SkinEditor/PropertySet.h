/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __PROPERTY_SET_H__
#define __PROPERTY_SET_H__

#include <MyGUI.h>
#include "Property.h"

namespace tools
{

	typedef std::vector<Property*> VectorProperty;
	typedef MyGUI::Enumerator<VectorProperty> EnumeratorProperty;

	class PropertySet
	{
	public:
		PropertySet();
		~PropertySet();

		Property* createChild(const MyGUI::UString& _name, const MyGUI::UString& _type, const MyGUI::UString& _value = "");

		void destroyChild(Property* _item);

		void destroyChild(const MyGUI::UString& _name);

		void destroyAllChilds();

		Property* getChild(const MyGUI::UString& _name);

		EnumeratorProperty getChildsEnumerator();

		EventHandle_ChangeProperty eventChangeProperty;

		void setPropertyValue(const MyGUI::UString& _propertyName, const MyGUI::UString& _value, const MyGUI::UString& _owner);
		const MyGUI::UString& getPropertyValue(const MyGUI::UString& _propertyName);

	private:
		void advise(Property* _item);
		void unadvise(Property* _item);

		void notifyChangeProperty(Property* _item, const MyGUI::UString& _owner);

	private:
		VectorProperty mChilds;
	};

} // namespace tools

#endif // __PROPERTY_SET_H__
