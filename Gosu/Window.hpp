//! \file Window.hpp
//! Interface of the Window class.

#ifndef GOSU_WINDOW_HPP
#define GOSU_WINDOW_HPP

#include <Gosu/Fwd.hpp>
#include <Gosu/Platform.hpp>
#include <Gosu/Input.hpp>
#include <Gosu/TR1.hpp>
#include <memory>
#include <string>

#ifdef GOSU_IS_WIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace Gosu
{
    //! Returns the width (in pixels) of the user's primary screen.
    unsigned screenWidth();
    
    //! Returns the height (in pixels) of the user's primary screen.
    unsigned screenHeight();
    
    //! Returns the maximum width (in 'points') that is available for a non-fullscreen Window.
    //! All windows larger than this size will automatically be shrunk to fit.
    unsigned availableWidth();
    
    //! Returns the maximum height (in 'points') that is available for a non-fullscreen Window.
    //! All windows larger than this size will automatically be shrunk to fit.
    unsigned availableHeight();
    
    //! Convenient all-in-one class that serves as the foundation of a standard
    //! Gosu application. Manages initialization of all of Gosu's core components
    //! and provides timing functionality.
    //! Note that you should really only use one instance of this class at the same time.
    //! This may or may not change later.
    class Window
    {
        struct Impl;
        const GOSU_UNIQUE_PTR<Impl> pimpl;
#if defined(GOSU_CPP11_ENABLED)
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
#endif
	
    public:
        //! Constructs a Window.
        //! \param width Width of the window in points; that is, pixels on a normal display, and 'virtual pixels' on a
        //! high-resolution display.
        //! \param height See width.
        //! \param updateInterval Interval in milliseconds between two calls
        //! to the update member function.
        Window(unsigned width, unsigned height, bool fullscreen = false,
            double updateInterval = 16.666666);
        virtual ~Window();

        std::wstring caption() const;
        void setCaption(const std::wstring& caption);
        
        double updateInterval() const;

        //! Enters a modal loop where the Window is visible on screen and
        //! receives calls to draw, update etc.
        void show();
        //! Closes the window if it is currently shown.
        void close();

        //! Called every updateInterval milliseconds while the window is being
        //! shown. Your application's main game logic goes here.
        virtual void update() {}
        //! Called after every update and when the OS wants the window to
        //! repaint itself. Your application's rendering code goes here.
        virtual void draw() {}
        
        //! Gives the game a chance to say no to being redrawn.
        //! This is not a definitive answer. The operating system can still force
        //! the window to redraw itself.
        //! By default, the window is redrawn all the time.
        virtual bool needsRedraw() const { return true; }

        //! If this function returns true, the system arrow cursor is drawn while
        //! over the window.
        virtual bool needsCursor() const { return false; }
        
        //! This function is called when the window loses focus on some platforms.
        //! Most importantly, it is called on the iPhone or iPad when the user
        //! locks the screen.
        virtual void loseFocus() {}
        
        //! This function is called when the operating system's memory is low.
        //! So far, it is only called in iOS applications.
        virtual void releaseMemory() {}

        //! Called before update when the user pressed a button while the
        //! window had the focus.
        virtual void buttonDown(Gosu::Button) {}
        //! Same as buttonDown. Called then the user released a button.
        virtual void buttonUp(Gosu::Button) {}
        
        // Ignore when SWIG is wrapping this class for Ruby/Gosu.
        #ifndef SWIG
        
        const Graphics& graphics() const;
        Graphics& graphics();
        
        const Input& input() const;
        Input& input();
        
        #ifdef GOSU_IS_IPHONE
        void* rootViewController() const;
        // iPhone-only callbacks for touch events.
        // Note that it does not hurt to override them even if you compile
        // for another platform; if you don't specify "virtual" the code
        // should even be stripped away cleanly.
        virtual void touchBegan(Touch touch) {}
        virtual void touchMoved(Touch touch) {}
        virtual void touchEnded(Touch touch) {}
        #endif        
        
        GOSU_DEPRECATED const Audio& audio() const;
        GOSU_DEPRECATED Audio& audio();

        #endif
    };
}

#ifdef GOSU_IS_IPHONE
Gosu::Window& windowInstance();
#endif

#endif
