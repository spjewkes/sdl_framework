# SDL Framework

This is a simple SDL 2 framework to be used when constructing other projects. The intention is that the SDL framework class is inherited and the virtual functions are overridden that provide the functionality you wish to add.

The functions for overriding are:
* __OnCreate()__ - allows for any initialization prior to running the update loop. Return true to indicate that initialization was successful. Returning will cause the framework to exit.
* __OnUpdate()__ - when framework is running this is called repeatedly to allow your project to do any necessary updates (including drawing to the screen). An elapsed time value is passed to indicate the time passed since the last __OnUpdate()__ call (this is a floating-point value indicating the number seconds). Returning true from this function ensures that another update will be called. Returning false will cause the framework to exit. It is the responsibility of this function to call whatever is needed to update the SDL window.
* __OnDestroy()__ - called once as the framework exits. This allows any necessary clean up of resources.

The SDL framework class provides other utility functions for accessing information about the SDL window created and controlled by the framework.

* __get_window()__ - returns the SDL window reference.
* __get_renderer()__ - return the SDL renderer context for the SDL window (this is created on start-up).
* __width()__ - return the width in pixels of the SDL window.
* __height()__ - return the height in pixels of the SDL window.


__NOTE: This is currently designed for building on a Mac OS X and Linux only__

