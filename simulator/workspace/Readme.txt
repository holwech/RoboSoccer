There are two possibilities, building your project:

1) Qt: use the provided project file soccer_sample.pro
	all rtdb dependencies have been added (see setting.pri for details)
	you just need to add your source code, header files or external libraries to it	

2) Cmake: required cmake 2.8 or above
	put your source code in src and your headers in include
	if adding libraries CMakeLists.txt has to be adapted (see cmake resources for details)
	delete all files in bin directory and cd bin/
	then cmake and at the end make
	a compiled executeable file should then be generated in the bin folder
	
	e.g.	rm -r bin/
		cd bin/
		cmake ..
		make

