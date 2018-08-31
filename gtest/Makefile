gtest: ./gtest.cc
	g++ $^ -o $@ -I ~/third_part/include -L ~/third_part/lib \
		-lgtest -Igtest_main -lpthread

.PHONY:clean
clean:
	rm gtest
