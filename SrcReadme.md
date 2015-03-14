# Copying #

All of these source files are distributed under the license as in the COPYING file above

# Compiling #

At the moment the code doesn't have any custom makefiles created for it - Eclipse will work with it as a 'Managed C++ Project', or you can just compile all the .cpp files together -- either into individual object files before linking or straight into one file.

## Required Libraries ##
(In no particular order:)
  * **[3ds](http://www.lib3ds.org/)** - used for 3d model loading. A custom .obj file format will probably be used later.

  * **[GL](http://mesa3d.org/)** - GL and GLU are used for 3D rendering.
  * **GLU**
  * **GLEW**

  * **[SDL](http://www.libsdl.org/)** - SDL is used to give access to display and audio hardware, as well as providing events.
  * **SDL\_mixer**
  * **SDL\_image**

  * **[avformat](http://ffmpeg.mplayerhq.hu/)** - ffmpeg is used for audio file processing, to produce samples to feed to SDL.
  * **avcodec**

  * **[id3](http://id3lib.sourceforge.net/)** - used for reading id3 tags in some files.
  * **[z](http://www.zlib.net/)** - used by id3lib.

  * **[boost\_filesystem](http://www.boost.org/)** - Boost is used for some platform-independent filesystem operations.

# Contributions #

Contributions are welcome, especially bug reports and/or patches. See http://koruru.googlecode.com
for more information. At the moment, contributions will only be accepted if they are in the public domain (ie without a license) - this policy will probably be revised later if I have a chance to think about it properly.


# Conventions #

## Class Names ##

Class names are in camel case and have an intial letter that denotes their type:

  * i - interface
  * c - normal class
  * m - mixin


## Names ##

Object methods and variables should `be_lowercase_with_underscores`. Functions that return a bool should probably be named as a question -- `can_open_doors()`, or `has_no_fear()`, for example. The questions should try to fit with an `if` statement -- for example, `if(music_is_playing())` is better than `if(is_music_playing())`.

Note that these conventions are not followed completely throughout the code - this is an [open issue](http://code.google.com/p/koruru/issues/detail?id=14).

## Braces and Indentation ##

Braces are arranged as in a rough k&r style - code appearance/readability should always take preference over any particular style. Indentation should be used liberally - for example, between `glBegin()` and `glEnd()`, or `gl[Push/Pop]Matrix()` as well as conditionals, loops, functions and classes.