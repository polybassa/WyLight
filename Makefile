include Makefile.android
include Makefile.docu

all_nils: test pic x86_client

all_pat: test pic simu x86_client android_client

release: test pic_release x86_client documentation

pic:
	cd firmware; make pic; cd ..

pic_release:
	cd firmware; make pic_release; cd ..

simu:
	cd firmware; make simu; cd ..
	
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

