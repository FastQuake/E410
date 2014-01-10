SRCS = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
CXX = g++
LD_FLAGS = -lsfml-system -lsfml-window -lsfml-graphics -lGLEW -lGL 
CC_FLAGS = -g
TITLE = e410

$(TITLE): $(OBJS)
	$(CXX) -o $@ $^ $(LD_FLAGS)
%.o: %.cpp
	$(CXX) $(CC_FLAGS) -c $< -o $@

clean:
	rm $(OBJS)
	rm $(TITLE)

run:
	./$(TITLE)
