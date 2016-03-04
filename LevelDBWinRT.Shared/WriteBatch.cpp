// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "WriteBatch.h"

namespace LevelDBWinRT {
	WriteBatch::WriteBatch() {
		this->batch = new leveldb::WriteBatch();
	}

	WriteBatch::~WriteBatch() {
		delete this->batch;
	}

	void WriteBatch::Put(Slice^ key, Slice^ value) {
		if (key == nullptr || value == nullptr) {
			throw ref new InvalidArgumentException(L"key or value must not be null");
		}

		this->batch->Put(key->ToLevelDBSlice(), value->ToLevelDBSlice());
	}

	void WriteBatch::Delete(Slice^ key) {
		if (key == nullptr) {
			throw ref new InvalidArgumentException(L"key must not be null");
		}

		this->batch->Delete(key->ToLevelDBSlice());
	}

	void WriteBatch::Clear() {
		this->batch->Clear();
	}

	leveldb::WriteBatch* WriteBatch::ToLevelDBWriteBatch() {
		return this->batch;
	}
}