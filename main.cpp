#include <windows.h>
#include <winuser.h>
#include <soundclass.h>
#include <stdio.h>

class Audio {
private:
	LPDIRECTSOUND ds;
	LPDIRECTSOUNDBUFFER dssb; //secondary sound buffer
	LPDIRECTSOUNDBUFFER dspb; //primary sound buffer

public:
	Audio() :	ds(NULL),
			dssb(NULL),
			dspb(NULL) {}

	~Audio() {}

	bool Init(HWND hWnd) {
		if (DirectSoundCreate(NULL, &ds, NULL) != DS_OK)
			return 1;

		if (ds->SetCooperativeLevel(hWnd, DSSCL_PRIORITY) != DS_OK)
			return 1;

		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
		dsbd.dwSize        = sizeof(DSBUFFERDESC);
		dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
		dsbd.dwBufferBytes = 0;
		dsbd.lpwfxFormat   = NULL;

		if (ds->CreateSoundBuffer(&dsbd, &dspb, NULL) != DS_OK)
			return 1;

		dspb->Release();

		return 0;
	}

	int CreateSecondaryBuffer(int channels, size_t sz, int samplerate, int bitspersample, DWORD flags) {
		WAVEFORMATEX wfx;
		DSBUFFERDESC dsbd;

		ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
		wfx.wFormatTag      = WAVE_FORMAT_PCM; 
		wfx.nChannels       = channels; 
		wfx.nSamplesPerSec  = samplerate; 
		wfx.wBitsPerSample  = bitspersample; 
		wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
								// number of bytes for each sample (16bit stereo=4, 8bit mono=1)
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

		ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
		dsbd.dwSize          = sizeof(DSBUFFERDESC);
		dsbd.dwFlags         = flags;
		dsbd.dwBufferBytes   = sz; //samplerate * bitspersample / 8 * channels * secs;
		dsbd.guid3DAlgorithm = GUID_NULL;
		dsbd.lpwfxFormat     = &wfx;

		if (ds->CreateSoundBuffer(&dsbd, &dssb, NULL) != DS_OK)
			return -1;

		return dsbd.dwBufferBytes;
	}

	bool LoadToBuffer(BYTE *bytes, size_t bfsz) {
		BYTE *pBufferBytes;
		DWORD lpdwAudioBytes;

		if (dssb->Lock(0, bfsz, (void**)&pBufferBytes, &lpdwAudioBytes, NULL, NULL, 0L) != DS_OK)
			return 1;

		memcpy(pBufferBytes, bytes, bfsz);

		dssb->Unlock(pBufferBytes, lpdwAudioBytes, NULL, NULL);

		return 0;
	}

	void Play() {
		dssb->Play(0, 0, 0);
	}

	void Stop() {
		dssb->Stop();
	}
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int main(int argc, char **argv) {
	HINSTANCE hInst = GetModuleHandle(NULL);

	WNDCLASS Window;
	ZeroMemory(&Window, sizeof(WNDCLASS));
	Window.lpfnWndProc = WindowProc;
	Window.hInstance = hInst;
	Window.lpszClassName = "ClassName";
	RegisterClass(&Window);

	HWND hWnd;
	hWnd = CreateWindow("ClassName", "Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);

	Audio audio;
	FILE *file;
	hdr_wavfile hdr;
	chk_wavfile chk;

	ShowWindow(hWnd, SW_SHOW);

	if (fopen_s(&file, "sound01.wav", "rb") == 0) {
		printf("Opened sound01.wav\n");

		fread(&hdr, sizeof(hdr_wavfile), 1, file);
		fread(&chk, sizeof(chk_wavfile) - 4, 1, file);

		if (chk.ChunkId == SubChunks::list) {
			fseek(file, chk.ChunkSize, SEEK_CUR);
			fread(&chk, sizeof(chk_wavfile) - 4, 1, file);
		}else if (chk.ChunkId == SubChunks::data) {
			chk.LoadData(chk.ChunkSize);
			fread(chk.Data, chk.ChunkSize, 1, file);
		}

		fclose(file);

		printf("Audio::Init %d\n",
			audio.Init(hWnd));
		printf("Audio::CreateSecondaryBuffer %d\n",
			audio.CreateSecondaryBuffer(hdr.nChannels, chk.ChunkSize, hdr.nSamplesPerSec, hdr.wBitsPerSample, 0));
		printf("Audio::LoadToBuffer %d\n", audio.LoadToBuffer(chk.Data, chk.ChunkSize));

		audio.Play();

		printf("Audio::Play\n");
	}

	MSG msg;

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
