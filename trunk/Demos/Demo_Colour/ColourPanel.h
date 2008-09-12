/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __COLOUR_PANEL_H__
#define __COLOUR_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout.h"

class ColourPanel : public BaseLayout
{
public:
	ColourPanel();

	virtual void initialise();

	void show();
	void hide();

	inline bool isShow() { return mMainWidget->isShow(); } 

private:
	void first_update();

	void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);
	void notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);

private:
	MyGUI::WidgetPtr mColourRect;
	MyGUI::WidgetPtr mColourView;
	MyGUI::StaticImagePtr mImageColourPicker;

	Ogre::ColourValue mCurrentColour;
	Ogre::ColourValue mStartColour;

	MyGUI::RawRect * mRawColourRect;
	MyGUI::RawRect * mRawColourView;
};

#endif // __COLOUR_PANEL_H__
