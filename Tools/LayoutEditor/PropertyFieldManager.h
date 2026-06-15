/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _71af0f85_133d_4e97_8f9f_0da6fe5f7d31_
#define _71af0f85_133d_4e97_8f9f_0da6fe5f7d31_

#include <MyGUI.h>
#include "IPropertyField.h"

namespace tools
{

	class PropertyFieldManager
	{
		MYGUI_SINGLETON_DECLARATION(PropertyFieldManager);

	public:
		PropertyFieldManager() :
			mSingletonHolder(this)
		{
		}
		void initialise();
		void shutdown();

		IPropertyField* createPropertyField(MyGUI::Widget* _window, std::string_view _type);

	private:
		using Delegate = MyGUI::delegates::Delegate<IPropertyField*&, MyGUI::Widget*>;
		using MapFactoryItem = std::map<std::string, Delegate, std::less<>>;
		MapFactoryItem mFactories;
	};

}

#endif
