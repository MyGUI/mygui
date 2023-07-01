/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _faf0ef48_7615_4de7_812c_48520c83de61_
#define _faf0ef48_7615_4de7_812c_48520c83de61_

#include <memory>
#include <type_traits>
#include "sigslot.h"
#include "DataTypeProperty.h"

namespace tools
{
	class Data;
	using DataPtr = std::shared_ptr<Data>;

	class MYGUI_EXPORT_DLL Property
	{
	public:
		using PropertyPtr = std::shared_ptr<Property>;
		using PropertyWeak = std::weak_ptr<Property>;

		Property() = delete;
		Property(DataTypePropertyPtr _type, DataPtr _owner);

		static PropertyPtr CreateInstance(DataTypePropertyPtr _type, DataPtr _owner);

		void initialise();

		const std::string& getValue() const;
		void setValue(std::string_view _value);

		template<typename Type>
		Type getValue() const
		{
			return MyGUI::utility::parseValue<Type>(getValue());
		}

		template<typename Type, typename = std::enable_if_t<!std::is_convertible_v<Type, std::string_view>>>
		void setValue(const Type& _value)
		{
			setValue(MyGUI::utility::toString(_value));
		}

		DataTypePropertyPtr getType();

		DataPtr getOwner();

		sigslot::signal1<PropertyPtr> eventChangeProperty;

	private:
		std::string mValue;
		DataTypePropertyPtr mType;
		DataPtr mOwner;
		PropertyWeak mWeakThis;
	};

	using PropertyPtr = Property::PropertyPtr;

}

#endif
