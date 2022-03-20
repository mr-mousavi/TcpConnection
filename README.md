#Compile

##Default Parameter
Following parameter set directly in CMakeLists.txt. Therefore, if you want to change them then you should modify CMakeLists.txt

- -std=c++98

##Debug Mode
In order to compile project in *debug* mode (enable debug flag) you should perform following step:

1. Make sure you are in root directory of project
2. mkdir _debug
3. cd _debug
4. cmake -DCMAKE_BUILD_TYPE=Debug ..
5. make

##Release Mode
It is mostly like debug mode, except that in step 4 you should pass another parameter :

1. Make sure you are in root directory of project
2. mkdir _release
3. cd _release
4. cmake -DCMAKE_BUILD_TYPE=Debug ..
5. make

##Example
You can use the following configuration to compile project in *debug mode*:

	
	#> cmake -DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_C_COMPILER=path_to_your_toolchain_bin/gcc \
	-DCMAKE_CXX_COMPILER=path_to_your_toolchain_bin/g++ \
	-DCMAKE_FIND_ROOT_PATH=path_to_your_toolchain_bin/path_to_sysroot \
        -DCMAKE_INSTALL_PREFIX=../_install ..
