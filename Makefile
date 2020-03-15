run: build
	@./a.out
build:
	@g++ main.cpp multiplexing.cpp main.h
clean: 
	@rm a.out main.h.gch