// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "leveldb\db.h"
#include "Slice.h"
#include "LevelDB-Iterator.h"
#include "LevelDB-Commons.h"
#include "WriteBatch.h"
#include <Windows.h>

namespace LevelDBWinRT {
	using namespace Platform;

	public ref class DB sealed
	{
	private:
		leveldb::DB* db;
		leveldb::Comparator* comparator;
	public:
		DB(Options^ options, String^ path);
		virtual ~DB();

		bool Put(WriteOptions^ writeOptions, Slice^ key, Slice^ value);
		bool Delete(WriteOptions^ writeOptions, Slice^ key);
		Slice^ Get(ReadOptions^ readOptions, Slice^ key);
		bool Write(WriteOptions^ writeOptions, WriteBatch^ batch);

		Iterator^ NewIterator(ReadOptions ^readOptions);
		Snapshot^ GetSnapshot();
		void ReleaseSnapshot(Snapshot^ snapshot);
	};
}

