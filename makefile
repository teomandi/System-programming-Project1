all:
	gcc main.c -o werhauz
	./werhauz -h1 2 -h2 4 -o input -b 120 -c config.txt