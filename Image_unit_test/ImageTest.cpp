#include <Image.hpp>
#include <catch.hpp>
#include <ctime>
#include <fstream>

TEST_CASE("Image_create_blank")
{
	//Create a 8x8 24bit image
	Image image{ 8, 8 };
	REQUIRE(image.bits() != nullptr);
}

TEST_CASE("Image_load_file")
{
	Image Lenna("./Lenna.png", FIF_PNG);
	REQUIRE(Lenna.bits());
}

TEST_CASE("Image_save_file")
{
	Image image{ 128, 128 };
	//ctor did not throw

	//generate filename
	srand(42);
	const std::string filename = [len = 20]
	{
		std::string s;
		for (size_t i{0}; i < len; ++i)
			s.push_back(char(rand() % 2 ? (rand() % 26) + 65 : (rand() % 26) + 97));
		return s + ".png";
	}();

	//save
	image.save(filename, FIF_PNG);

	//Check for existance of file
	std::ifstream file(filename);
	REQUIRE(file);
}

TEST_CASE("Image_load_check_content")
{
	Image green("./green.bmp", FIF_BMP);
	for (size_t y{ 0 }; y < 2; y++)
		for (size_t x{ 0 }; x < 2; x++)
		{
			const auto pixel = green.pixelColor(x, y);
			REQUIRE(pixel.rgbBlue == 0);
			REQUIRE(pixel.rgbGreen == 255);
			REQUIRE(pixel.rgbRed == 0);
		}
}

TEST_CASE("Image_write_reload_check")
{
	Image newImage(5, 5);
	const RGBQUAD pureRed = []
	{
		RGBQUAD color;
		color.rgbRed = 255;
		color.rgbBlue = 0;
		color.rgbGreen = 0;
		return color;
	}();

	for (size_t x{ 0 }; x < 5; ++x)
		for (size_t y{ 0 }; y < 5; ++y)
			newImage.setPixelColor(x, y, pureRed);
	newImage.save("red.bmp", FIF_BMP);

	Image red("./red.bmp", FIF_BMP);
	for (size_t x{ 0 }; x < 5; ++x)
		for (size_t y{ 0 }; y < 5; ++y)
		{
			const auto pixel = red.pixelColor(x, y);
			REQUIRE(pixel.rgbBlue == 0);
			REQUIRE(pixel.rgbGreen == 0);
			REQUIRE(pixel.rgbRed == 255);
		}
}
