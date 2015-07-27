#include <WriteOptions.h>

namespace LevelDBWinRT {
	leveldb::WriteOptions WriteOptions::ToLevelDBOptions() {
		leveldb::WriteOptions opts;
		opts.sync = this->Sync;
		return opts;
	}

	WriteOptions::WriteOptions() {
		this->Sync = false;
	}
}