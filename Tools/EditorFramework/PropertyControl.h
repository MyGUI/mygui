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

	class MYGUI_EXPORT_DLL PropertyControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		PropertyControl();
		virtual ~PropertyControl();

		void setProperty(PropertyPtr _value);
		PropertyPtr getProperty();

		void executeAction(const std::string& _value, bool _merge = false);

	protected:
		virtual void updateProperty();
		virtual void updateCaption();

	private:
		void notifyChangeProperty(PropertyPtr _sender);

		void advice();
		void unadvice();

	private:
		PropertyPtr mProperty;
	};

}

#endif
