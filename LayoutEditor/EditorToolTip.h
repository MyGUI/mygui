/*!
	@file
	@author		Georgiy Evmenov
	@date		08/2008
	@module
*/
#ifndef __EDITOR_TOOL_TIP_H__
#define __EDITOR_TOOL_TIP_H__

#include <MyGUI.h>
#include "BaseLayout.h"

class EditorToolTip : public BaseLayout
{
public:
	EditorToolTip();

	virtual void initialise();

	void show(const Ogre::UTFString & _text, const MyGUI::IntPoint & _point);
	void hide();

private:

	MyGUI::EditPtr mText;
};

#endif // __EDITOR_TOOL_TIP_H__
