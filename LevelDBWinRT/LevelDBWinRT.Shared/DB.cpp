// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DB.h"
#include "Utils.h"
#include "leveldb\comparator.h"
#include "KeyComparator.h"
#include "port\port.h"
#include <Windows.h>

using Windows::Storage::ApplicationData;
using Windows::ApplicationModel::Package;

namespace LevelDBWinRT {

	class LevelDBComparatorWrapper : public leveldb::Comparator {
	private:
		IKeyComparator^ comparator;
		std::string name;

	public:
		LevelDBComparatorWrapper(IKeyComparator^ c) {
			std::wstring wsname(c->Name->Data());

			this->comparator = c;
			this->name = leveldb::port::ws2s(wsname);
		}

		virtual ~LevelDBComparatorWrapper() {
			this->comparator = nullptr;
		}

		int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const {
			try 
			{
				return this->comparator->Compare(ref new Slice(a), ref new Slice(b));
			}
			catch (Platform::Exception^ e) {
				String^ detailMessage = L"Comparator raised an exception. \n";
				detailMessage = String::Concat(detailMessage, e->ToString());
				throw ref new COMException(E_ABORT, detailMessage);
			}
		}

		const char* Name() const {
			return this->name.c_str();
		}

		void FindShortestSeparator(std::string* p, const leveldb::Slice& s) const {
			return;
		}

		void FindShortSuccessor(std::string* p) const {
			return;
		}
	};

	DB::DB(Options^ options, String^ path) {
		this->openOptions = options->ToLevelDBOptions();

		if (options->Comparator != nullptr) {
			this->openOptions.comparator = this->comparator = new LevelDBComparatorWrapper(options->Comparator);
		}
		else {
			this->comparator = NULL;
		}
		
		// Make sure the path provided is absolute path like C:\ or D:\ otherwise 
		// append the localfolder path as prefix to whatever is passed in path
		std::wstring wpath(path->Data());
		if (wpath.find(L":\\") != 1) {
			ApplicationData^ currentAppData = ApplicationData::Current;
			path = ref new String(currentAppData->LocalFolder->Path->Data());
			path = String::Concat(path, L"\\");
			path = String::Concat(path, ref new String(wpath.c_str()));
		}

		leveldb::Status status = leveldb::DB::Open(this->openOptions, Utils::FromPlatformString(path), &this->db);

		if (!status.ok()) {
			throw Utils::ExceptionFromStatus(E_FAIL, status);
		}
	}

	DB::~DB() {
		if (this->db != NULL) {
			delete this->db;
			this->db = NULL;
		}

		if (this->comparator != NULL) {
			delete this->comparator;
			this->comparator = NULL;
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
			if (k.size() == searchKey.size() && this->openOptions.comparator->Compare(k, searchKey) == 0) {
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
