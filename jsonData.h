#pragma once
#pragma once
#ifndef JSONDATA_H
#define JSONDATA_H
#include <string> //字符串类 c++标准库
#include <iostream>
#include <vector>
using namespace std;
//用于判断是否存在该路径,路径和文件名以键值对形式存储在json中，key为路径（path），value为文件名（name）
bool hasPath(string path);
//将文件信息写入json，若存在相同路径的文件记录则返回false
//判断是否含有该文件名
bool hasName(string name);
//将文件信息写入json，若存在相同路径的文件记录则返回false
bool addFile(string name, string path);
//按照路径删除文件
bool deleteFileByPath(string path);
//按照文件名删除json内对应键值对
bool deleteFileByName(string name);
//删除所有文件
bool jsonFileClear();
//返回所有Path，以vector<string>形式返回
vector<string> getAllPath();
//返回所有Name，以vector<string>形式返回
vector<string> getAllName();
string getNameByPath(string path);
string getPathByName(string name);
#endif // !JSONDATA_H


