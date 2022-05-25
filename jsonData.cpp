#include <unordered_map>
#include <iostream> //读写io c++标准库
#include <fstream> //读写文件 c++标准库
#include <sstream> //字符串流 c++标准库
#include "json.h"
using namespace std;

//用于判断是否存在该路径,路径和文件名以键值对形式存储在json中，key为路径（path），value为文件名（name）
bool hasPath(string path) {
    ifstream ifs;
    ifs.open("./checkjson.json");
    //assert(ifs.is_open());

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        cerr << "parse failed \n";
        return false;
    }
    int n = root.size();
    bool ans = false;
    for (int i = 0; i < n; i++) {
        if (root.getMemberNames()[i] == path) {
            ans = true;
            break;
        }
    }
    return ans;
}
//判断是否含有该文件名
bool hasName(string name) {
    ifstream ifs;
    ifs.open("./checkjson.json");
    //assert(ifs.is_open());

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        cerr << "parse failed \n";
        return false;
    }
    int n = root.size();
    bool ans = false;
    for (int i = 0; i < n; i++) {
        if (root[root.getMemberNames()[i]] == name) {
            ans = true;
            break;
        }
    }
    return ans;
}
//将文件信息写入json，若存在相同路径的文件记录则返回false
bool addFile(string name, string path) {
    ifstream ifs;
    ifs.open("./checkjson.json");
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        cerr << "parse failed \n";
        return false;
    }
    int n = root.size();
    bool repeat = false;
    for (int i = 0; i < n; i++) {
        if (root.getMemberNames()[i] == path) {
            repeat = true;
            break;
        }
    }
    //若重复返回false
    if (repeat == true)return false;
    Json::FastWriter writer;

    root[path] = name;
    ifs.close();
    ofstream os;
    os.open("./checkjson.json");
    Json::StyledWriter sw;
    sw.write(root);
    os << sw.write(root);
    os.close();
    return true;
}

bool deleteFileByPath(string path) {
    ifstream ifs;
    ifs.open("./checkjson.json");

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        cerr << "parse failed \n";
        return false;
    }
    int n = root.size();
    bool repeat = false;
    for (int i = 0; i < n; i++) {
        if (root.getMemberNames()[i] == path) {
            repeat = true;
            break;
        }
    }
    ifs.close();
    //若找不到此路径则返回false；
    if (repeat == false)return false;
    Json::FastWriter writer;
    //删除path对应的键值对
    root.removeMember(path);
    ofstream os;
    os.open("./checkjson.json");
    Json::StyledWriter sw;
    sw.write(root);
    //将所有键值对写回json
    os << sw.write(root);
    os.close();
    return true;
}
//按照文件名删除json内对应键值对
bool deleteFileByName(string name) {
    ifstream ifs;
    ifs.open("./checkjson.json");

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        cerr << "parse failed \n";
        return false;
    }
    string key;
    int n = root.size();
    bool exist = false;
    for (int i = 0; i < n; i++) {
        key = root.getMemberNames()[i];
        if (root[key] == name) {
            exist = true;
            break;
        }
    }
    ifs.close();
    //若找不到此文件名则返回false；
    if (exist == false)return false;
    Json::FastWriter writer;
    //删除path对应的键值对
    root.removeMember(key);
    ofstream os;
    os.open("./checkjson.json");
    Json::StyledWriter sw;
    sw.write(root);
    //将所有键值对写回json
    os << sw.write(root);
    os.close();
    return true;
}

//返回所有Path，以vector<string>形式返回
vector<string> getAllPath() {
    vector<string> allpath;
    ifstream ifs;
    ifs.open("./checkjson.json");

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        cerr << "parse failed \n";
    }
    int n = root.size();


    string key;
    for (int i = 0; i < n; i++) {
        key = root.getMemberNames()[i];
        allpath.push_back(key);
    }
    ifs.close();
    return allpath;
}
//返回所有Name，以vector<string>形式返回
vector<string> getAllName() {
    vector<string> allname;
    ifstream ifs;
    ifs.open("./checkjson.json");

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        cerr << "parse failed \n";
    }
    int n = root.size();


    string key;
    for (int i = 0; i < n; i++) {
        key = root.getMemberNames()[i];
        allname.push_back(root[key].asString());
    }
    ifs.close();
    return allname;
}
string getNameByPath(string path) {
    string ans;
    ifstream ifs;
    ifs.open("./checkjson.json");

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        cerr << "parse failed \n";
    }
    int n = root.size();


    string key;
    for (int i = 0; i < n; i++) {
        key = root.getMemberNames()[i];
        if (path == key)ans = root[key].asString();
    }
    ifs.close();
    return ans;
}

string getPathByName(string name) {
    string ans;
    ifstream ifs;
    ifs.open("./checkjson.json");

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        cerr << "parse failed \n";
    }
    int n = root.size();


    string key;
    for (int i = 0; i < n; i++) {
        key = root.getMemberNames()[i];
        if (name == root[key].asString())ans = key;
    }
    ifs.close();
    return ans;
}