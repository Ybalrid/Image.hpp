# Image.hpp

[![Build Status](https://travis-ci.com/Ybalrid/Image.hpp.svg?branch=master)](https://travis-ci.com/Ybalrid/Image.hpp)
[![Build status](https://ci.appveyor.com/api/projects/status/8gj0l2br4lccfwq0/branch/master?svg=true)](https://ci.appveyor.com/project/Ybalrid/image-hpp/branch/master)

C++ wrapper around FreeImage. Provide a simple, object based abstraction to represent an image file.

## Features

- Ease of use (single header to add to your code after linking to FreeImage)
- Simple `Image` class
- RAII managed bitmap in memory
- Automatic initialization of free image library
- Uses C++11 move semantics
- Exception based error checking
- Load image from disk (from all FreeImage supported formats)
- Create image (array of pixel) from code
- Easy pixel access to the image
- Save image to disk (in any FreeImage supported format)

## 3rd party notice

This library is mainly a wrapper around FreeImage. Free Image is either usable under the terms of the `liberal FreeImage Public Licence`, or the `GNU General Public License` version 2.0 or version 3.0

This repository uses Catch2 for unit testing. Catch2 is distributed under the terms of the `Boost Software Licese 1.0`

## Usage

- Make your program able to build and link with FreeImage
- Add Image.hpp to your include path or copy it in your code
- In one single source file, add this:
```cpp
#define IMAGE_PP_IMPLEMENTS
#include "Image.hpp"
```
- have fun

