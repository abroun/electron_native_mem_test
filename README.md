Simple project to demonstrate limited heap size for C++ Node JS addons when running in 
Electron JS on Linux.

This project shows that on Windows, there are no limitations to the heap size in a Node JS
addon in Electron JS, although there does seem to be a limitation in the main Electron process
when memory is allocated by JS. On Linux, the heap size looks to be capped to 16GB. 
The behaviour is unknown on MacOS.

## Requirements

To build this project you'll need CMake 3.22 or later and Node JS (tested with v20.14.0) installed.

To see the heap size limitation you'll likely need a computer with 32GB RAM or more, and to run with
minimal other active programs so that you don't get hit by the out of memory killer on Linux.

## Building

You can see the operation of the Node JS addon on its own by running

```
cd native_addon
npm install
node example.js
```

This will build the native addon and run it in vanilla Node JS. On both Windows and Linux you should
see the allocated memory increase in 1GB blocks until all system memory is consumed.

Then observe the operation of the addon running inside Electron JS by running

```
cd electron_app
npm install
npm run start
```

This will start up a basic Electron JS app with a button to allocate 1GB of memory. Here you should find
that on Windows this will again keep going until all system memory is consumed. On Linux though it will
crash when 16GB of memory has been allocated, even if there is much more memory available on the system.