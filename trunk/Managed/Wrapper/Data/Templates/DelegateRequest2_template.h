	public:
		delegate void Handle#{DelegateName}( Convert<#{ValueType1}>::Type #{ValueName1}, Convert<#{ValueType2}>::Type #{ValueName2} );
		event Handle#{DelegateName}^ #{DelegateName}
		{
			void add(Handle#{DelegateName}^ _value)
			{
				mDelegate#{DelegateName} += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->request#{DelegateName} =
					static_cast< MyGUI::delegates::IDelegate2< #{ValueType1}, #{ValueType2} > *>(
						new Delegate2< Handle#{DelegateName}^, #{ValueType1}, #{ValueType2} >(mDelegate#{DelegateName}) );
			}
			void remove(Handle#{DelegateName}^ _value)
			{
				mDelegate#{DelegateName} -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegate#{DelegateName} == nullptr)
					static_cast<ThisType*>(mNative)->request#{DelegateName} = nullptr;
				else
					static_cast<ThisType*>(mNative)->request#{DelegateName} =
						static_cast< MyGUI::delegates::IDelegate2< #{ValueType1}, #{ValueType2} > *>(
							new Delegate2< Handle#{DelegateName}^, #{ValueType1}, #{ValueType2} >(mDelegate#{DelegateName}) );
			}
		}
	private:
		Handle#{DelegateName}^ mDelegate#{DelegateName};

