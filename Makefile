# ------------------------------------------------------------------
PATH_INCLUDE := include
PATH_SRC     := src
PATH_BIN     := bin
PATH_OBJ     := $(PATH_BIN)/obj
PATH_OUT 	 := $(PATH_BIN)/out
# ------------------------------------------------------------------
RM           = @rm -rf
MKDIR        = @mkdir -p
# ------------------------------------------------------------------
CXX          = g++
CXXFLAGS     = -std=c++11
CC           = $(CXX) $(CXXFLAGS)
# ------------------------------------------------------------------
NAMED_PIPES := building_bills_pipe
OBJS         = $(PATH_OBJ)/main.o $(PATH_OBJ)/util.o
BUILDING     = building.out
BILLS      = bills.out
RESOURCE         = resource.out
OUT_EXE      = a.out
# ------------------------------------------------------------------
all: pre_build build final

pre_build:
	$(MKDIR) $(PATH_OBJ)
	$(MKDIR) $(PATH_OUT)

build: $(PATH_OUT)/$(BUILDING) $(PATH_OUT)/$(BILLS) $(PATH_OUT)/$(RESOURCE)

final: $(OBJS)
	$(CC) $(OBJS) -o $(OUT_EXE)
# ------------------------------------------------------------------
$(PATH_OBJ)/main.o: $(PATH_SRC)/main.cpp $(PATH_INCLUDE)/util.hpp $(PATH_INCLUDE)/constants.hpp
	$(CC) -c $(PATH_SRC)/main.cpp -o $@

$(PATH_OBJ)/util.o: $(PATH_SRC)/util.cpp $(PATH_INCLUDE)/util.hpp
	$(CC) -c $(PATH_SRC)/util.cpp -o $@
# ------------------------------------------------------------------
$(PATH_OUT)/$(BUILDING): $(PATH_SRC)/building.cpp $(PATH_INCLUDE)/util.hpp $(PATH_OBJ)/util.o $(PATH_INCLUDE)/constants.hpp
	$(CC) $(PATH_SRC)/building.cpp $(PATH_OBJ)/util.o -o $@

$(PATH_OUT)/$(BILLS): $(PATH_SRC)/bills.cpp $(PATH_OBJ)/util.o
	$(CC) $(PATH_SRC)/bills.cpp $(PATH_OBJ)/util.o -o $@

$(PATH_OUT)/$(RESOURCE): $(PATH_SRC)/resource.cpp $(PATH_OBJ)/util.o
	$(CC) $(PATH_SRC)/resource.cpp $(PATH_OBJ)/util.o -o $@
# ------------------------------------------------------------------
clean:
	$(RM) $(PATH_BIN) *.out $(NAMED_PIPES) $(OUT_EXE)
# ------------------------------------------------------------------