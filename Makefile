without_threads:
	@g++ matrix_with_no_threads.cpp -o main.out
	@./main.out
	@rm *.out

with_threads:
	@g++ matrix_with_threads.cpp -o main.out
	@./main.out
	@rm *.out
