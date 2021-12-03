FLAGS = -std=c++14 -I`root-config --incdir` -I$(HOME)/.local/include/ -O3
LDFLAGS = -L$(HOME)/.local/lib/ -lpixie
#CXX = g++
CXX = clang++

all : libClarionTrinity

libClarionTrinity : Clarion.o ClarionHit.o Clover.o Event.o GAGG.o Gamma.o Trinity.o
	$(CXX) $(FLAGS) -shared -o ../lib/libClarionTrinity.so ../obj/Clarion.o ../obj/ClarionHit.o ../obj/Clover.o ../obj/Event.o ../obj/GAGG.o ../obj/Gamma.o ../obj/Trinity.o $(LDFLAGS)

Clarion.o : Clarion.cc Clarion.hh
	$(CXX) $(FLAGS) -fPIC -c -o ../obj/Clarion.o Clarion.cc

ClarionHit.o : ClarionHit.cc ClarionHit.hh
	$(CXX) $(FLAGS) -fPIC -c -o ../obj/ClarionHit.o ClarionHit.cc

Clover.o : Clover.cc Clover.hh
	$(CXX) $(FLAGS) -fPIC -c -o ../obj/Clover.o Clover.cc

Event.o : Event.cc Event.hh
	$(CXX) $(FLAGS) -fPIC -c -o ../obj/Event.o Event.cc

GAGG.o : GAGG.cc GAGG.hh
	$(CXX) $(FLAGS) -fPIC -c -o ../obj/GAGG.o GAGG.cc

Gamma.o : Gamma.cc Gamma.hh
	$(CXX) $(FLAGS) -fPIC -c -o ../obj/Gamma.o Gamma.cc

Trinity.o : Trinity.cc Trinity.hh
	$(CXX) $(FLAGS) -fPIC -c -o ../obj/Trinity.o Trinity.cc

clean :
	rm ../obj/*.o
	rm ../lib/libClarionTrinity.o

install :
	cp *.hh ~/.local/include/libClarionTrinity
	cp ../lib/libClarionTrinity.so ~/.local/lib
