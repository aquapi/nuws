LIBRARY_NAME := libuwebsockets

default:
	$(MAKE) clean

	cd ../uWebSockets/uSockets && $(CC) -pthread -DLIBUS_NO_SSL -std=c11 -Isrc -flto -fPIC -O3 -c src/*.c src/eventing/*.c src/crypto/*.c
	cd ../uWebSockets/uSockets && $(CXX) -std=c++17 -flto -fPIC -O3 -c src/crypto/*.cpp

	$(CXX) -DUWS_WITH_PROXY -c -O3 -std=c++17 -flto -I ../uWebSockets/src -I ../uWebSockets/uSockets/src $(LIBRARY_NAME).cpp
	$(AR) rvs $(LIBRARY_NAME).a $(LIBRARY_NAME).o ../uWebSockets/uSockets/*.o

clean:
	rm -f *.o $(LIBRARY_NAME).a $(LIBRARY_NAME).so
