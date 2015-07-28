#include "DB.h"
#include "Utils.h"
#include "port\port.h"

namespace LevelDBWinRT {
	DB::DB(String^ path) {
		leveldb::Options opts;
		opts.create_if_missing = true;
		leveldb::Status status = leveldb::DB::Open(opts, Utils::FromPlatformString(path), &this->db);
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

	Slice^ DB::Get(Slice^ key) {
		std::string str;

		leveldb::Iterator* itr = this->db->NewIterator(leveldb::ReadOptions());

		if (itr == NULL) {
			return nullptr;
		}

		Slice^ valueSlice = nullptr;
		itr->Seek(key->ToLevelDBSlice());

		if (itr->Valid()) {
			leveldb::Slice s = itr->value();
			valueSlice = ref new Slice(s);
		}

		delete itr;
		return valueSlice;
	}

	Iterator^ DB::NewIterator() {
		return ref new Iterator(this->db->NewIterator(leveldb::ReadOptions()));
	}
}
