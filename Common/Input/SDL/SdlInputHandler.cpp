
//Thanks to Jordan Milne and Scrawl for allowing to use their
//sdlinputwrapper files as base under the MIT license

#include "Precompiled.h"
#include "SdlInputHandler.h"
#include "InputManager.h"

#if MYGUI_USE_SDL2

#include <SDL_syswm.h>

namespace base
{
    SdlInputHandler::SdlInputHandler( SDL_Window *sdlWindow, input::InputManager *inputManager) :
        mSdlWindow( sdlWindow ),
		mInputManager( inputManager ),
        mWantRelative( false ),
        mWantMouseGrab( false ),
        mWantMouseVisible( true ),
        mIsMouseRelative( !mWantRelative ),
        mWrapPointerManually( false ),
        mGrabPointer( false ),
        mMouseInWindow( true ),
        mWindowHasFocus( true ),
        mWarpX( 0 ),
        mWarpY( 0 ),
        mWarpCompensate( false )
    {
    }
    //-----------------------------------------------------------------------------------
    SdlInputHandler::~SdlInputHandler()
    {
    }
    //-----------------------------------------------------------------------------------
    void SdlInputHandler::handleWindowEvent( const SDL_Event& evt )
    {
        switch( evt.window.event )
        {
            case SDL_WINDOWEVENT_ENTER:
                mMouseInWindow = true;
                updateMouseSettings();
                break;
            case SDL_WINDOWEVENT_LEAVE:
                mMouseInWindow = false;
                updateMouseSettings();
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                mWindowHasFocus = true;
                updateMouseSettings();
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                mWindowHasFocus = false;
                updateMouseSettings();
                break;
        }
    }
    //-----------------------------------------------------------------------------------
    void SdlInputHandler::_handleSdlEvents( const SDL_Event& evt )
    {
        switch( evt.type )
        {
            case SDL_MOUSEMOTION:
                // Ignore this if it happened due to a warp
                if( !handleWarpMotion(evt.motion) )
                {
                    // If in relative mode, don't trigger events unless window has focus
                    if( (!mWantRelative || mWindowHasFocus) && mInputManager )
                        mInputManager->mouseMoved( evt.motion );

                    // Try to keep the mouse inside the window
                    if (mWindowHasFocus)
                        wrapMousePointer( evt.motion );
                }
                break;
            case SDL_MOUSEWHEEL:
                {
                    if( mInputManager )
                        mInputManager->mouseWheelMoved( evt.wheel );
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    if( mInputManager )
                        mInputManager->mousePressed( evt.button );
                }
                break;
            case SDL_MOUSEBUTTONUP:
                {
                    if( mInputManager )
                        mInputManager->mouseReleased( evt.button );
                }
                break;
            case SDL_KEYDOWN:
                {
                    if( !evt.key.repeat && mInputManager )
                        mInputManager->keyPressed( 
							SDL_GetKeyFromScancode(evt.key.keysym.scancode), NULL );
                }
                break;
			case SDL_TEXTEDITING:
				break;
            case SDL_TEXTINPUT:
                {
                    if( !evt.key.repeat && mInputManager )
                        mInputManager->keyPressed( 
							SDL_GetKeyFromScancode(evt.key.keysym.scancode), &evt.text );
                }
                break;
            case SDL_KEYUP:
                {
                    if( !evt.key.repeat && mInputManager )
                        mInputManager->keyReleased( evt.key );
                }
                break;
            case SDL_JOYAXISMOTION:
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
            case SDL_JOYDEVICEADDED:
            case SDL_JOYDEVICEREMOVED:
				std::cerr << "Warning. Caught unexpected SDL event. Ignored." <<  std::endl;
                break;
            case SDL_WINDOWEVENT:
                handleWindowEvent(evt);
                break;
            /*default:
                std::cerr << "Unhandled SDL event of type " << evt.type << std::endl;
                break;*/
        }
    }
    //-----------------------------------------------------------------------------------
    void SdlInputHandler::setGrabMousePointer( bool grab )
    {
        mWantMouseGrab = grab;
        updateMouseSettings();
    }
    //-----------------------------------------------------------------------------------
    void SdlInputHandler::setMouseRelative( bool relative )
    {
        mWantRelative = relative;
        updateMouseSettings();
    }
    //-----------------------------------------------------------------------------------
    void SdlInputHandler::setMouseVisible( bool visible )
    {
        mWantMouseVisible = visible;
        updateMouseSettings();
    }
    //-----------------------------------------------------------------------------------
    void SdlInputHandler::updateMouseSettings(void)
    {
        mGrabPointer = mWantMouseGrab && mMouseInWindow && mWindowHasFocus;
        SDL_SetWindowGrab( mSdlWindow, mGrabPointer ? SDL_TRUE : SDL_FALSE );

        SDL_ShowCursor( mWantMouseVisible || !mWindowHasFocus );

        bool relative = mWantRelative && mMouseInWindow && mWindowHasFocus;
        if( mIsMouseRelative == relative )
            return;

        mIsMouseRelative = relative;

        mWrapPointerManually = false;

        //Input driver doesn't support relative positioning. Do it manually.
        int success = SDL_SetRelativeMouseMode( relative ? SDL_TRUE : SDL_FALSE );
        if( !relative || (relative && success != 0) )
            mWrapPointerManually = true;

        //Remove all pending mouse events that were queued with the old settings.
        SDL_PumpEvents();
        SDL_FlushEvent( SDL_MOUSEMOTION );
    }
    //-----------------------------------------------------------------------------------
    void SdlInputHandler::warpMouse( int x, int y )
    {
        SDL_WarpMouseInWindow( mSdlWindow, x, y );
        mWarpCompensate = true;
        mWarpX = x;
        mWarpY = y;
    }
    //-----------------------------------------------------------------------------------
    void SdlInputHandler::wrapMousePointer( const SDL_MouseMotionEvent& evt )
    {
        //Don't wrap if we don't want relative movements, support
        //relative movements natively, or aren't grabbing anyways
        if( mIsMouseRelative || !mWrapPointerManually || !mGrabPointer )
            return;

        int width = 0;
        int height = 0;

        SDL_GetWindowSize( mSdlWindow, &width, &height );

        const int FUDGE_FACTOR_X = width;
        const int FUDGE_FACTOR_Y = height;

        //Warp the mouse if it's about to go outside the window
        if( evt.x - FUDGE_FACTOR_X < 0  || evt.x + FUDGE_FACTOR_X > width ||
            evt.y - FUDGE_FACTOR_Y < 0  || evt.y + FUDGE_FACTOR_Y > height )
        {
            warpMouse( width / 2, height / 2 );
        }
    }
    //-----------------------------------------------------------------------------------
    bool SdlInputHandler::handleWarpMotion( const SDL_MouseMotionEvent& evt )
    {
        if( !mWarpCompensate )
            return false;

        //This was a warp event, signal the caller to eat it.
        if( evt.x == mWarpX && evt.y == mWarpY )
        {
            mWarpCompensate = false;
            return true;
        }

        return false;
    }
}
#endif
