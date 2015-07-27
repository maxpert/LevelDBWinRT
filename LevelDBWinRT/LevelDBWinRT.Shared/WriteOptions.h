#pragma once
#include "leveldb\options.h"

namespace LevelDBWinRT {
	public ref class WriteOptions sealed {
	internal:
		leveldb::WriteOptions ToLevelDBOptions();
	public:
		property bool Sync;

		WriteOptions(void);
	};
}