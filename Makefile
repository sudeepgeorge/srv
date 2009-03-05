# Makefile for the SRV-1

srv1:SRV.o srv_test.o
	g++ -Wall -o srv1 SRV.o srv_test.o

SRV.o: SRV.cpp SRV.h
	g++ -Wall -c SRV.cpp

srv_test.o: srv_test.cpp
	g++ -Wall -c srv_test.cpp

clean:
	- rm -f *.o srv1

#End of Makefile
