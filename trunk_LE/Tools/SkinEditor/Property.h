/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __PROPERTY_H__
#define __PROPERTY_H__

#include <MyGUI.h>

namespace tools
{

	class Property;
	typedef MyGUI::delegates::CMultiDelegate2<Property*, const MyGUI::UString&> EventHandle_ChangeProperty;

	class Property :
		public MyGUI::ISerializable
	{
	public:
		Property(const MyGUI::UString& _name, const MyGUI::UString& _type);
		~Property();

		const MyGUI::UString& getValue() const;
		void setValue(const MyGUI::UString& _value, const MyGUI::UString& _owner);

		const MyGUI::UString& getName() const;
		const MyGUI::UString& getType() const;

		bool getReadOnly() const;
		void setReadOnly(bool _value);

		EventHandle_ChangeProperty eventChangeProperty;

		virtual void serialization(MyGUI::xml::Element* _node, MyGUI::Version _version);
		virtual void deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version);

	private:
		MyGUI::UString mName;
		MyGUI::UString mType;
		MyGUI::UString mValue;
		bool mReadOnly;
	};

} // namespace tools

#endif // __PROPERTY_H__
