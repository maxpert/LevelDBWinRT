// Copyright (c) 2015 Zohaib Sibte Hassan. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once
#include "snappy\snappy.h"
#include <Windows.h>

using namespace Platform;
using namespace Platform::Runtime;

namespace LevelDBWinRT {
	namespace Snappy {
		public ref class ArrayBuffer sealed {
		private:
			Array<uint8>^ buffer;
		internal:
			ArrayBuffer(char *buffer, size_t bufferLength) {
				this->buffer = ref new Array<uint8>(bufferLength);
				memcpy(this->buffer->Data, buffer, bufferLength);
			}
		public:
			Array<uint8>^ GetBytes() {
				return this->buffer;
			}

		};

		public ref class Snappy sealed {
		public:
			static ArrayBuffer^ Compress(const Array<uint8>^ data) {
				char *compressed_out = new char[snappy::MaxCompressedLength(data->Length)];
				size_t compressed_len = 0;

				snappy::RawCompress((const char *)data->Data, data->Length, compressed_out, &compressed_len);
				auto ret = ref new ArrayBuffer(compressed_out, compressed_len);
				delete[] compressed_out;

				return ret;
			}

			static ArrayBuffer^ Uncompress(const Array<uint8> ^data) {
				size_t uncompressed_length = 0;
				if (!snappy::GetUncompressedLength((const char*)data->Data, data->Length, &uncompressed_length)) {
					return nullptr;
				}

				char *uncompressed = new char[uncompressed_length];
				if (!snappy::RawUncompress((const char *)data->Data, data->Length, uncompressed)) {
					delete[] uncompressed;
					return nullptr;
				}

				auto ret = ref new ArrayBuffer(uncompressed, uncompressed_length);
				delete[] uncompressed;

				return ret;
			}
		};
	}
}