#pragma once
#include <iostream>
#include <string>
#include <format>
#include <stb_image.h>
#include <stb_image_write.h>


class ColorBuffer
{
public:
	unsigned char* raw_data = nullptr;
	int width;
	int height;
	int channels = 3; // components

	ColorBuffer(int width, int height, int channels = 3);
	ColorBuffer(std::string input_image_path) { Load(input_image_path); }
	~ColorBuffer();
	void Save(std::string output_image_path);
	bool Load(std::string input_image_path);
private:
	void Delete();

};

