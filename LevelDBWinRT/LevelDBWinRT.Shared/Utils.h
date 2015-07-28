#pragma once
#include <string>
#include "leveldb\status.h"

namespace LevelDBWinRT {
	using namespace Platform;

	ref class Utils sealed {
	internal:
		static std::string FromPlatformString(String^ str);
		static String^ ToPlatformString(std::string str);

		static COMException^ ExceptionFromStatus(int errorCode, leveldb::Status& status);
	};
}