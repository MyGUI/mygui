
//Thanks to Jordan Milne and Scrawl for allowing to use their
//sdlinputwrapper files as base under the MIT license

#ifndef _Base_SdlInputHandler_H_
#define _Base_SdlInputHandler_H_

#ifdef MYGUI_SAMPLES_INPUT_SDL2
#include "SdlEmulationLayer.h"
#endif

#include <SDL.h>

namespace input
{
	class InputManager;
}

namespace base
{

    class SdlInputHandler
    {
        SDL_Window  *mSdlWindow;
        input::InputManager *mInputManager;

        // User settings
        // User setting. From the SDL docs: While the mouse is in relative mode, the
        // cursor is hidden, and the driver will try to report continuous motion in
        // the current window. Only relative motion events will be delivered, the
        // mouse position will not change.
        bool        mWantRelative;
        // Locks the pointer to remain inside the window.
        bool        mWantMouseGrab;
        // Whether to show the mouse.
        bool        mWantMouseVisible;

        // Describes internal state.
        bool        mIsMouseRelative;
        // Used when driver doesn't support relative mode.
        bool        mWrapPointerManually;
        bool        mGrabPointer;
        bool        mMouseInWindow;
        bool        mWindowHasFocus;

        Uint16      mWarpX;
        Uint16      mWarpY;
        bool        mWarpCompensate;

        void updateMouseSettings(void);

        void handleWindowEvent( const SDL_Event& evt );

        // Moves the mouse to the specified point within the viewport
        void warpMouse( int x, int y);

        // Prevents the mouse cursor from leaving the window.
        void wrapMousePointer( const SDL_MouseMotionEvent& evt );

        // Internal method for ignoring relative
        // motions as a side effect of warpMouse()
        bool handleWarpMotion( const SDL_MouseMotionEvent& evt );

    public:
		SdlInputHandler(SDL_Window *sdlWindow, input::InputManager *inputManager);
        virtual ~SdlInputHandler();

        void _handleSdlEvents( const SDL_Event& evt );

        // Locks the pointer to the window
        void setGrabMousePointer( bool grab );

        // Set the mouse to relative positioning mode (when not supported
        // by hardware, we emulate the behavior).
        // From the SDL docs: While the mouse is in relative mode, the
        // cursor is hidden, and the driver will try to report continuous
        // motion in the current window. Only relative motion events will
        // be delivered, the mouse position will not change.
        void setMouseRelative( bool relative );

        // Shows or hides the mouse cursor.
        void setMouseVisible( bool visible );
    };
}

#endif
