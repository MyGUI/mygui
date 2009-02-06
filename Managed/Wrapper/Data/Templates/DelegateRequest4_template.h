	public:
		delegate void Handle#{DelegateName}( Convert<#{ValueType1}>::Type #{ValueName1}, Convert<#{ValueType2}>::Type #{ValueName2}, Convert<#{ValueType3}>::Type #{ValueName3}, Convert<#{ValueType4}>::Type #{ValueName4} );
		event Handle#{DelegateName}^ Request#{DelegateName}
		{
			void add(Handle#{DelegateName}^ _value)
			{
				mDelegate#{DelegateName} += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->request#{DelegateName} =
					static_cast< MyGUI::delegates::IDelegate4< #{ValueType1}, #{ValueType2}, #{ValueType3}, #{ValueType4} > *>(
						new Delegate4< Handle#{DelegateName}^, #{ValueType1}, #{ValueType2}, #{ValueType3}, #{ValueType4} >(mDelegate#{DelegateName}) );
			}
			void remove(Handle#{DelegateName}^ _value)
			{
				mDelegate#{DelegateName} -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegate#{DelegateName} == nullptr)
					static_cast<ThisType*>(mNative)->request#{DelegateName} = nullptr;
				else
					static_cast<ThisType*>(mNative)->request#{DelegateName} =
						static_cast< MyGUI::delegates::IDelegate4< #{ValueType1}, #{ValueType2}, #{ValueType3}, #{ValueType4} > *>(
							new Delegate4< Handle#{DelegateName}^, #{ValueType1}, #{ValueType2}, #{ValueType3}, #{ValueType4} >(mDelegate#{DelegateName}) );
			}
		}
	private:
		Handle#{DelegateName}^ mDelegate#{DelegateName};

