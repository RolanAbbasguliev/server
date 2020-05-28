run: build
	@echo BUILD: okay
	@echo Starting......
	@./a.out
run_tests: build_tests
	@./a.out
build:
	@clang++ -std=c++17 main.cpp multiplexing.cpp main.h logs.cpp http_request.cpp send_error.cpp exec_cfg.cpp http_responce.cpp
build_tests:
	@clang++ -std=c++17 -D_BUILD_TESTS_ unittests_main.cpp unittests_main.h http_request.cpp exec_cfg.cpp
clean: 
	@rm a.out main.h.gch unittests_main.h.gch



