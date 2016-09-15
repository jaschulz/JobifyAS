#!/bin/bash

root_dir=`pwd`
install_dir="$root_dir/temp_install"

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update -qq
sudo apt-get install -qq g++-4.8
export CXX="g++-4.8"

cd $install_dir
echo "Installing jsoncpp"
wget https://github.com/open-source-parsers/jsoncpp/archive/0.y.z.zip && \
unzip 0.y.z.zip && \
cd jsoncpp-0.y.z && \
python amalgamate.py && \
cd dist && \
gcc -c jsoncpp.cpp && \
ar rvs libjsoncpp.a jsoncpp.o && \
sudo cp libjsoncpp.a /usr/lib && \


cd $install_dir
# Last release does not have cmake enabled, so we install it from master.
echo "Installing mongose-cpp"
wget https://github.com/Gregwar/mongoose-cpp/archive/master.zip && \
unzip master.zip && \
rm master.zip && \
cd mongoose-cpp-master && \
cmake . && \
make install && \

sudo apt-get install libcurl4-openssl-dev

cd $root_dir
cd AppServer
mkdir build && cd build
cmake ..
make