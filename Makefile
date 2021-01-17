CXX=g++
CXXFLAGS=-Wall -ggdb
RM=rm -f

programme : codage.o decodage.o test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ 

codage.o: codage.cpp codage.h
decodage.o: decodage.cpp decodage.h
test.o: test.cpp codage.h decodage.h

%.o: %.cpp; $(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) test.o codage.o decodage.o programme
