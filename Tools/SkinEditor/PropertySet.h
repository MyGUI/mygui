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

	class PropertySet :
		public MyGUI::ISerializable
	{
	public:
		PropertySet();
		~PropertySet();

		Property* createChild(const MyGUI::UString& _name, const MyGUI::UString& _type, const MyGUI::UString& _value = "");

		void destroyChild(Property* _item);

		void destroyChild(const MyGUI::UString& _name);

		void destroyAllChilds();

		Property* getChild(const MyGUI::UString& _name) const;

		EnumeratorProperty getChildsEnumerator() const;

		EventHandle_ChangeProperty eventChangeProperty;

		void setPropertyValue(const MyGUI::UString& _propertyName, const MyGUI::UString& _value, const MyGUI::UString& _owner);
		const MyGUI::UString& getPropertyValue(const MyGUI::UString& _propertyName) const;

		void setPropertyReadOnly(const MyGUI::UString& _propertyName, bool _value);
		bool getPropertyReadOnly(const MyGUI::UString& _propertyName) const;

		virtual void serialization(MyGUI::xml::Element* _node, MyGUI::Version _version);
		virtual void deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version);

	private:
		void advise(Property* _item);
		void unadvise(Property* _item);

		void notifyChangeProperty(Property* _item, const MyGUI::UString& _owner);

	private:
		VectorProperty mChilds;
	};

} // namespace tools

#endif // __PROPERTY_SET_H__
