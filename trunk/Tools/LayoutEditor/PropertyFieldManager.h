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

	class PropertyFieldManager :
		public MyGUI::Singleton<PropertyFieldManager>
	{
	public:
		PropertyFieldManager();
		virtual ~PropertyFieldManager();

		void initialise();
		void shutdown();

		IPropertyField* createPropertyField(MyGUI::Widget* _window, const std::string& _type);

	private:
		typedef MyGUI::delegates::CDelegate2<IPropertyField*&, MyGUI::Widget*> Delegate;
		typedef std::map<std::string, Delegate> MapFactoryItem;
		MapFactoryItem mFactories;
	};

}

#endif
