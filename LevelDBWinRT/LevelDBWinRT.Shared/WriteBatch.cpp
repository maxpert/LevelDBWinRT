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
		this->batch->Put(key->ToLevelDBSlice(), value->ToLevelDBSlice());
	}

	void WriteBatch::Delete(Slice^ key) {
		this->batch->Delete(key->ToLevelDBSlice());
	}

	void WriteBatch::Clear() {
		this->batch->Clear();
	}

	leveldb::WriteBatch* WriteBatch::ToLevelDBWriteBatch() {
		return this->batch;
	}
}