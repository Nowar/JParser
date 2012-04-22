CXXFLAGS := -Werror -g

src_files := $(wildcard src/*.cc)
header_files := $(wildcard src/*.h)

all: create_dir jp

create_dir: FORCE
	mkdir -p out/

jp: $(patsubst src/%.cc, out/%.o, $(src_files))
	$(CXX) $(LDFLAGS) -o out/$@ $^

out/%.o: src/%.cc $(header_files)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean: FORCE
	rm -rf out/

.PHONY: FORCE
