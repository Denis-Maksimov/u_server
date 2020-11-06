# --no-print-directory
ifndef platform
  platform=windows
endif



TARGET:=run
LDFLAGS= -lu_net 

#----------Linux-----------
ifeq ($(platform),linux)

  ifndef GCC
    GCC :=gcc
  endif

  ifndef GPP
    GPP :=g++
  endif

  ifndef AR
    AR  :=ar
  endif

  TARGET:=run

endif
#----------WINDOWS-----------
ifeq ($(platform),windows)
 ifndef GCC
    GCC :=x86_64-w64-mingw32-gcc
  endif

  ifndef GPP
    GPP :=x86_64-w64-mingw32-g++
  endif

  ifndef AR
    AR  :=x86_64-w64-mingw32-ar
  endif
  
  LDFLAGS += -static-libgcc -static-libstdc++ -lwinmm -lws2_32 
  TARGET:=run.exe
endif



export platform




all: $(TARGET)

$(TARGET): libu_net.a
	$(GCC) ./t.c -I./inc/server -L. $(LDFLAGS) -o $@

libu_net.a:
	${MAKE} -C server


	
clean: 
	${MAKE} -C server $@ 

help:
	@echo "Использование:"
	@echo "make [platform=windows|linux]"
	@echo "По умолчанию собирается для винды"


