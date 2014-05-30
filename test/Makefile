OS = $(shell uname -s)
ifeq ($(OS),Darwin)
osx:
	#mocha test_osx.js -R spec
	mocha test.js -R spec
endif
ifeq ($(OS),Linux)
linux:
	sudo mocha test.js -R spec
	#sudo mocha test_osx.js -R spec
	#sudo mocha test_linux.js -R spec
endif

