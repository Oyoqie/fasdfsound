#include <soundclass.h>

chk_wavfile::chk_wavfile() : Data(NULL) {
}

chk_wavfile::~chk_wavfile() {
	if (Data)
		free(Data);
}

BYTE* chk_wavfile::LoadData(const size_t n) {
	Data = (BYTE*)malloc(n);
	return Data;
}
