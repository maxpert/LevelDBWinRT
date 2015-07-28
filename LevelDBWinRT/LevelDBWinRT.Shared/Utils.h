// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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