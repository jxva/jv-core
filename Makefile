CC=gcc

CFLAGS=-Wall -g -pedantic -pipe -W -Werror=variadic-macros -Wpointer-arith -Wno-unused-parameter -Wunused-function -Wunused-variable -Wunused-value -Wno-long-long -Werror=return-type -Werror

INCLUDES=-I.

LIBS=

LINKS=
	
TARGET=main

TIME_TEST=jv_time_test

LOG_TEST=jv_log_test

POOL_TEST=jv_pool_test

STRING_TEST=jv_string_test

all:	
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_time.c -o jv_time.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_log.c -o jv_log.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_pool.c -o jv_pool.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_string.c -o jv_string.o

	$(CC) $(CFLAGS) $(INCLUDES) -c jv_time_test.c -o jv_time_test.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_log_test.c -o jv_log_test.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_pool_test.c -o jv_pool_test.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_string_test.c -o jv_string_test.o

	$(CC) $(CFLAGS) $(INCLUDES) -c jv_main.c -o jv_main.o
	
	$(CC) -o $(TIME_TEST) jv_time_test.o jv_time.o $(LINKS) $(LIBS)
	$(CC) -o $(LOG_TEST) jv_log_test.o jv_log.o jv_time.o $(LINKS) $(LIBS)
	$(CC) -o $(POOL_TEST) jv_pool_test.o jv_pool.o jv_log.o jv_time.o $(LINKS) $(LIBS)
	$(CC) -o $(STRING_TEST) jv_string_test.o jv_string.o jv_pool.o jv_log.o jv_time.o $(LINKS) $(LIBS)
	$(CC) -o $(TARGET) jv_main.o jv_time.o jv_log.o jv_pool.o jv_string.o $(LINKS) $(LIBS)


	@echo 
	@echo Project has been successfully compiled.
	@echo
	
clean:
	rm -rf $(TARGET) $(TIME_TEST) $(LOG_TEST) $(POOL_TEST) $(STRING_TEST) *.depend *.layout bin obj *.o *.stackdump *.exe *.log *~