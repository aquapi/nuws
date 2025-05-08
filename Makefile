bindings:
	cd ./capi && $(MAKE)
	nelua ./lib/bindgen.nelua
