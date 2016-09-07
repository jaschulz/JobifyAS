#Jobify

# **AppServer** #


**Building JSONcpp**
1. Clone JSONCpp from https://github.com/open-source-parsers/jsoncpp.git

2. cd jsoncpp

3. python amalgamate.py

4. mkdir build && cd build

5. cmake ..

6. sudo make install

**Building AppServer**

1. cd AppServer/

2. mkdir build && cd build

3. cmake ..

4. make

5. ./main

**Simple Test**

1. cd AppServer/

2. sh prueba.sh
