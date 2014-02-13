NOC_BINARY = ./bin/test/noc_test

all: display_options

display_options:
	@echo "Available options:"
	@echo "  build:    build the binaries."
	@echo "  run:      run the noc test to generate data in the data folder."
	@echo "  clean:    clean the build directory."
	@echo "  mrproper: clean all the source tree from generated files."

build: $(NOC_BINARY)

$(NOC_BINARY):
	@mkdir -p build
	@cd build && cmake .. && make -j8

run: $(NOC_BINARY)
	@./scripts/run_test.sh 100000

clean:
	@cd build && make clean

mrproper:
	@rm -fr build data *.vcd *.data
	@[ -d .git ] && git clean -xdf

.PHONY: all display_options clean mrproper
