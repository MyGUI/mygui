/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_MANAGER_H__
#define __PROPERTY_FIELD_MANAGER_H__

#include <MyGUI.h>
#include "PropertyField.h"

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

		IPropertyField* createPropertyField(MyGUI::Widget* _window, const std::string& _type, MyGUI::Widget* _currentWidget);
	};

} // namespace tools

#endif // __PROPERTY_FIELD_MANAGER_H__
