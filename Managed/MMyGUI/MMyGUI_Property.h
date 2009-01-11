/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_PROPERTY_H__
#define __MMYGUI_PROPERTY_H__

namespace MMyGUI
{

#define DECLARE_PROPERTY(name, type) \
	public: \
		property type name \
		{ \
			type get() { return mNative->get##name(); } \
			void set(type _value) { mNative->set##name(_value); } \
		}

} // namespace MMyGUI

#endif // __MMYGUI_PROPERTY_H__
