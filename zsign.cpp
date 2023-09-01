#include "common/common.h"
#include "common/json.h"
#include "openssl.h"
#include "macho.h"
#include "bundle.h"
#include <libgen.h>
#include <dirent.h>
#include <getopt.h>

const struct option options[] = {
        {"debug",          no_argument,       NULL, 'd'},
        {"force",          no_argument,       NULL, 'f'},
        {"unzip",          no_argument,       NULL, 'u'},
        {"verbose",        no_argument,       NULL, 'v'},
        {"cert",           required_argument, NULL, 'c'},
        {"pkey",           required_argument, NULL, 'k'},
        {"prov",           required_argument, NULL, 'm'},
        {"password",       required_argument, NULL, 'p'},
        {"bundle_id",      required_argument, NULL, 'b'},
        {"bundle_name",    required_argument, NULL, 'n'},
        {"bundle_version", required_argument, NULL, 'r'},
        {"tmp_fold",       required_argument, NULL, 't'},
        {"entitlements",   required_argument, NULL, 'e'},
        {"output",         required_argument, NULL, 'o'},
        {"zip_level",      required_argument, NULL, 'z'},
        {"dylib",          required_argument, NULL, 'l'},
        {"weak",           no_argument,       NULL, 'w'},
        {"install",        no_argument,       NULL, 'i'},
        {"quiet",          no_argument,       NULL, 'q'},
        {"help",           no_argument,       NULL, 'h'},
        {}};

int usage() {
    ZLog::Print(
            "from sign.ipadump.com Usage: sign [-options] [-k privkey.pem] [-m dev.prov] [-o output.ipa] file|folder\n");
    ZLog::Print("options:\n");
    ZLog::Print("-k, --pkey\t\tPath to private key or p12 file. (PEM or DER format)\n");
    ZLog::Print("-m, --prov\t\tPath to mobile provisioning profile.\n");
    ZLog::Print("-c, --cert\t\tPath to certificate file. (PEM or DER format)\n");
    ZLog::Print("-d, --debug\t\tGenerate debug output files. (.zsign_debug folder)\n");
    ZLog::Print("-f, --force\t\tForce sign without cache when signing folder.\n");
    ZLog::Print("-u, --unzip\t\tSign unzip folder\n");
    ZLog::Print("-o, --output\t\tPath to output ipa file.\n");
    ZLog::Print("-p, --password\t\tPassword for private key or p12 file.\n");
    ZLog::Print("-b, --bundle_id\t\tNew bundle id to change.\n");
    ZLog::Print("-n, --bundle_name\tNew bundle name to change.\n");
    ZLog::Print("-r, --bundle_version\tNew bundle version to change.\n");
    ZLog::Print("-t, --tmp_fold\tunzip ipa tmp file will be to save here.\n");
    ZLog::Print("-e, --entitlements\tNew entitlements to change.\n");
    ZLog::Print("-z, --zip_level\t\tCompressed level when output the ipa file. (0-9)\n");
    ZLog::Print("-l, --dylib\t\tPath to inject dylib file.\n");
    ZLog::Print("-w, --weak\t\tInject dylib as LC_LOAD_WEAK_DYLIB.\n");
    ZLog::Print("-i, --install\t\tInstall ipa file using ideviceinstaller command for test.\n");
    ZLog::Print("-q, --quiet\t\tQuiet operation.\n");
    ZLog::Print("-v, --version\t\tShows version.\n");
    ZLog::Print("-h, --help\t\tShows help (this message).\n");

    return -1;
}

