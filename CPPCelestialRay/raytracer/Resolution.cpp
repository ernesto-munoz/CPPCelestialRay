#include "Resolution.h"

const std::map<Resolution::kResolution, const char*> Resolution::kResolutionsNiceName = {
	{kResolution_400x225, "400x225"},
	{kResolution_1920x1080, "1920x1080"},
	{kResolution_1280x720, "1280x720"},
	{kResolution_1080x1080, "1080x1080"},
	{kResolution_512x512, "512x512"},
	{kResolution_256x256, "256x256"},
};