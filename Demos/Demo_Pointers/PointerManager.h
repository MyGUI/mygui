/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/
#ifndef __POINTER_MANAGER_H__
#define __POINTER_MANAGER_H__

#include <MyGUI.h>
#include "ResourcePointerContext.h"

namespace demo
{

	class PointerManager
	{
	public:
		PointerManager();

		void addContext(const std::string& _name);
		void removeContext(const std::string& _name);

		void setPointer(const std::string& _name);

	private:
		void notifyChangeMousePointer(const std::string& _name);
		void updateCursor();

		bool isHighLevelContext();
		void _setPointer(const std::string& _name);

	private:
		std::string mCurrentPointer;
		std::string mCurrentGuiPointer;
		typedef std::vector<ResourcePointerContext*> VectorContext;
		VectorContext mContexts;
	};

} // namespace demo

#endif // __POINTER_MANAGER_H__
