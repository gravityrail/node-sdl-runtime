# node-sdl

Simple DirectMedia Layer bindings for node.js



## Installation (Ubuntu or Debian)

```bash
sudo apt-get install nodejs; # This will install npm, too.
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev;
sudo npm install -g node-gyp;
```



## Installation (Mac OSX)

This is still unstable and needs some documentation.



## Build Process

The build process is integrated with **node-gyp** which offers cross-compiling features
and allows integration with the typical nodejs build workflow. If you installed everything correctly,
you can build the library by using the following commands:

```bash
node-gyp configure;
node-gyp build;
```


## Usage

As a general rule, these bindings adhere to the following conventions:

* SDL structs are wrappet at the base namespace (e.g. sdl.Window).
* SDL structs that are normally passed to memory-owning functions are instead wrapped on the object instances. (e.g. **sdl.GetWindowWidth(window)** is now **new sdl.Window().getWidth()**).
* SDL constants and enums are split up into subnamespaces (e.g. **SDL_WINDOWPOS_CENTERED** is now **sdl.WINDOWPOS.CENTERED**).
* SDL extensions to the base SDL API are under their own namespace (e.g. sdl.TTF.Font).



## Hello World Program

This is the **Hello World** Program using the SDL bindings.

It will create a window instance and close the program after 2 seconds.
In order to quit the program successfully, it is necessary to delete all references to sdl struct instances.


```javascript
sdl.init(sdl.INIT.EVERYTHING);

var window = new sdl.Window("Hello World Window", sdl.WINDOWPOS.CENTERED, sdl.WINDOWPOS.CENTERED, 640, 480);

setTimeout(function() {
	delete window; // Clears reference of the sdl.Window instance
	sdl.quit();
}, 2000);
```



# License

The node-sdl project (and this Android-ready fork) is licensed under MIT license.
Please see LICENSE.txt for more details.

