#ifndef STDAFX_H
#define STDAFX_H

#include <QtCore/qglobal.h>

/// @brief 如果定义了ELAWIDGETTOOLS_LIBRARY，则使用Q_DECL_EXPORT，否则使用Q_DECL_IMPORT
///这是在创建共享库时常用的技巧。
#ifdef ELAWIDGETTOOLS_LIBRARY
#define ELA_EXPORT Q_DECL_EXPORT
#else
#define ELA_EXPORT Q_DECL_IMPORT
#endif

/// @brief  属性创建宏: 创建一个属性，包括Q_PROPERTY声明、信号、setter和getter方法
/// p##M是属性名，_p##M是属性对应的私有成员变量。
#define Q_PROPERTY_CREATE(TYPE, M)                          \
    Q_PROPERTY(TYPE p##M MEMBER _p##M NOTIFY p##M##Changed) \
public:                                                     \
    Q_SIGNAL void p##M##Changed();                          \
    void set##M(TYPE M)                                     \
    {                                                       \
        _p##M = M;                                          \
        Q_EMIT p##M##Changed();                             \
    }                                                       \
    TYPE get##M() const                                     \
    {                                                       \
        return _p##M;                                       \
    }                                                       \
                                                            \
private:                                                    \
    TYPE _p##M;

/// @brief  简化属性创建宏
/// 简化了属性创建，只包含Q_PROPERTY声明、信号、setter和getter方法，不包含私有成员变量的定义。
#define Q_PROPERTY_CREATE_Q_H(TYPE, M)                                  \
    Q_PROPERTY(TYPE p##M READ get##M WRITE set##M NOTIFY p##M##Changed) \
public:                                                                 \
    Q_SIGNAL void p##M##Changed();                                      \
    void set##M(TYPE M);                                                \
    TYPE get##M() const;

/// @brief 私有属性创建宏, 只包含setter和getter方法的声明
#define Q_PRIVATE_CREATE_Q_H(TYPE, M) \
public:                               \
    void set##M(TYPE M);              \
    TYPE get##M() const;

/// @brief  属性实现宏：用于在类的实现文件中定义setter和getter方法，使用Q_D宏来访问私有数据。
#define Q_PROPERTY_CREATE_Q_CPP(CLASS, TYPE, M) \
    void CLASS::set##M(TYPE M)                  \
    {                                           \
        Q_D(CLASS);                             \
        d->_p##M = M;                           \
        Q_EMIT p##M##Changed();                 \
    }                                           \
    TYPE CLASS::get##M() const                  \
    {                                           \
        return d_ptr->_p##M;                    \
    }

/// @brief  私有属性实现宏：用于在类的实现文件中定义私有属性的setter和getter方法
#define Q_PRIVATE_CREATE_Q_CPP(CLASS, TYPE, M) \
    void CLASS::set##M(TYPE M)                 \
    {                                          \
        Q_D(CLASS);                            \
        d->_p##M = M;                          \
    }                                          \
    TYPE CLASS::get##M() const                 \
    {                                          \
        return d_ptr->_p##M;                   \
    }
/// @brief 私有数据成员简化定义宏: 定义私有数据成员。
#define Q_PROPERTY_CREATE_D(TYPE, M) \
private:                             \
    TYPE _p##M;

/// @brief 私有数据成员简化定义宏：
#define Q_PRIVATE_CREATE_D(TYPE, M) \
private:                            \
    TYPE _p##M;

/// @brief 用于定义一个简单的私有数据成员及其setter和getter方法。
#define Q_PRIVATE_CREATE(TYPE, M) \
public:                           \
    void set##M(TYPE M)           \
    {                             \
        _p##M = M;                \
    }                             \
    TYPE get##M() const           \
    {                             \
        return _p##M;             \
    }                             \
                                  \
private:                          \
    TYPE _p##M;

/// @brief Q_Q宏,用于在类中使用Qt的私有数据类（Private Class）模式
/// 用于在公共类中定义和使用私有数据类。
#define Q_Q_CREATE(CLASS)                                        \
protected:                                                       \
    explicit CLASS(CLASS##Private& dd, CLASS* parent = nullptr); \
    QScopedPointer<CLASS##Private> d_ptr;                        \
                                                                 \
private:                                                         \
    Q_DISABLE_COPY(CLASS)                                        \
    Q_DECLARE_PRIVATE(CLASS);

/// @brief Q_D宏,用于在类中使用Qt的私有数据类（Private Class）模式
/// 用于在私有数据类中访问公共类
#define Q_D_CREATE(CLASS) \
protected:                \
    CLASS* q_ptr;         \
                          \
private:                  \
    Q_DECLARE_PUBLIC(CLASS);

#endif // STDAFX_H
