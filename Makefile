run: build
	@echo BUILD: okay
	@echo Starting......
	@./a.out
build:
	@g++ main.cpp multiplexing.cpp main.h logs.cpp http_request.cpp HTTP.h  HTTP_req.h send_error.cpp
clean: 
	@rm a.out main.h.gch HTTP.h.gch HTTP_req.h.gch

