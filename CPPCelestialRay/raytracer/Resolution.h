#pragma once
#include <vector>
#include <string>
#include <map>


class Resolution
{
public:
	enum kResolution{
		kResolution_400x225,
		kResolution_1920x1080,
		kResolution_1280x720,
		kResolution_1080x1080,
		kResolution_512x512,
		kResolution_256x256
	};
	unsigned int width, height;

	static const std::vector<kResolution> GetAllResolutions() {
		return { kResolution_400x225, kResolution_1920x1080, kResolution_1280x720, kResolution_1080x1080, kResolution_512x512, kResolution_256x256 };
	}

	static const std::map<kResolution, const char*> kResolutionsNiceName;

	Resolution(unsigned int width, unsigned int height) : width(width), height(height) {};

	Resolution(kResolution res) {
		switch (res)
		{
		case Resolution::kResolution_400x225:
			width = 400; height = 225;
			break;
		case Resolution::kResolution_1920x1080:
			width = 1920; height = 1080;
			break;
		case Resolution::kResolution_1280x720:
			width = 1280; height = 720;
			break;
		case Resolution::kResolution_1080x1080:
			width = 1080; height = 1080;
			break;
		case Resolution::kResolution_512x512:
			width = 512; height = 512;
			break;
		case Resolution::kResolution_256x256:
			width = 256; height = 256;
			break;
		default:
			width = 256; height = 256;
			break;
		}
		
	}
};

