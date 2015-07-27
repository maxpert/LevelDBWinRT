#pragma once

#include "leveldb\slice.h"
#include <Windows.h>

using namespace Platform;
using namespace Platform::Runtime;

namespace LevelDBWinRT {
	public ref class Slice sealed : public Platform::Object {
	internal:
		Array<uint8>^ buffer;
	internal:
		Slice(leveldb::Slice& slice);
		Slice(const Array<uint8>^ bytes);
		Slice(String^ str);

		leveldb::Slice ToLevelDBSlice() const;

	public:
		Slice(Slice^ obj);

		static Slice^ FromString(String^ s);
		static Slice^ FromByteArray(const Array<uint8>^ b);
	};
}