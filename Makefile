ifeq ($(SCIDB),) 
  X := $(shell which scidb 2>/dev/null)
  ifneq ($(X),)
    X := $(shell dirname ${X})
    SCIDB := $(shell dirname ${X})
  endif
endif

# A development environment will have SCIDB_VER defined, and SCIDB
# # will not be in the same place... but the 3rd party directory *will*
# # be, so build it using SCIDB_VER .
ifeq ($(SCIDB_VER),)
  SCIDB_3RDPARTY = $(SCIDB)
else
  SCIDB_3RDPARTY = /opt/scidb/$(SCIDB_VER)
endif

ifeq ($(SCIDB_THIRDPARTY_PREFIX),) 
  SCIDB_THIRDPARTY_PREFIX := $(SCIDB_3RDPARTY)
endif

CFLAGS=-pedantic -W -Wextra -Wall -Wno-variadic-macros -Wno-strict-aliasing -Wno-long-long -Wno-unused-parameter -fPIC -D_STDC_FORMAT_MACROS -Wno-system-headers -isystem -O2 -g -DNDEBUG -ggdb3  -D_STDC_LIMIT_MACROS
INC=-I. -DPROJECT_ROOT="\"$(SCIDB)\"" -I"$(SCIDB_THIRDPARTY_PREFIX)/3rdparty/boost/include/" -I"$(SCIDB)/include"

LIBS=-shared -Wl,-soname,libexec.so -L. -L"$(SCIDB_THIRDPARTY_PREFIX)/3rdparty/boost/lib" -L"$(SCIDB)/lib" -Wl,-rpath,$(SCIDB)/lib:$(RPATH) -lm

all:
	@if test ! -d "$(SCIDB)"; then echo  "Error. Try:\n\nmake SCIDB=<PATH TO SCIDB INSTALL PATH>"; exit 1; fi
	$(CXX) $(CFLAGS) $(INC) -o libexec.so  plugin.cpp exec.cpp $(LIBS)
	@echo "Now copy libexec.so to your SciDB lib/scidb/plugins directory and restart SciDB."

clean:
	rm -f *.so *.o
