﻿	public:
		delegate void Handle#{DelegateName}( Convert<#{ValueType1}>::Type #{ValueName1} );
		event Handle#{DelegateName}^ #{DelegateName}
		{
			void add(Handle#{DelegateName}^ _value)
			{
				mDelegate#{DelegateName} += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->event#{DelegateName} =
					static_cast< MyGUI::delegates::IDelegate1< #{ValueType1} > *>(
						new Delegate1< Handle#{DelegateName}^, #{ValueType1} >(mDelegate#{DelegateName}) );
			}
			void remove(Handle#{DelegateName}^ _value)
			{
				mDelegate#{DelegateName} -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegate#{DelegateName} == nullptr)
					static_cast<ThisType*>(mNative)->event#{DelegateName} = nullptr;
				else
					static_cast<ThisType*>(mNative)->event#{DelegateName} =
						static_cast< MyGUI::delegates::IDelegate1< #{ValueType1} > *>(
							new Delegate1< Handle#{DelegateName}^, #{ValueType1} >(mDelegate#{DelegateName}) );
			}
		}
	private:
		Handle#{DelegateName}^ mDelegate#{DelegateName};

