/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _faf0ef48_7615_4de7_812c_48520c83de61_
#define _faf0ef48_7615_4de7_812c_48520c83de61_

#include "sigslot.h"
#include "DataTypeProperty.h"
#include "SharedPtr.h"

namespace tools
{
	class Data;
	typedef shared_ptr<Data> DataPtr;

	class MYGUI_EXPORT_DLL Property
	{
	public:
		typedef shared_ptr<Property> PropertyPtr;
		typedef weak_ptr<Property> PropertyWeak;

		Property(DataTypePropertyPtr _type, DataPtr _owner);
		~Property();

		static PropertyPtr CreateInstance(DataTypePropertyPtr _type, DataPtr _owner);

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

		DataTypePropertyPtr getType();

		DataPtr getOwner();

		sigslot::signal1<PropertyPtr> eventChangeProperty;

	private:
		Property();

	private:
		std::string mValue;
		DataTypePropertyPtr mType;
		DataPtr mOwner;
		PropertyWeak mWeakThis;
	};

	typedef Property::PropertyPtr PropertyPtr;

}

#endif
