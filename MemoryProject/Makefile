ifeq (, $(shell sst-config))

CXX=g++
CXXFLAGS=-g -std=c++1y -D__STDC_FORMAT_MACROS -fPIC
LDFLAGS=-Llib
else

CXX=$(shell sst-config --CXX)
CXXFLAGS=$(shell sst-config --ELEMENT_CXXFLAGS) -fsanitize=address
LDFLAGS=$(shell sst-config --ELEMENT_LDFLAGS)
#Please insert the path to your SST elements directory here
SST_ELEMENTS_PATH=/mnt/workspace/TOOLS/SST/sst-elements
CXXFLAGS+=-I$(SST_ELEMENTS_PATH)/include 

endif

SRC_FILES := $(wildcard src/*/*.cc)
OBJ_FILES := $(patsubst src/*/%.cpp, src/*/%.o, $(SRC_FILES))

all: factory_cs factory_fm factory_fmap libpressim.so presSimulator install

factory_cs: src/correction_schemes/cs_generator.py
	cd src/correction_schemes && python cs_generator.py

factory_fm: src/fault_models/fm_generator.py
	cd src/fault_models && python fm_generator.py

factory_fmap: src/fault_maps/fmap_generator.py
	cd src/fault_maps && python fmap_generator.py

libpressim.so: PresSim.o $(OBJ_FILES) 
	$(CXX) -g -shared $(CXXFLAGS) $(LDFLAGS),-soname,$@ -o $@ $^ 

src/*/%.o: src/*/%.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -o $@ $^

presSimulator: PresSim.o $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o presSimulator PresSimulator.cc $^

install:
	cp libpressim.so lib
clean:
	rm -f *.o *.lo src/*.o src/*.lo include/*.h $(CURDIR)/lib/libpressim.so $(CURDIR)/*.so


print-%  : ; @echo $* = $($*)
