/*
 *   MIT License
 *
 *   Copyright (c) 2018 Arthur Brainville (Ybalrid)
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */


#pragma once

#include <FreeImage.h>
#include <iostream>
#include <cassert>
#include <exception>
#include <string>
#include <sstream>

///Exception : can't allocate image
class ImageCantAllocateExcept : public std::runtime_error
{
    public:
        explicit ImageCantAllocateExcept(const std::string& _Message = "Allocating image failed!")
            : runtime_error(_Message)
            {
                std::cout << exception::what() << '\n';
            }
};

///Exception : can't load image
class ImageCantLoadExept : public std::runtime_error
{
    std::string path;
    public:
    ImageCantLoadExept(const std::string& path, const std::string& _Message = "Loading image from fime Failed")
        : runtime_error(_Message), path{path}
    {
        std::cout << ImageCantLoadExept::what() << std::endl;
    }

    const char* what() const throw() final
    {
        std::ostringstream out;
        out << std::runtime_error::what();
        out << "\nImage path was : " << path << '\n';
        return out.str().c_str();
    }

};

///Class that represent an image
class Image
{
    ///Free Image bitmap for this image
    FIBITMAP* bitmap = nullptr;

    ///Is free image initialized?
    static bool freeimageInitialized;

    ///Initialize the freeimage library. Called automatically one time
    static void initFreeImage()
    {
        //We love FreeImage, show that we are using it!
        std::cout << "This program uses Image.hpp. And Image.hpp uses FreeImage:\n";
        std::cout << FreeImage_GetCopyrightMessage() << '\n';

        //give a function to print the errors for free image
        FreeImage_SetOutputMessage([](FREE_IMAGE_FORMAT fif, const char* message) {
                std::cerr << "\n***";
                if(fif != FIF_UNKNOWN) std::cout << FreeImage_GetFormatFromFIF(fif) << " Format\n";
                std::cerr << message;
                std::cerr << " ***\n";
                });

        freeimageInitialized = true;
    }


    public:

    ///Return the mask to find the bits that correspound to a pixel
    struct ColorMask
    {
        unsigned R, G, B;
    };


    ///Construct an image into memory
    Image(int width, int height, int depth = 24)
    {
        if(!freeimageInitialized) initFreeImage();

        bitmap = FreeImage_Allocate(width, height, depth);

        if(!bitmap) throw ImageCantAllocateExcept();
        std::cout << "Allocated a " << width << 'x' << height << 'x' << depth << " bitmap image\n";
    }


    ///Load image from file
    Image(const std::string& path, FREE_IMAGE_FORMAT fif, int flags = 0)
    {
        if(!freeimageInitialized) initFreeImage();

        bitmap = FreeImage_Load(fif, path.c_str(), flags);

        if(!bitmap) throw ImageCantLoadExept(path);
        std::cout << "Loaded " << path << " as " << FreeImage_GetFormatFromFIF(fif) << " format\n";
    }


    ///non copyable
    Image(const Image&) = delete;

    ///non copy-asignable
    Image& operator=(const Image&) = delete;

    ///Move constructor
    Image(Image&& other) noexcept :
        bitmap(other.bitmap)
        {
            other.bitmap = nullptr;
        }

    ///Asignment-move operator
    Image& operator=(Image&& other) noexcept
    {
        //Steal guts of other object
        bitmap = other.bitmap;
        //Prevent other::~Image() from blowing up
        other.bitmap = nullptr;
        return *this;
    }

    ///Destructor. Will FreeImage_Unload() the bitmap
    ~Image()
    {
        if(bitmap)
        {
            FreeImage_Unload(bitmap);
            std::cout << "Unloaded image\n";
        }
    }

    ///Save image to file
    void save(const std::string& path, FREE_IMAGE_FORMAT fif, int flags = 0) const
    {
        std::cout << "Saving image into filepath : " << path << '\n';
        FreeImage_Save(fif, bitmap, path.c_str(), flags);
    }

    ///Return the type of the image
    FREE_IMAGE_TYPE type() const
    {
        assert(bitmap);
        return FreeImage_GetImageType(bitmap);
    }

    ///Return the color type of the pixels in the image
    FREE_IMAGE_COLOR_TYPE colorType() const
    {
        assert(bitmap);
        return FreeImage_GetColorType(bitmap);
    }

    ///Return the number of bit per pixels
    size_t depth() const
    {
        assert(bitmap);
        return FreeImage_GetBPP(bitmap);
    }

    ///Return the width of the image in pixels
    size_t width() const
    {
        assert(bitmap);
        return FreeImage_GetWidth(bitmap);
    }

    ///Return the height of the image in pixels
    size_t height() const
    {
        assert(bitmap);
        return FreeImage_GetHeight(bitmap);
    }

    ///Return the pitch/stride of the image
    size_t pitch() const
    {
        assert(bitmap);
        return FreeImage_GetPitch(bitmap);
    }

    ///Return the color at this position
    RGBQUAD pixelColor(size_t x, size_t y) const
    {
        assert(bitmap);
        assert(x < width());
        assert(y < height());
        RGBQUAD color;
        FreeImage_GetPixelColor(bitmap, static_cast<unsigned int>(x), static_cast<unsigned int>(y), &color);
        return color;
    }

    ///Set the color at this position
    void setPixelColor(size_t x, size_t y, const RGBQUAD& value)
    {
        assert(bitmap);
        assert(x < width());
        assert(y < height());
        FreeImage_SetPixelColor(bitmap, static_cast<unsigned int>(x), static_cast<unsigned int>(y), const_cast<RGBQUAD*>(&value));
    }

    ///get the array of bits that this image does
    BYTE* bits()
    {
        assert(bitmap);
        return FreeImage_GetBits(bitmap);
    }

    ///Get the 3 color masks
    ColorMask colorMask() const
    {
        return {
            FreeImage_GetRedMask(bitmap),
                FreeImage_GetGreenMask(bitmap),
                FreeImage_GetBlueMask(bitmap)
        };
    }

    ///Return width/height of the image
    float getRatio() const
    {
        return float(width()) / float(height());
    }
};

#ifdef IMAGE_PP_IMPLEMENTS
bool Image::freeimageInitialized = false;
#endif
