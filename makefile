WIN_LIBS=dsound.lib dxguid.lib winmm.lib user32.lib
CFLAGS=/Ox /W3 /nologo /openmp
main_libs=advapi32.lib iphlpapi.lib $(WIN_LIBS)
#soundclass.obj

startup: main

main:
	$(CC) $(CFLAGS) /I. main.cpp $(main_libs)

#soundclass:
#	$(CC) $(CFLAGS) /c /I. soundclass.cpp
