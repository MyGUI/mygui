/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _d2ea5365_b568_42c0_9028_0ac6e208494f_
#define _d2ea5365_b568_42c0_9028_0ac6e208494f_

#include "Action.h"
#include "Property.h"

namespace tools
{

	class ActionChangeDataProperty :
		public Action
	{
	public:
		ActionChangeDataProperty();
		virtual ~ActionChangeDataProperty();

		void setProperty(Property* _value);
		Property* getProperty();

		void setValue(const std::string& _value);
		const std::string& getValue() const;

	private:
		Property* mProperty;
		std::string mValue;
	};

}

#endif
