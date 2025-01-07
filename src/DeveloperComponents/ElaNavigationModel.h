#ifndef ELANAVIGATIONMODEL_H
#define ELANAVIGATIONMODEL_H

#include <QAbstractItemModel>
#include <QObject>

#include "Def.h"
#include "stdafx.h"
// 声明ElaNavigationNode类，用于表示导航节点
class ElaNavigationNode;
// ElaNavigationModel类继承自QAbstractItemModel，用于管理导航模型
class ElaNavigationModel : public QAbstractItemModel
{
    Q_OBJECT
    // 使用Q_PRIVATE_CREATE宏声明私有创建函数，用于创建SelectedNode和SelectedExpandedNode对象
    Q_PRIVATE_CREATE(ElaNavigationNode*, SelectedNode)
    Q_PRIVATE_CREATE(ElaNavigationNode*, SelectedExpandedNode)

public:
    // 构造函数，初始化ElaNavigationModel对象
    explicit ElaNavigationModel(QObject* parent = nullptr);
    // 析构函数，销毁ElaNavigationModel对象
    ~ElaNavigationModel();
    // 重写QAbstractItemModel的parent函数，返回指定子项的父项索引
    QModelIndex parent(const QModelIndex& child) const override;
    // 重写QAbstractItemModel的index函数，返回指定行、列和父项的索引
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    // 重写QAbstractItemModel的rowCount函数，返回指定父项的行数
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    // 重写QAbstractItemModel的columnCount函数，返回指定父项的列数
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    // 重写QAbstractItemModel的data函数，返回指定索引和角色的数据
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    // 添加扩展节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType::IconName awesome);
    // 添加扩展节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType::IconName awesome);
    // 添加页面节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, ElaIconType::IconName awesome);
    // 添加页面节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, ElaIconType::IconName awesome);
    // 添加页面节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, int keyPoints, ElaIconType::IconName awesome);
    // 添加页面节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, int keyPoints, ElaIconType::IconName awesome);

    // 根据节点键获取导航节点
    ElaNavigationNode* getNavigationNode(QString nodeKey) const;
    // 获取根扩展节点列表
    QList<ElaNavigationNode*> getRootExpanderNodes() const;
    // 获取根展开节点列表
    QList<ElaNavigationNode*> getRootExpandedNodes() const;

private:
    // 用于存储节点键和节点对象的映射
    mutable QMap<QString, ElaNavigationNode*> _nodesMap;
    // 根节点指针
    ElaNavigationNode* _rootNode{nullptr};
};

#endif // ELANAVIGATIONMODEL_H
