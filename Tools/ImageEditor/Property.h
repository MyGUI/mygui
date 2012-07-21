/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _faf0ef48_7615_4de7_812c_48520c83de61_
#define _faf0ef48_7615_4de7_812c_48520c83de61_

#include "sigslot.h"
#include "DataTypeProperty.h"

namespace tools
{
	class Data;

	class Property
	{
	public:
		Property(DataTypeProperty* _type, Data* _owner);
		~Property();

		void initialise();

		const std::string& getValue() const;
		void setValue(const std::string& _value);

		template <typename Type>
		Type getValue() const
		{
			return MyGUI::utility::parseValue<Type>(getValue());
		}

		template <typename Type>
		void setValue(const Type& _value)
		{
			setValue(MyGUI::utility::toString(_value));
		}

		void setValue(const bool& _value)
		{
			setValue(std::string(_value ? "True" : "False"));
		}

		DataTypeProperty* getType();

		Data* getOwner();

		sigslot::signal1<Property*> eventChangeProperty;

	private:
		Property();

	private:
		std::string mValue;
		DataTypeProperty* mType;
		Data* mOwner;
	};

}

#endif
