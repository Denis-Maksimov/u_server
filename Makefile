# --no-print-directory
ifndef platform
  platform=linux
endif



TARGET:=run
LDFLAGS= -lu_net -lu_proto 

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
  
  LIB_FOLDER=$(PWD)/lib/linux
  BINFOLDER:=$(PWD)/obj-linux/
  TARGET:=bin/run

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

  LIB_FOLDER=$(PWD)/lib/win
  BINFOLDER:=$(PWD)/obj-win/

  LDFLAGS += -static-libgcc -static-libstdc++ -lwinmm -lws2_32 
  TARGET:=bin/run.exe
endif

INCLUDES:= -I./inc/server -I./inc/proto 

export platform
export LIB_FOLDER
export BINFOLDER
# export PWD


all: $(TARGET)

$(TARGET): $(LIB_FOLDER)/libu_net.a $(LIB_FOLDER)/libu_proto.a
	$(GCC) ./t.c $(INCLUDES) -L$(LIB_FOLDER) $(LDFLAGS) -o $@
	@size ${TARGET}

$(LIB_FOLDER)/libu_net.a:
	${MAKE} -C server

$(LIB_FOLDER)/libu_proto.a:
	${MAKE} -C proto
	
clean: 
	${MAKE} -C server $@ 
	${MAKE} -C proto $@ 
	rm -f ${TARGET}

help:
	@echo "Использование:"
	@echo "make [platform=windows|linux]"
	@echo "По умолчанию собирается для винды"


