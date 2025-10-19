#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include<tchar.h>
#include<filesystem>
#include<cstdlib>
#include<fstream>

namespace fs = std::filesystem;

int main() {

    //SetConsoleOutputCP(CP_UTF8);

    // 初始化OPENFILENAME结构体
    OPENFILENAME ofn;
    TCHAR szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _T("C++ Files\0 * .cpp\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

  

    if (!GetOpenFileName(&ofn)) {
        return -1;
    }
        
    std::filesystem::path filePath(szFile);
       
    std::cout << filePath.u8string() << std::endl; 

    std::string path = filePath.string();
    std::string filename = path.substr(path.rfind('\\')+1, path.rfind('.') - path.rfind('\\'));

    std::string sourceFile = path;
    std::string destDir = "test";

    // 1. 构造目标文件路径（目录+文件名）
    fs::path targetPath = destDir / fs::path(sourceFile).filename();

    try {
        // 2. 检查目标目录是否存在，不存在则创建
        if (!fs::exists(destDir)) {
            fs::create_directories(destDir);
        }

        // 3. 执行复制，如果目标文件已存在则覆盖
        fs::copy_file(sourceFile, targetPath, fs::copy_options::overwrite_existing);

        std::cout << "文件复制成功！" << std::endl;
        std::cout << "源文件: " << sourceFile << std::endl;
        std::cout << "目标位置: " << targetPath << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }

    
    std::string topath = destDir + "\\" + filename;
    std::cout << path << std::endl;
   

    

    std::string str = "g++ " + topath + "cpp -o " + topath+"exe";

    std::cout << str << std::endl;
    std::string run = topath+"exe";

    int result =system(str.c_str());
    if (result == 0) {
        system(run.c_str());
    }
    

    return 0;
}