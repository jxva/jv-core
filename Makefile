CC=gcc

CFLAGS=-Wall -g -DDEBUG -pedantic -pipe -W -Werror=variadic-macros -Wpointer-arith -Wno-unused-parameter -Wunused-function -Wunused-variable -Wunused-value -Wno-long-long -Werror=return-type -Werror

INCLUDES=-I.

LIBS=

LINKS=
	
TARGET=main

TIME_DEMO=jv_time_demo

LOG_DEMO=jv_log_demo

POOL_DEMO=jv_pool_demo

STRING_DEMO=jv_string_demo

all:	
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_time.c -o jv_time.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_log.c -o jv_log.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_pool.c -o jv_pool.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_string.c -o jv_string.o

	$(CC) $(CFLAGS) $(INCLUDES) -c jv_time_demo.c -o jv_time_demo.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_log_demo.c -o jv_log_demo.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_pool_demo.c -o jv_pool_demo.o
	$(CC) $(CFLAGS) $(INCLUDES) -c jv_string_demo.c -o jv_string_demo.o

	$(CC) $(CFLAGS) $(INCLUDES) -c jv_main.c -o jv_main.o
	
	$(CC) -o $(TIME_DEMO) jv_time_demo.o jv_time.o $(LINKS) $(LIBS)
	$(CC) -o $(LOG_DEMO) jv_log_demo.o jv_log.o jv_time.o $(LINKS) $(LIBS)
	$(CC) -o $(POOL_DEMO) jv_pool_demo.o jv_pool.o jv_log.o jv_time.o $(LINKS) $(LIBS)
	$(CC) -o $(STRING_DEMO) jv_string_demo.o jv_string.o jv_pool.o jv_log.o jv_time.o $(LINKS) $(LIBS)
	$(CC) -o $(TARGET) jv_main.o jv_time.o jv_log.o jv_pool.o jv_string.o $(LINKS) $(LIBS)


	@echo 
	@echo Project has been successfully compiled.
	@echo
	
clean:
	rm -rf $(TARGET) $(TIME_DEMO) $(LOG_DEMO) $(POOL_DEMO) $(STRING_DEMO) *.depend *.layout bin obj *.o *.stackdump *.exe *.log *~