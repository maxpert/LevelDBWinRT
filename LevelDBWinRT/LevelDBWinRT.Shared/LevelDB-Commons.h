#pragma once
#include "leveldb\options.h"
#include <Windows.h>

namespace LevelDBWinRT {

	public ref class Snapshot sealed {
	internal:
		const leveldb::Snapshot *snapshot;
		Snapshot(const leveldb::Snapshot *s) {
			this->snapshot = s;
		}
	};

	public ref class WriteOptions sealed {
	internal:
		leveldb::WriteOptions ToLevelDBOptions() {
			leveldb::WriteOptions opts;
			opts.sync = this->Sync;
			return opts;
		}
	public:
		property bool Sync;

		WriteOptions(void) {
			this->Sync = false;
		}
	};

	public ref class ReadOptions sealed {
	internal:
		leveldb::ReadOptions ToLevelDBOptions() {
			leveldb::ReadOptions opts;
			opts.fill_cache = this->FillCache;
			opts.verify_checksums = this->VerifyChecksums;
			opts.snapshot = this->Snapshot->snapshot;
			return opts;
		}
	public:
		property bool VerifyChecksums;
		property bool FillCache;
		property Snapshot^ Snapshot;

		ReadOptions() {
			this->VerifyChecksums = false;
			this->FillCache = true;
			this->Snapshot = ref new LevelDBWinRT::Snapshot(NULL);
		}
	};
}
