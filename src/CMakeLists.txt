cmake_minimum_required(VERSION 3.0)

# additional level of optimisation for the engine
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
			-O3 \
			-flto \
			-std=c11 \
			-march=native \
			-mtune=native \
			-Wpedantic \
			-Wformat=2 \
			-Wall \
			-Wextra \ 
			-Wstrict-prototypes \ 
			-Wmissing-prototypes \ 
			-Wswitch-default \
			-Wunused-macros \
			-Wextra \
			-Wconversion \
			-Wsign-conversion \
			-Wshadow \
			-Wcast-qual \
			-Wcast-align \
			-Wmissing-include-dirs \ 
			-Wpointer-arith \
			-fstack-protector \ 
			-fsanitize=address \ 
			-fsanitize=undefined \ 
			-fomit-frame-pointer \
			-DENABLE_ASSERTS=true")
