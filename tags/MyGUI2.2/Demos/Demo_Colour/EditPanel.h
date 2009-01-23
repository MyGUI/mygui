/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __EDIT_PANEL_H__
#define __EDIT_PANEL_H__

#include <MyGUI.h>
#include <Ogre.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class EditPanel : public wraps::BaseLayout
	{
	public:
		EditPanel();

		/*virtual void initialise();
		virtual void shutdown();*/

		void setColourSelected(const Ogre::ColourValue& _colour);

	private:
		MyGUI::EditPtr mEdit;
	};

} // namespace demo

#endif // __EDIT_PANEL_H__
