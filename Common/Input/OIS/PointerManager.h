/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#ifndef __POINTER_MANAGER_H__
#define __POINTER_MANAGER_H__

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

		void loadPointerResources();
	};

} // namespace input

#endif // __POINTER_MANAGER_H__
