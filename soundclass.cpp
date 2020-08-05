#include <soundclass.h>

BYTE* chk_wavfile::LoadData(const size_t n) {
	Data = (BYTE*)malloc(n);
	return Data;
}

chk_wavfile::~chk_wavfile() {
	if (Data)
		free(Data);
}