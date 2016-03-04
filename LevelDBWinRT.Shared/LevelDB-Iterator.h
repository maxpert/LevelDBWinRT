// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "Slice.h"
#include "leveldb\db.h"

namespace LevelDBWinRT {
	public ref class Iterator sealed
	{
	private:
		leveldb::Iterator* iterator;
	internal:
		Iterator(leveldb::Iterator *i) {
			this->iterator = i;
		}

	public:
		Iterator() {
			this->iterator = leveldb::NewEmptyIterator();
		}

		virtual ~Iterator() {
			delete this->iterator;
		}

		Slice^ Key() {
			return ref new Slice(this->iterator->key());
		}

		void Next() {
			this->iterator->Next();
		}

		void Prev() {
			this->iterator->Prev();
		}

		void Seek(Slice^ target) {
			this->iterator->Seek(target->ToLevelDBSlice());
		}

		void SeekToFirst() {
			this->iterator->SeekToFirst();
		}

		void SeekToLast() {
			this->iterator->SeekToLast();
		}

		bool Valid() {
			return this->iterator->Valid();
		}

		Slice^ Value() {
			return ref new Slice(this->iterator->value());
		}
	};
}