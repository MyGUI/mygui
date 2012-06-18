/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __EXPORT_MARSHALING_TYPE_H__
#define __EXPORT_MARSHALING_TYPE_H__

#include "Generate/MyGUI.Export_MarshalingWidget.h"

namespace Export
{

	template <> struct Convert< const MyGUI::DDItemInfo& >
	{
		struct DDItemInfo
		{
			Convert<MyGUI::DDContainer*>::Type sender;
			size_t sender_index;
			Convert<MyGUI::DDContainer*>::Type receiver;
			size_t receiver_index;
		};

		typedef const DDItemInfo& Type;
		static DDItemInfo mHolder;
		inline static const DDItemInfo& To(const MyGUI::DDItemInfo& _value)
		{
			mHolder.sender = Convert<MyGUI::DDContainer*>::To(_value.sender);
			mHolder.sender_index = _value.sender_index;
			mHolder.receiver = Convert<MyGUI::DDContainer*>::To(_value.receiver);
			mHolder.receiver_index = _value.receiver_index;
			return mHolder;
		}
	};

} // namespace Export

#endif // __EXPORT_MARSHALING_TYPE_H__
