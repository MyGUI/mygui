/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#ifndef __POINTER_CONTEXT_MANAGER_H__
#define __POINTER_CONTEXT_MANAGER_H__

#include <MyGUI.h>
#include "ResourcePointerContext.h"

namespace demo
{

	class IPointerSetter
	{
	public:
		IPointerSetter() { }
		virtual ~IPointerSetter() { }

		virtual void setPointer(const std::string& _name) = 0;
	};

	class PointerContextManager
	{
	public:
		PointerContextManager(IPointerSetter* _setter);

		void addContext(const std::string& _name);
		void removeContext(const std::string& _name);

		void setPointer(const std::string& _name);

	private:
		void notifyChangeMousePointer(const std::string& _name);
		void updateCursor();

		bool isHighLevelContext() const;
		void _setPointer(const std::string& _name);

	private:
		std::string mCurrentPointer;
		std::string mCurrentGuiPointer;
		typedef std::vector<ResourcePointerContext*> VectorContext;
		VectorContext mContexts;
		IPointerSetter* mPointerSetter;
	};

} // namespace demo

#endif // __POINTER_CONTEXT_MANAGER_H__
