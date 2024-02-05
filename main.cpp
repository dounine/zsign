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

int main() {
    std::cout << "Hello, World!" << std::endl;

    string keyPath = "/Users/lake/dounine/github/ipadump/zsign/ipa/key.pem";
    string mpPath = "/Users/lake/dounine/github/ipadump/zsign/ipa/lake_13_pm.mobileprovision";
    string ipaPath = "/Users/lake/dounine/github/ipadump/zsign/ipa/video.ipa";

    string tmpFolderPath;


    ZSignAsset zSignAsset;
    if (!zSignAsset.Init("", keyPath, mpPath, "", "1")) {
        cerr << "init sign asset failed" << endl;
        return -1;
    }

    cerr << "hello" << endl;

    ZTimer timer;
    if (tmpFolderPath.empty()) {
        StringFormat(tmpFolderPath, "/tmp/zsign_folder_%llu_%s", timer.Reset(), GenerateUUID().c_str());
    }

    CreateFolder(tmpFolderPath);

    ZLog::PrintV("signing ipa: %s \n", tmpFolderPath.c_str());


    if (IsZipFile(ipaPath)) {
        unzip(ipaPath, tmpFolderPath);
    }


    return 0;
}