#ifndef ELANAVIGATIONNODE_H
#define ELANAVIGATIONNODE_H

#include <QModelIndex>
#include <QObject>

#include "Def.h"

// 定义ElaNavigationNode类，继承自QObject
class ElaNavigationNode : public QObject
{
    Q_OBJECT
    // 创建Q_PROPERTY属性，用于存储子节点列表
    Q_PROPERTY_CREATE(QList<ElaNavigationNode*>, ChildrenNodes)
    // 创建Q_PRIVATE_PROPERTY属性，用于存储父节点
    Q_PRIVATE_CREATE(ElaNavigationNode*, ParentNode)
    // 创建Q_PROPERTY属性，用于存储图标名称
    Q_PROPERTY_CREATE(ElaIconType::IconName, Awesome)
    // 创建Q_PROPERTY属性，用于存储模型索引
    Q_PROPERTY_CREATE(QModelIndex, ModelIndex)
    // 创建Q_PROPERTY属性，用于存储关键点数量
    Q_PROPERTY_CREATE(int, KeyPoints)
    // 创建Q_PROPERTY属性，用于存储节点深度
    Q_PROPERTY_CREATE(int, Depth)
    // 创建Q_PROPERTY属性，用于标识是否为根节点
    Q_PROPERTY_CREATE(bool, IsRootNode)
    // 创建Q_PROPERTY属性，用于标识是否为页脚节点
    Q_PROPERTY_CREATE(bool, IsFooterNode)
    // 创建Q_PROPERTY属性，用于标识是否有页脚页面
    Q_PROPERTY_CREATE(bool, IsHasFooterPage)
    // 创建Q_PROPERTY属性，用于标识是否为扩展节点
    Q_PROPERTY_CREATE(bool, IsExpanderNode)
    // 创建Q_PROPERTY属性，用于标识是否可见
    Q_PROPERTY_CREATE(bool, IsVisible)
public:
    // 构造函数，初始化节点标题和父节点
    explicit ElaNavigationNode(QString nodeTitle, ElaNavigationNode* parent = nullptr);
    // 析构函数
    ~ElaNavigationNode();

    // 获取节点键
    QString getNodeKey() const;
    // 获取节点标题
    QString getNodeTitle() const;

    // 设置节点是否展开
    void setIsExpanded(bool isExpanded);
    // 获取节点是否展开
    bool getIsExpanded() const;

    // 设置子节点是否可见
    void setChildVisible(bool isVisible);
    // 获取节点是否有子节点
    bool getIsHasChild() const;
    // 获取节点是否有页面子节点
    bool getIsHasPageChild() const;

    // 添加子节点
    void appendChildNode(ElaNavigationNode* childNode);

    // 获取子节点是否有关键点
    bool getIsChildHasKeyPoints() const;

    // 获取原始节点
    ElaNavigationNode* getOriginalNode();
    // 获取节点是否为子节点
    bool getIsChildNode(ElaNavigationNode* node);

    // 获取节点行号
    int getRow() const;

private:
    // 节点键
    QString _nodeKey = "";
    // 节点标题
    QString _nodeTitle = "";
    // 节点是否展开
    bool _isExpanded{false};
};

#endif // ELANAVIGATIONNODE_H
