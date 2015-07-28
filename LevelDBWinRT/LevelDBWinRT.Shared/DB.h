#pragma once

#include "leveldb\db.h"
#include "Slice.h"
#include "LevelDB-Iterator.h"
#include "LevelDB-Commons.h"
#include <Windows.h>

namespace LevelDBWinRT {
	using namespace Platform;

	public ref class DB sealed
	{
	private:
		leveldb::DB* db;
	public:
		DB(String^ path);
		virtual ~DB();

		bool Put(WriteOptions^ writeOptions, Slice^ key, Slice^ value);
		bool Delete(WriteOptions^ writeOptions, Slice^ key);
		Slice^ Get(ReadOptions^ readOptions, Slice^ key);

		Iterator^ NewIterator(ReadOptions ^readOptions);
		Snapshot^ GetSnapshot();
		void ReleaseSnapshot(Snapshot^ snapshot);
	};
}

