#pragma once
#include <string>

namespace LevelDBWinRT {
	using namespace Platform;

	ref class Utils sealed {
	internal:
		static std::string FromPlatformString(String^ str);
		static String^ ToPlatformString(std::string str);
	};
}