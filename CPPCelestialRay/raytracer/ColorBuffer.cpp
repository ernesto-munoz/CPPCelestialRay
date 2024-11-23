#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ColorBuffer.h"

ColorBuffer::ColorBuffer(int width, int height, int channels) : width(width), height(height), channels(channels) {
	raw_data = new unsigned char[width * height * channels];
}

ColorBuffer::~ColorBuffer() {
	Delete();
}

void ColorBuffer::Save(std::string output_image_path) {

	bool success = stbi_write_png(output_image_path.c_str(), width, height, channels, raw_data, width * 3);
	if (success) {
		std::cout << "ColorBuffer saved successfully: " << output_image_path << std::endl;
	}
	else {
		std::cerr << "Failed to save image." << std::endl;
	}
}

bool ColorBuffer::Load(std::string input_image_path)
{
	
	// laod with stbi_loadf instead stbi_load because the latter makes linear gamma correction
	float* float_data = stbi_loadf(input_image_path.c_str(), &width, &height, &channels, 0);
	//raw_data = stbi_load(input_image_path.c_str(), &width, &height, &channels, 0);

	if (float_data == nullptr) {
		std::cout << "ColorBuffer load failed: " << input_image_path << std::endl;
		return false;
	}

	Delete(); // delete previous data
	// create new data and assign values between 0..255
	raw_data = new unsigned char[width * height * channels];
	for (size_t i = 0; i < width * height * channels; i++)
		raw_data[i] = static_cast<unsigned char>(float_data[i] * 255);
	
	stbi_image_free(float_data);
	std::cout << "ColorBuffer loaded successfully: " << input_image_path << std::endl;
	std::cout << std::format("Info: ({}, {}, {})", width, height, channels) << std::endl;

	return true;
}

void ColorBuffer::Delete()
{
	delete[] raw_data;
	// Now, raw_data is only created with new

	// In C++, pointers allocated with new have their LSB set to 1,
	// while those allocated with malloc have their LSB set to 0.
	//if (raw_data != nullptr) {
	//	if (reinterpret_cast<char*>(raw_data)[0] & 0x01) {
	//		// allowcated with new
	//		delete[] raw_data;
	//	}
	//	else {
	//		// allowcated with malloc (the stbi_load function)
	//		stbi_image_free(raw_data);
	//	}
	//}
	
}

