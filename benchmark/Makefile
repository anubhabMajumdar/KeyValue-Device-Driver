all: benchmark validate collision

benchmark: benchmark.c
	$(CC) -g -O0 benchmark.c -o benchmark -w -lckv
validate: validate.c
	$(CC) -g -O0 validate.c -o validate -w -lckv
collision: collision.c
	$(CC) -g -O0 collision.c -o collision -w -lckv

clean:
	rm -f benchmark validate collision
