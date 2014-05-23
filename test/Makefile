OS = $(shell uname -s)
ifeq ($(OS),Darwin)
osx:
	mocha test_osx.js -R spec
endif
ifeq ($(OS),Linux)
linux:
	sudo mocha test_linux.js -R spec
endif

