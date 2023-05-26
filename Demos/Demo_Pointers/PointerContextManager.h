/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#ifndef POINTER_CONTEXT_MANAGER_H_
#define POINTER_CONTEXT_MANAGER_H_

#include <MyGUI.h>
#include "ResourcePointerContext.h"

namespace demo
{

	class IPointerSetter
	{
	public:
		virtual ~IPointerSetter() = default;

		virtual void setPointer(std::string_view _name) = 0;
	};

	class PointerContextManager
	{
	public:
		PointerContextManager(IPointerSetter* _setter);

		void addContext(std::string_view _name);
		void removeContext(std::string_view _name);

		void setPointer(std::string_view _name);

	private:
		void notifyChangeMousePointer(std::string_view _name);
		void updateCursor();

		bool isHighLevelContext() const;
		void _setPointer(std::string_view _name);

	private:
		std::string mCurrentPointer;
		std::string mCurrentGuiPointer;
		using VectorContext = std::vector<ResourcePointerContext*>;
		VectorContext mContexts;
		IPointerSetter* mPointerSetter;
	};

} // namespace demo

#endif // POINTER_CONTEXT_MANAGER_H_
