#include "Slice.h"
#include "port\port.h"

using namespace Platform;
using namespace Platform::Runtime::InteropServices;

namespace LevelDBWinRT {
	Slice::Slice(leveldb::Slice& slice) {
		this->buffer = ref new Array<uint8>((int)slice.size());
		memcpy_s(this->buffer->Data, this->buffer->Length, slice.data(), this->buffer->Length);
	}

	Slice::Slice(const Array<uint8>^ m) {
		this->buffer = ref new Array<uint8>(m->Length);
		memcpy_s(this->buffer->Data, this->buffer->Length, m->Data, this->buffer->Length);
	}

	Slice::Slice(Slice^ obj) {
		this->buffer = obj->buffer;
	}

	Slice::Slice(String^ str) {
		std::wstring ws(str->Begin());
		this->buffer = ref new Array<uint8>(ws.length() * sizeof(wchar_t));
		memcpy_s(this->buffer->Data, this->buffer->Length, ws.c_str(), this->buffer->Length);
	}

	leveldb::Slice Slice::ToLevelDBSlice() const {
		leveldb::Slice ret((const char *)this->buffer->Data, this->buffer->Length);
		return ret;
	}

	Slice^ Slice::FromString(String^ s) {
		return ref new Slice(s);
	}

	Slice^ Slice::FromByteArray(const Array<uint8>^ b) {
		return ref new Slice(b);
	}
}