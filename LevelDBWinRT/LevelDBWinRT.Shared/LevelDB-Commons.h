// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "leveldb\options.h"
#include "leveldb\cache.h"
#include "leveldb\filter_policy.h"
#include <Windows.h>

namespace LevelDBWinRT {

	public enum class FilterType {
		None = 0,
		BloomFilter = 1,
	};

	public enum class CompressorType {
		None = 0,
		Snappy = 1,
	};

	public interface class IFilterParams {
	};

	public ref class BloomFilterParams sealed : public IFilterParams {
	public:
		property int BitsPerKey;
	};

	public ref class Options sealed {
	internal:
		leveldb::Options ToLevelDBOptions() {
			leveldb::Options opts;
			opts.create_if_missing = this->CreateIfMissing;
			opts.error_if_exists = this->ErrorIfExists;
			opts.paranoid_checks = this->ParanoidChecks;
			opts.write_buffer_size = this->WriteBufferSize;
			opts.max_open_files = this->MaximumOpenFiles;
			opts.block_size = this->BlockSize;
			opts.block_restart_interval = this->BlockRestartInterval;

			opts.block_cache = leveldb::NewLRUCache(this->BlockCacheSize);

			if (this->Filter == FilterType::BloomFilter) {
				try {
					BloomFilterParams^ params = safe_cast<BloomFilterParams^>(this->FilterParameters);
					opts.filter_policy = leveldb::NewBloomFilterPolicy(params->BitsPerKey);
				}
				catch (InvalidCastException^) {
					throw ref new InvalidArgumentException(L"BloomFilter must pass BloomFilterParams in FilterParameters field.");
				}
			}

			switch(this->Compressor) {
			case CompressorType::Snappy:
				opts.compression = leveldb::CompressionType::kSnappyCompression;
				break;
			default:
				opts.compression = leveldb::CompressionType::kNoCompression;
				break;
			}

			return opts;
		}
	public:
		property bool			CreateIfMissing;
		property bool			ErrorIfExists;
		property bool			ParanoidChecks;
		property uint32			WriteBufferSize;
		property int			MaximumOpenFiles;
		property uint32			BlockCacheSize;
		property uint32			BlockSize;
		property int			BlockRestartInterval;
		
		property FilterType		Filter;
		property IFilterParams^	FilterParameters;

		property CompressorType	Compressor;

		Options() {
			this->CreateIfMissing = false;
			this->ErrorIfExists = false;
			this->ParanoidChecks = false;

			this->WriteBufferSize = 4 << 20;
			this->MaximumOpenFiles = 1024;
			this->BlockCacheSize = 8 << 20;
			this->BlockSize = 4 << 10;
			this->BlockRestartInterval = 16;
			
			this->Filter = FilterType::None;
			this->Compressor = CompressorType::None;
		}
	};

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
