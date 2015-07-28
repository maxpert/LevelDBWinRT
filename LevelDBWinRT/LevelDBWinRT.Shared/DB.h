#pragma once

#include "leveldb\db.h"
#include "Slice.h"
#include "LevelDB-Iterator.h"
#include "WriteOptions.h"
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

		bool Put(WriteOptions^ writeopts, Slice^ key, Slice^ value);
		Slice^ Get(Slice^ key);
		Iterator^ NewIterator();
	};
}

