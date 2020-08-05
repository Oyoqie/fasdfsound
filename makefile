WIN_LIBS=dsound.lib dxguid.lib winmm.lib user32.lib
CFLAGS=/Ox /W3 /nologo /openmp
main_libs=soundclass.obj advapi32.lib iphlpapi.lib $(WIN_LIBS)

startup: main

main: soundclass
	$(CC) $(CFLAGS) /I. main.cpp $(main_libs)

soundclass:
	$(CC) $(CFLAGS) /c /I. soundclass.cpp
