#include<windows.h>
#include<string>
#include<vector>
using namespace std;
bool deleteOne(const string&path){
    if(remove(path.c_str())){
        return false;
    }
    return true;
}
vector<string> getFiles(const string&path){
    vector<string> files;
    WIN32_FIND_DATA findData;
    HANDLE hFind=FindFirstFile((path+"\\*").c_str(),&findData);
    if(hFind==INVALID_HANDLE_VALUE){
        return files;
    }
    do{
        if(!(findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
            files.emplace_back(path+"\\"+findData.cFileName);
        }
    }while(FindNextFile(hFind,&findData));
    FindClose(hFind);
    return files;
}
void deleteAll(const string&path){
    vector<string> files=getFiles(path);
    for(const auto&file:files){
        deleteOne(file);
    }
}
int main(){
    deleteAll("mcpack\\hsyy_hopes_and_dreams\\b\\functions");
    deleteAll("mcpack\\hsyy_hopes_and_dreams\\r\\particles");
    deleteOne("mcpack\\hsyy_hopes_and_dreams.mcaddon");
    return 0;
}
