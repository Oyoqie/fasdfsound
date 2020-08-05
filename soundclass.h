#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

#include <dsound.h>

namespace SubChunks {
	enum {
		fmt = 0x20746d66,
		list = 0x5453494c,
		fact = 0x74636166,
		wavl = 0x746e6c73,
		data = 0x61746164,
	};
}

struct chk_wavfile {
	DWORD ChunkId;
	DWORD ChunkSize;

	BYTE *Data;

	BYTE* LoadData(const size_t n) {
		Data = (BYTE*)malloc(n);
		return Data;
	}

	~chk_wavfile() {
		if (Data)
			free(Data);
	}
};

struct hdr_wavfile {
	enum {
		riff = 0x46464952,
	} ChunkId;

	DWORD ChunkSize;

	enum {
		wave = 0x45564157,
	} ChunkType;

	DWORD fmtId;
	DWORD fmtSize;

	WORD  wFormatTag;
	WORD  nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD  nBlockAlign;
	WORD  wBitsPerSample;
};

#endif