	public:
		delegate void Handle#{DelegateName}( Convert<#{ValueType1}>::Type #{ValueName1}, Convert<#{ValueType2}>::Type #{ValueName2}, Convert<#{ValueType3}>::Type #{ValueName3} );
		event Handle#{DelegateName}^ Event#{DelegateName}
		{
			void add(Handle#{DelegateName}^ _value)
			{
				mDelegate#{DelegateName} += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->event#{DelegateName} =
					static_cast< MyGUI::delegates::IDelegate3< #{ValueType1}, #{ValueType2}, #{ValueType3} > *>(
						new Delegate3< Handle#{DelegateName}^, #{ValueType1}, #{ValueType2}, #{ValueType3} >(mDelegate#{DelegateName}) );
			}
			void remove(Handle#{DelegateName}^ _value)
			{
				mDelegate#{DelegateName} -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegate#{DelegateName} == nullptr)
					static_cast<ThisType*>(mNative)->event#{DelegateName} = nullptr;
				else
					static_cast<ThisType*>(mNative)->event#{DelegateName} =
						static_cast< MyGUI::delegates::IDelegate3< #{ValueType1}, #{ValueType2}, #{ValueType3} > *>(
							new Delegate3< Handle#{DelegateName}^, #{ValueType1}, #{ValueType2}, #{ValueType3} >(mDelegate#{DelegateName}) );
			}
		}
	private:
		Handle#{DelegateName}^ mDelegate#{DelegateName};

