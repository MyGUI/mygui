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

	class Property
	{
	public:
		Property(const MyGUI::UString& _name, const MyGUI::UString& _type);
		~Property();

		const MyGUI::UString& getValue();
		void setValue(const MyGUI::UString& _value, const MyGUI::UString& _owner);

		const MyGUI::UString& getName();
		const MyGUI::UString& getType();

		bool getReadOnly();
		void setReadOnly(bool _value);

		EventHandle_ChangeProperty eventChangeProperty;

	private:
		MyGUI::UString mName;
		MyGUI::UString mType;
		MyGUI::UString mValue;
		bool mReadOnly;
	};

} // namespace tools

#endif // __PROPERTY_H__
