all :
	gcc -std=c99 -pthread -o concurrency1 concurrency1.c

clean:
	rm concurrency1
