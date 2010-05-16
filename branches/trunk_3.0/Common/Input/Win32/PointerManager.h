/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#ifndef __POINTER_MANAGER_H__
#define __POINTER_MANAGER_H__

#include "ResourceW32Pointer.h"

namespace input
{

	class PointerManager
	{
	public:
		PointerManager();
		virtual ~PointerManager();

		void createPointerManager(size_t _handle);
		void destroyPointerManager();

		void setPointerVisible(bool _value);
		void setPointerName(const std::string& _name);

	private:
		void notifyChangeMousePointer(const std::string& _name);
		void setPointerHandle(size_t _id);
		bool isMouseInClient();
		void setPointer(const std::string& _name);

	private:
		typedef std::map<std::string, size_t> MapPointer;
		MapPointer mMapGuiPointer;
		size_t mHwnd;
		bool mManagerPointer;
	};

} // namespace input

#endif // __POINTER_MANAGER_H__
