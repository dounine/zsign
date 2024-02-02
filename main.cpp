//
// Created by lake on 2024/2/2.
//
#include "common/common.h"
#include "common/json.h"
#include "openssl.h"
#include "macho.h"
#include "bundle.h"
#include <getopt.h>
#include <iostream>
#include <string>

using namespace std;
//int main() {
//    std::cout << "Hello, World!" << std::endl;
//
//    string p12Path = "/Users/lake/dounine/github/ipadump/zsign/ipa/lake.p12";
//    string mpPath = "/Users/lake/dounine/github/ipadump/zsign/ipa/lake_13_pm.mobileprovision";
//    string ipaPath = "/Users/lake/dounine/github/ipadump/zsign/ipa/video.ipa";
//
//
//    ZSignAsset zSignAsset;
//    if (!zSignAsset.Init("", p12Path, mpPath, "", "1")) {
//        cerr << "init sign asset failed" << endl;
//        return -1;
//    }
//
//
//    return 0;
//}