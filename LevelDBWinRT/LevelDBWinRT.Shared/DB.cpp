#include "DB.h"
#include "Utils.h"

namespace LevelDBWinRT {
	DB::DB(String^ path) {
		leveldb::Options opts;
		opts.create_if_missing = true;
		leveldb::Status status = leveldb::DB::Open(opts, Utils::FromPlatformString(path), &this->db);
		if (!status.ok()) {
			throw ref new COMException(1);
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
}
