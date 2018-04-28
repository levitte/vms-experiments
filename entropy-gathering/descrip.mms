.IFDEF DEBUG
CFLAGS = /DEFINE=DEBUG/DEBUG/NOOPT/NOLIST/OBJECT=$@
LINKFLAGS = /DEBUG/TRACE/NOMAP/EXEC=$@
.ENDIF

.default :
	@ !

all : entropy-stats.exe entropy-hash.exe check-dict.exe

entropy-stats.exe : entropy-stats.obj dict.obj entropy.obj
	link $(LINKFLAGS) $+
entropy-hash.exe : entropy-hash.obj dict.obj entropy.obj
	link $(LINKFLAGS) $+,entropy-hash.opt/opt
check-dict.exe : check-dict.obj dict.obj
	link $(LINKFLAGS) $+

entropy-stats.obj : entropy-stats.c dict.h entropy.h
entropy-hash.obj : entropy-hash.c dict.h entropy.h
dict.obj : dict.c dict.h
entropy.obj : entropy.c entropy.h

check-dict.obj : check-dict.c dict.h