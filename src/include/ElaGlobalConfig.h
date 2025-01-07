//
// Created by qomos on 2025/1/7.
//

#ifndef ELAGLOBALCONFIG_H
#define ELAGLOBALCONFIG_H
#include <QObject>
#include "Def.h" // 包含自定义定义的头文件
#include "stdafx.h" // 包含预编译头文件

class ELA_EXPORT ElaGlobalConfig : public QObject
{
    Q_OBJECT


public:
    ElaGlobalConfig(QObject* parent = nullptr);
    ~ElaGlobalConfig();
    // 添加其他成员函数和变量
private:
    bool _isShowSearch{true};
};
#endif //ELAGLOBALCONFIG_H
