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

	chk_wavfile::chk_wavfile();
	chk_wavfile::~chk_wavfile();
	BYTE* LoadData(const size_t n);
	BYTE *Data;
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
