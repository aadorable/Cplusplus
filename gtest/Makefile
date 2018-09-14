map: ./map.cc
	g++ $^ -o $@ -I ~/third_part/include -L ~/third_part/lib \
		-lmap -Imap_main -lpthread -std=c++11

.PHONY:clean
clean:
	rm map
