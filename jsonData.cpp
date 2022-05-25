#include <unordered_map>
#include <iostream> //��дio c++��׼��
#include <fstream> //��д�ļ� c++��׼��
#include <sstream> //�ַ����� c++��׼��
#include "json.h"
using namespace std;

//�����ж��Ƿ���ڸ�·��,·�����ļ����Լ�ֵ����ʽ�洢��json�У�keyΪ·����path����valueΪ�ļ�����name��
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
//�ж��Ƿ��и��ļ���
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
//���ļ���Ϣд��json����������ͬ·�����ļ���¼�򷵻�false
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
    //���ظ�����false
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
    //���Ҳ�����·���򷵻�false��
    if (repeat == false)return false;
    Json::FastWriter writer;
    //ɾ��path��Ӧ�ļ�ֵ��
    root.removeMember(path);
    ofstream os;
    os.open("./checkjson.json");
    Json::StyledWriter sw;
    sw.write(root);
    //�����м�ֵ��д��json
    os << sw.write(root);
    os.close();
    return true;
}
//�����ļ���ɾ��json�ڶ�Ӧ��ֵ��
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
    //���Ҳ������ļ����򷵻�false��
    if (exist == false)return false;
    Json::FastWriter writer;
    //ɾ��path��Ӧ�ļ�ֵ��
    root.removeMember(key);
    ofstream os;
    os.open("./checkjson.json");
    Json::StyledWriter sw;
    sw.write(root);
    //�����м�ֵ��д��json
    os << sw.write(root);
    os.close();
    return true;
}

//��������Path����vector<string>��ʽ����
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
//��������Name����vector<string>��ʽ����
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