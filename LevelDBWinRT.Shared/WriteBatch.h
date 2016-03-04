// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "Slice.h"
#include "leveldb\write_batch.h"

namespace LevelDBWinRT {
	public ref class WriteBatch sealed {
	private:
		leveldb::WriteBatch* batch;

	internal:
		leveldb::WriteBatch* ToLevelDBWriteBatch();

	public:
		WriteBatch();
		virtual ~WriteBatch();

		void Put(Slice^ key, Slice^ value);
		void Delete(Slice^ key);
		void Clear();
	};
}
