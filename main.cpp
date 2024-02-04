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

//uuid生成函数
string GenerateUUID() {
    string uuid;
    uuid.resize(36);
    for (int i = 0; i < 36; i++) {
        if (i == 14) {
            uuid[i] = '4';
        } else if (i == 19) {
            uuid[i] = '8';
        } else {
            uuid[i] = "0123456789abcdef"[rand() % 16];
        }
    }
    return uuid;
}

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

    ZTimer timer;
    if (tmpFolderPath.empty()) {
        StringFormat(tmpFolderPath, "/tmp/zsign_folder_%llu_%s", timer.Reset(), GenerateUUID().c_str());
    }

    CreateFolder(tmpFolderPath);

    ZLog::PrintV("signing ipa: %s \n", tmpFolderPath.c_str());

    unzip(ipaPath, tmpFolderPath);

    return 0;
}