.PHONY: all
all: build flash

BINARY = th8a

.PHONY: build
build:
	mkdir -p build \
	&& cd ./build \
	&& cmake ..; \
	make $(BINARY)

.PHONY: reset
reset:
	openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "init; reset; exit"

.PHONY: flash
flash:
	cd ./build; \
	openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program $(BINARY).elf verify reset exit"

.PHONY: apply
apply: build reset flash

.PHONY: clean
clean:
	rm -rf build