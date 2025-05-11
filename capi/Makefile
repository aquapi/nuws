LIBRARY_NAME := libuwebsockets

uws:
	cd ../uWebSockets/uSockets && $(CC) -pthread -DLIBUS_NO_SSL -std=c11 -Isrc -flto -fPIC -O3 -c src/*.c src/eventing/*.c src/crypto/*.c
	cd ../uWebSockets/uSockets && $(CXX) -std=c++17 -flto -fPIC -O3 -c src/crypto/*.cpp

	$(CXX) -DUWS_WITH_PROXY -c -O3 -std=c++17 -flto -I ../uWebSockets/src -I ../uWebSockets/uSockets/src $(LIBRARY_NAME).cpp

static: clean uws
	$(AR) rvs $(LIBRARY_NAME).a $(LIBRARY_NAME).o ../uWebSockets/uSockets/*.o

shared: clean uws
	$(CXX) -fPIC -lz -luv -lssl -lcrypto -shared -o $(LIBRARY_NAME).so  $(LIBRARY_NAME).o ../uWebSockets/uSockets/uSockets.a

clean:
	rm -f *.o $(LIBRARY_NAME).a $(LIBRARY_NAME).so
