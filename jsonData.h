#pragma once
#pragma once
#ifndef JSONDATA_H
#define JSONDATA_H
#include <string> //�ַ����� c++��׼��
#include <iostream>
#include <vector>
using namespace std;
//�����ж��Ƿ���ڸ�·��,·�����ļ����Լ�ֵ����ʽ�洢��json�У�keyΪ·����path����valueΪ�ļ�����name��
bool hasPath(string path);
//���ļ���Ϣд��json����������ͬ·�����ļ���¼�򷵻�false
//�ж��Ƿ��и��ļ���
bool hasName(string name);
//���ļ���Ϣд��json����������ͬ·�����ļ���¼�򷵻�false
bool addFile(string name, string path);
//����·��ɾ���ļ�
bool deleteFileByPath(string path);
//�����ļ���ɾ��json�ڶ�Ӧ��ֵ��
bool deleteFileByName(string name);
//ɾ�������ļ�
bool jsonFileClear();
//��������Path����vector<string>��ʽ����
vector<string> getAllPath();
//��������Name����vector<string>��ʽ����
vector<string> getAllName();
string getNameByPath(string path);
string getPathByName(string name);
#endif // !JSONDATA_H


