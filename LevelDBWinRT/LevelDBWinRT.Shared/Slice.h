// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "leveldb\slice.h"
#include <Windows.h>

using namespace Platform;
using namespace Platform::Runtime;

namespace LevelDBWinRT {
	public ref class Slice sealed : public Platform::Object {
	internal:
		Array<uint8>^ buffer;
	internal:
		Slice(const leveldb::Slice& slice);
		Slice(const Array<uint8>^ bytes);
		Slice(String^ str);

		leveldb::Slice ToLevelDBSlice() const;

	public:
		Slice(Slice^ obj);
		Array<uint8>^ ToByteArray();
		String^ AsString();

		static Slice^ FromString(String^ s);
		static Slice^ FromByteArray(const Array<uint8>^ b);
	};
}