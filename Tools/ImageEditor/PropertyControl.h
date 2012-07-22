/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _e14ab35f_103d_4acf_ab0e_643de48cf55f_
#define _e14ab35f_103d_4acf_ab0e_643de48cf55f_

#include <MyGUI.h>
#include "Property.h"
#include "Control.h"

namespace tools
{

	class PropertyControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		PropertyControl();
		virtual ~PropertyControl();

		void setProperty(Property* _value);
		Property* getProperty();

		void executeAction(const std::string& _value, bool _merge = false);

	protected:
		virtual void updateProperty();
		virtual void updateCaption();

	private:
		void notifyChangeProperty(Property* _sender);

		void advice();
		void unadvice();

	private:
		Property* mProperty;
	};

}

#endif
