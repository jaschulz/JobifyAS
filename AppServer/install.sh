#!/bin/bash

root_dir=`pwd`
install_dir="$root_dir/temp_install"

# Instalación de dependencias y herramientas
echo "Installing tools and dependencies"
apt-get update
apt-get install -y --assume-yes --force-yes libsnappy-dev zlib1g-dev libbz2-dev libgflags-dev cmake lcov libboost-all-dev libssl-dev git libcppunit-dev

## Instala las librerías
echo "Installing libraries"
apt-get install -y wget
mkdir -p temp_install && \
cd temp_install && \

cd $install_dir
echo "Installing jsoncpp"
wget https://github.com/open-source-parsers/jsoncpp/archive/0.y.z.zip && \
unzip 0.y.z.zip && \
cd jsoncpp-0.y.z && \
python amalgamate.py && \
#cd dist && \
#gcc -c jsoncpp.cpp && \
#ar rvs libjsoncpp.a jsoncpp.o && \
#sudo cp libjsoncpp.a /usr/lib && \
mkdir build && cd build && \
cmake .. && \
make install && \
ldconfig
echo "Installation complete."


cd $install_dir
# Last release does not have cmake enabled, so we install it from master.
echo "Installing mongose-cpp"
wget https://github.com/Gregwar/mongoose-cpp/archive/master.zip && \
unzip master.zip && \
rm master.zip && \
cd mongoose-cpp-master && \
cmake . && \
make install && \

cd $install_dir
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libssl-dev 
sudo pip install requests

sudo apt-get install libsnappy-dev
wget https://github.com/google/leveldb/archive/master.zip && \
unzip master.zip && \
rm master.zip && \
cd leveldb-master && \
make && \
cp --preserve=links out-shared/libleveldb.* /usr/local/lib && \
cp -r include/leveldb /usr/local/include/ && \
ldconfig

cd $root_dir
rm -rf temp_install

echo "Make and install"
mkdir -p build && cd build && \
cmake .. && make


#corro para ejecutar los tests
#./appServer
