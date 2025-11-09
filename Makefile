
all:
	cmake -S . -B build && cmake --build ./build/

install_submodules:
	git submodule update --init --recursive

update_submodules:
	git submodule update --remote --recursive
