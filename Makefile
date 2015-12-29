CPP_FILES := src/ipmt.cpp
OBJ_FILES := $(addprefix bin/,$(notdir $(CPP_FILES:.cpp=.o)))
CC_FLAGS += -MMD -w -O3

ipmt: $(OBJ_FILES)
	g++ -o bin/$@ $^
	rm bin/*.o
	rm bin/*.d

bin/%.o: src/%.cpp
	mkdir -p bin
	g++ $(CC_FLAGS) -c -o $@ $<

clean:
	rm -rf bin/*

