// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DB.h"
#include "Utils.h"
#include "port\port.h"

namespace LevelDBWinRT {
	DB::DB(Options^ options, String^ path) {
		leveldb::Status status = leveldb::DB::Open(options->ToLevelDBOptions(), Utils::FromPlatformString(path), &this->db);

		if (!status.ok()) {
			throw Utils::ExceptionFromStatus(0, status);
		}
	}

	DB::~DB() {
		if (this->db != NULL) {
			delete this->db;
			this->db = NULL;
		}
	}

	bool DB::Put(WriteOptions^ writeOptions, Slice^ key, Slice^ value) {
		return this->db->Put(writeOptions->ToLevelDBOptions(), key->ToLevelDBSlice(), value->ToLevelDBSlice()).ok();
	}

	bool DB::Delete(WriteOptions^ writeOptions, Slice^ key) {
		return this->db->Delete(writeOptions->ToLevelDBOptions(), key->ToLevelDBSlice()).ok();
	}

	Slice^ DB::Get(ReadOptions^ readOptions, Slice^ key) {
		leveldb::Iterator* itr = this->db->NewIterator(readOptions->ToLevelDBOptions());

		if (itr == NULL) {
			return nullptr;
		}

		Slice^ valueSlice = nullptr;
		leveldb::Slice searchKey = key->ToLevelDBSlice();
		itr->Seek(searchKey);
		
		// If key is exactly same then return wrapped slice object
		if (itr->Valid()) {
			leveldb::Slice k = itr->key();
			if (k.size() == searchKey.size() && memcmp(k.data(), searchKey.data(), k.size()) == 0) {
				leveldb::Slice s = itr->value();
				valueSlice = ref new Slice(s);
			}
		}

		delete itr;
		return valueSlice;
	}

	Iterator^ DB::NewIterator(ReadOptions^ opts) {
		return ref new Iterator(
			this->db->NewIterator(opts->ToLevelDBOptions())
		);
	}

	Snapshot^ DB::GetSnapshot() {
		return ref new Snapshot(
			this->db->GetSnapshot()
		);
	}

	void DB::ReleaseSnapshot(Snapshot^ s) {
		this->db->ReleaseSnapshot(s->snapshot);
	}

	bool DB::Write(WriteOptions^ writeOptions, WriteBatch^ batch) {
		leveldb::WriteBatch* wbatch = batch->ToLevelDBWriteBatch();
		return this->db->Write(writeOptions->ToLevelDBOptions(), wbatch).ok();
	}
}
