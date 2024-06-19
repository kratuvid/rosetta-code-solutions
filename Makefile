ifndef what
$(error "Specify source file in the what env variable")
endif

base_flags = -std=c++23 -fmodules-ts
type_flags = -g

target = build/$(what:%.cpp=%)

$(target): $(what)
	g++ $(base_flags) $(type_flags) $(what) -o $(target)

clean:
	rm -f $(target)

run: $(target)
	./$(target)

precompile:
	g++ $(base_flags) -x c++-system-header $(what)

.PHONY: clean run precompile