int main(int argc, char *argv[]) {
    ZTimer gtimer;

    bool bForce = false;
    bool bUnzip = false;
    bool bInstall = false;
    bool bWeakInject = false;
    uint32_t uZipLevel = -1;

    string strCertFile;
    string strPKeyFile;
    string strProvFile;
    string strPassword;
    string strBundleId;
    string strBundleVersion;
    string strDyLibFile;
    string strOutputFile;
    string strTmpFolder;
    string strDisplayName;
    string strEntitlementsFile;

    ZAppBundle bundle;

//    string ss = "/Users/lake/dounine/github/ipadump/sign-docker/lib/zsign/files/aa.ipa";
//    DIR *dir = opendir(ss.c_str());
//    if (NULL != dir) {
//        ZLog::PrintV("");
//        dirent *ptr = readdir(dir);
//
//        closedir(dir);
//    } else {
//
//    }
//    if (true) {
//        return 1;
//    }

    //打印文件夹下的所有文件路径
//    string path = "/Users/lake/dounine/github/ipadump/sign-docker/lib/zsign/common";
//    string basePath = "";
//    set<string> files;
//    bundle.GetFolderFiles(path, basePath, files);
//    //获取文件夹下的所有文件路径
//    DIR *dir = opendir(path.c_str());
//    if (NULL != dir) {
//        dirent *ptr = readdir(dir);
//        while (NULL != ptr) {
//            if (0 != strcmp(ptr->d_name, ".") && 0 != strcmp(ptr->d_name, "..")) {
//                string strNode = path;
//                strNode += "/";
//                strNode += ptr->d_name;
////                if (DT_DIR == ptr->d_type) {
////                    GetFolderFiles(strNode, strBaseFolder, setFiles);
//                if (DT_REG == ptr->d_type) {
//                    files.insert(strNode);
//                }
//            }
//            ptr = readdir(dir);
//        }
//        closedir(dir);
//    }
    //for files;
//    for (auto &file: files) {
//        ZLog::Print(file.c_str());
//        ZLog::Print("\n");
//    }


//    string hi = "";
//    string ss = StringFormat(hi,"%s/tmp/%s/hello","/files/uuid","hi");
//
//    //打印ss
//    ZLog::Print(ss.c_str());
//    if (true) {
//        return 1;
//    }

    int opt = 0;
    int argslot = -1;
    while (-1 != (opt = getopt_long(argc, argv, "dfvhc:k:t:r:m:o:ip:e:b:n:z:ql:w", options, &argslot))) {
        switch (opt) {
            case 'd':
                ZLog::SetLogLever(ZLog::E_DEBUG);
                break;
            case 'f':
                bForce = true;
                break;
            case 'u':
                bUnzip = true;
                break;
            case 'c':
                strCertFile = optarg;
                break;
            case 'k':
                strPKeyFile = optarg;
                break;
            case 'm':
                strProvFile = optarg;
                break;
            case 'p':
                strPassword = optarg;
                break;
            case 'b':
                strBundleId = optarg;
                break;
            case 'r':
                strBundleVersion = optarg;
                break;
            case 'n':
                strDisplayName = optarg;
                break;
            case 'e':
                strEntitlementsFile = optarg;
                break;
            case 'l':
                strDyLibFile = optarg;
                break;
            case 'i':
                bInstall = true;
                break;
            case 'o':
                strOutputFile = GetCanonicalizePath(optarg);
                break;
            case 't':
                strTmpFolder = GetCanonicalizePath(optarg);
                break;
            case 'z':
                uZipLevel = atoi(optarg);
                break;
            case 'w':
                bWeakInject = true;
                break;
            case 'q':
                ZLog::SetLogLever(ZLog::E_NONE);
                break;
            case 'v': {
                printf("version: 0.5\n");
                return 0;
            }
                break;
            case 'h':
            case '?':
                return usage();
                break;
        }

        ZLog::DebugV("from sign.ipadump.com>>> Option:\t-%c, %s\n", opt, optarg);
    }

    if (optind >= argc) {
        return usage();
    }

    if (ZLog::IsDebug()) {
        CreateFolder("./.zsign_debug");
        for (int i = optind; i < argc; i++) {
            ZLog::DebugV("from sign.ipadump.com>>> Argument:\t%s\n", argv[i]);
        }
    }

    string strPath = GetCanonicalizePath(argv[optind]);
    if (!IsFileExists(strPath.c_str())) {
        ZLog::ErrorV("from sign.ipadump.com>>> Invalid Path! %s\n", strPath.c_str());
        return -1;
    }

    bool bZipFile = false;
    if (!IsFolder(strPath.c_str())) {
        bZipFile = IsZipFile(strPath.c_str());
        if (!bZipFile) { //macho file
            ZMachO macho;
            if (macho.Init(strPath.c_str())) {
                if (!strDyLibFile.empty()) { //inject dylib
                    bool bCreate = false;
                    macho.InjectDyLib(bWeakInject, strDyLibFile.c_str(), bCreate);
                } else {
                    macho.PrintInfo();
                }
                macho.Free();
            }
            return 0;
        }
    }

    ZTimer timer;
    ZSignAsset zSignAsset;
    if (!zSignAsset.Init(strCertFile, strPKeyFile, strProvFile, strEntitlementsFile, strPassword)) {
        return -1;
    }


    if (strTmpFolder.empty()) {
        StringFormat(strTmpFolder, "");
    }

    bool bEnableCache = true;
    string strFolder = strPath;
    if (bZipFile) { //ipa file
        bForce = true;
        bEnableCache = false;
        if (!strTmpFolder.empty()) {
            strFolder = strTmpFolder;
        } else {
            StringFormat(strFolder, "/tmp/zsign_folder_%llu", timer.Reset());
        }

        //strFolder目录不存在创建多级目录
        SystemExec("mkdir -p '%s'", strFolder.c_str());

        if (bUnzip) {
            timer.PrintResult(true, "from sign.ipadump.com>>> sing unzip folder，not unzip");
        } else {
            ZLog::PrintV("from sign.ipadump.com>>> Unzip:\t%s (%s) -> %s ... \n", strPath.c_str(),
                         GetFileSizeString(strPath.c_str()).c_str(),
                         strFolder.c_str());
            RemoveFolder(strFolder.c_str());
            if (!SystemExec("unzip -qq -n -d '%s' '%s'", strFolder.c_str(), strPath.c_str())) {
                RemoveFolder(strFolder.c_str());
                ZLog::ErrorV("from sign.ipadump.com>>> Unzip Failed!\n");
                return -1;
            }
            timer.PrintResult(true, "from sign.ipadump.com>>> Unzip OK!");
        }
    }

    timer.Reset();
    bool bRet = bundle.SignFolder(&zSignAsset, strFolder, strBundleId, strBundleVersion, strDisplayName, strDyLibFile,
                                  bForce, bWeakInject, bEnableCache);
    timer.PrintResult(bRet, "from sign.ipadump.com>>> Signed %s!", bRet ? "OK" : "Failed");


    //如果不指定保存路径，保存到临时文件中
    if (strOutputFile.empty()) {
        StringFormat(strOutputFile, "/tmp/zsign_temp_%llu.ipa", GetMicroSecond());
    }

    if (!strOutputFile.empty() && uZipLevel != -1) {
        timer.Reset();
        size_t pos = bundle.m_strAppFolder.rfind("/Payload");
        if (string::npos == pos) {
            ZLog::Error("from sign.ipadump.com>>> Can't Find Payload Directory!\n");
            return -1;
        }

        ZLog::PrintV("from sign.ipadump.com>>> Archiving: \t%s ... \n", strOutputFile.c_str());
        string strBaseFolder = bundle.m_strAppFolder.substr(0, pos);
        char szOldFolder[PATH_MAX] = {0};
        if (NULL != getcwd(szOldFolder, PATH_MAX)) {
            if (0 == chdir(strBaseFolder.c_str())) {
                uZipLevel = uZipLevel > 9 ? 9 : uZipLevel;
                RemoveFile(strOutputFile.c_str());
                SystemExec("zip -q -%u -r '%s' Payload", uZipLevel, strOutputFile.c_str());
                chdir(szOldFolder);
                if (!IsFileExists(strOutputFile.c_str())) {
                    ZLog::Error("from sign.ipadump.com>>> Archive Failed!\n");
                    return -1;
                }
            }
        }
        timer.PrintResult(true, "from sign.ipadump.com>>> Archive OK! (%s)",
                          GetFileSizeString(strOutputFile.c_str()).c_str());
    } else {
        timer.PrintResult(true, "from sign.ipadump.com>>> Not Archive!");
    }

//    if (bRet && bInstall) {
//        SystemExec("ideviceinstaller -i '%s'", strOutputFile.c_str());
//    }

//    if (0 == strOutputFile.find("/tmp/zsign_tmp_")) {
//        RemoveFile(strOutputFile.c_str());
//    }

    //if (0 == strFolder.find( "/tmp/zsign_folder_")) {
//    RemoveFolder(strFolder.c_str());
    //}

    gtimer.Print("from sign.ipadump.com>>> Done.");
    return bRet ? 0 : -1;
}
