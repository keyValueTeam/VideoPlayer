#pragma once
#include<QSqlDatabase>
#include<QSqlQuery>
#include <QDebug>
#include <QDir>
class SQLTest
{
public:
	SQLTest() {
        
        QSqlQuery query;
        //query.exec("create table student(id int primary key,name varchar(20))");
        //query.exec("insert into student values(0,'liming')");
        //query.exec("insert into student values(1,'liutao')");
        //query.exec("insert into student values(2,'wanghong')");
        query.exec("select * from student where id = 1");
        while (query.next())
        {
            qDebug() << query.value(0).toInt() << query.value(1).toString();
        }
        return;
        
	}
private:
    
    
};