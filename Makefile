ANDROID_DIR=./android/WiflyLight
ANDROID_BIN=android/.metadata ${ANDROID_DIR}/bin/ ${ANDROID_DIR}/gen/ ${ANDROID_DIR}/libs/armeabi/ ${ANDROID_DIR}/obj/ ${ANDROID_DIR}Test/bin/ ${ANDROID_DIR}Test/gen/ ${ANDROID_DIR}Test/libs/ ${ANDROID_DIR}Test/obj/

all_nils: test pic x86_client documentation

all_pat: test pic simu x86_client android_client

pic:
	cd firmware; make pic; cd ..

simu:
	cd firmware; make simu; cd ..

android_test:
	ndk-build -C $(ANDROID_DIR)Test
	cp -r $(ANDROID_DIR)Test/libs/ $(ANDROID_DIR)/

android_client:
	ndk-build -C $(ANDROID_DIR)
	
x86_client_debug:
	cd cli; make cli_debug; cd ..

x86_client:
	cd cli; make cli; cd ..
%_test:
	@cd $(subst _test,,$@); make test; cd ..

test: clean firmware_test library_test

%_clean:
	cd $(subst _clean,,$@); make clean; cd ..

clean: cli_clean firmware_clean library_clean
	rm -rf $(ANDROID_BIN)

documentation:
	doxygen docs/Doxyfile; cd html; git add .; git commit -m "documentation"; git push origin gh-pages; cd ..