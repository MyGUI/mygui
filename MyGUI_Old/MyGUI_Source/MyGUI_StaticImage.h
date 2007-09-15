#pragma once

/*******************************************************
A stripped down version of the original window class
that just handles issues of position and Ogre overlays.
It effectively just encapsulates the MyGUI specific
issues of ZOrder and overlays into a simpler interface.
********************************************************/

#include "MyGUI_Skin.h"
#include <OgrePanelOverlayElement.h>

namespace MyGUI {

    class StaticImage
    {
        //Our interface with Ogre
        Ogre::Overlay* m_overlay; // оверлей этого окна
	    Ogre::PanelOverlayElement * m_overlayContainer; // оверлей элемента
	    
	    std::list<StaticImage *> mChildren;
	    
	    //Store different materials to use for different states
	    String mStateSkins[__SKIN_STATE_COUNT]; // скины состояний
	    
	    int mPosX, mPosY,
            mSizeX, mSizeY; // размеры окна

        StaticImage *mParent;
    
    public:
        typedef std::list<StaticImage *>::iterator ChildrenIterator;
        
	    StaticImage(const __MYGUI_SUBSKIN_INFO &pSkin, const String & strMaterialElement, uint8 uOverlay, StaticImage *pWindowParent = NULL);
	    virtual ~StaticImage();
	    
	    //Static Image
		StaticImage *show(bool ShowWindow = true);
        StaticImage *hide(); //hide the window
		StaticImage *setSkinState(__SKIN_STATES Skin);
	    
	    //returns "this" for chaining
	    StaticImage *move(int16 iPosX, int16 iPosY); // передвинуть окно
	    StaticImage *resize(int16 iSizeX, int16 iSizeY); // изменяем размер окна
	    
	    int16 getPosX() const { return mPosX; }
	    int16 getPosY() const { return mPosY; }
	    int16 getSizeX() const { return mSizeX; }
	    int16 getSizeY() const { return mSizeX; }
	    StaticImage *getParent() const { return (mParent); }
	    
	    //returns "this" for chaining
	    StaticImage *addChild(StaticImage *child);
	};
}