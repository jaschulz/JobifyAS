#include "utils.h"
#include <openssl/sha.h>
#include <sstream>
#include <iostream>
#include <iomanip>

#define pi 3.14159265358979323846

using namespace std;

std::string utils::sha256(const std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// This function converts decimal degrees to radians
double utils::deg2rad(double deg) {
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
double utils::rad2deg(double rad) {
  return (rad * 180 / pi);
}
