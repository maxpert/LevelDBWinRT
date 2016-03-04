// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "leveldb\db.h"
#include "Slice.h"
#include <Windows.h>

using namespace Platform;

namespace LevelDBWinRT {
	
	public interface class IKeyComparator {
	public:
		int Compare(Slice^ a, Slice^ b);
		property String^ Name {
			String^ get();
		};
	};
}