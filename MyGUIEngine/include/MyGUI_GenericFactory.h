/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_GENERIC_FACTORY_H_
#define MYGUI_GENERIC_FACTORY_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_Delegate.h"

namespace MyGUI
{

	template <typename Type>
	class GenericFactory
	{
	public:
		typedef delegates::CDelegate1<IObject*&> Delegate;
		static typename Delegate::IDelegate* getFactory()
		{
			return newDelegate(createFromFactory);
		}

	private:
		static void createFromFactory(IObject*& _instance)
		{
			_instance = new Type();
		}
	};

} // namespace MyGUI

#endif // MYGUI_GENERIC_FACTORY_H_
