// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Utils.h"
#include "port\port.h"

namespace LevelDBWinRT {
	std::string Utils::FromPlatformString(String^ str) {
		std::wstring ws(str->Begin());
		return leveldb::port::ws2s(ws);
	}

	String^ Utils::ToPlatformString(std::string str) {
		return ref new String(leveldb::port::s2ws(str).c_str());
	}

	COMException^ Utils::ExceptionFromStatus(int errorCode, leveldb::Status& status) {
		std::wstring err = leveldb::port::s2ws(status.ToString());
		String^ error = ref new String(err.c_str());
		return ref new COMException(errorCode, error);
	}
}