// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once
#include "snappy\snappy.h"
#include <Windows.h>

using namespace Platform;
using namespace Platform::Runtime;

namespace LevelDBWinRT {
	namespace Snappy {
		public ref class ArrayBuffer sealed {
		private:
			size_t bufferLength;
			char* buffer;
		public:
			Array<uint8>^ GetBytes();
		};
	}
}