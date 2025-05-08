LIBRARY_NAME := libuwebsockets

default:
	$(MAKE) clean

	cd ../uWebSockets/uSockets && $(CC) -pthread -DLIBUS_NO_SSL -std=c11 -Isrc -flto -fPIC -O3 -c src/*.c src/eventing/*.c src/crypto/*.c
	cd ../uWebSockets/uSockets && $(CXX) -std=c++17 -flto -fPIC -O3 -c src/crypto/*.cpp
	cd ../uWebSockets/uSockets && $(AR) rvs uSockets.a *.o

	$(CXX) -DUWS_WITH_PROXY -c -O3 -std=c++17 -lz -luv -flto -fPIC -I ../uWebSockets/src -I ../uWebSockets/uSockets/src $(LIBRARY_NAME).cpp
	$(CXX) -shared -o $(LIBRARY_NAME).so  $(LIBRARY_NAME).o ../uWebSockets/uSockets/uSockets.a -fPIC  -lz -luv -lssl -lcrypto

clean:
	rm -f *.o $(LIBRARY_NAME).a $(LIBRARY_NAME).so
