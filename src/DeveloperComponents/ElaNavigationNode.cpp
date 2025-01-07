#include "ElaNavigationNode.h" // 包含ElaNavigationNode类的头文件

#include <QUuid> // 包含QUuid库，用于生成唯一的标识符

// 构造函数
ElaNavigationNode::ElaNavigationNode(QString nodeTitle, ElaNavigationNode* parent)
    : QObject(parent) // 调用QObject的构造函数，设置父对象
{
    _pDepth = 0; // 初始化节点深度为0
    _pKeyPoints = 0; // 初始化关键点数量为0
    _nodeKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-"); // 生成唯一的节点键，并移除UUID中的花括号和连字符
    _nodeTitle = nodeTitle; // 设置节点标题
    _pIsRootNode = false; // 初始化是否为根节点为false
    _pIsFooterNode = false; // 初始化是否为页脚节点为false
    _pIsHasFooterPage = false; // 初始化是否有页脚页为false
    _pParentNode = parent; // 设置父节点
    _pIsExpanderNode = false; // 初始化是否为扩展节点为false
    _pIsVisible = false; // 初始化是否可见为false
}

// 析构函数
ElaNavigationNode::~ElaNavigationNode()
{
    qDeleteAll(_pChildrenNodes); // 删除所有子节点
}

// 获取节点键
QString ElaNavigationNode::getNodeKey() const
{
    return _nodeKey;
}

// 获取节点标题
QString ElaNavigationNode::getNodeTitle() const
{
    return _nodeTitle;
}

// 设置节点是否展开
void ElaNavigationNode::setIsExpanded(bool isExpanded)
{
    _isExpanded = isExpanded; // 设置当前节点是否展开
    setChildVisible(isExpanded); // 根据展开状态设置子节点可见性
}

// 获取节点是否展开
bool ElaNavigationNode::getIsExpanded() const
{
    return _isExpanded;
}

// 设置子节点可见性
void ElaNavigationNode::setChildVisible(bool isVisible)
{
    if (isVisible)
    {
        for (auto node : _pChildrenNodes)
        {
            node->setIsVisible(isVisible); // 设置子节点可见性
            if (node->getIsExpanderNode() && !node->getIsExpanded())
            {
                continue; // 如果子节点是扩展节点且未展开，则跳过
            }
            node->setChildVisible(isVisible); // 递归设置子节点的子节点可见性
        }
    }
    else
    {
        for (auto node : _pChildrenNodes)
        {
            node->setChildVisible(isVisible); // 递归设置子节点的子节点不可见
            node->setIsVisible(isVisible); // 设置子节点不可见
        }
    }
}

// 获取是否有子节点
bool ElaNavigationNode::getIsHasChild() const
{
    if (_pChildrenNodes.count() > 0)
    {
        return true; // 如果子节点数量大于0，返回true
    }
    return false; // 否则返回false
}

// 获取是否有页面子节点
bool ElaNavigationNode::getIsHasPageChild() const
{
    if (_pChildrenNodes.count() == 0)
    {
        return false; // 如果子节点数量为0，返回false
    }
    for (auto childNode : _pChildrenNodes)
    {
        if (!childNode->getIsExpanderNode())
        {
            return true; // 如果子节点不是扩展节点，返回true
        }
        if (childNode->getIsHasPageChild())
        {
            return true; // 如果子节点有页面子节点，返回true
        }
    }
    return false; // 否则返回false
}

// 添加子节点
void ElaNavigationNode::appendChildNode(ElaNavigationNode* childNode)
{
    if (_pIsExpanderNode)
    {
        _pChildrenNodes.append(childNode); // 如果当前节点是扩展节点，添加子节点
    }
}

// 获取子节点是否有关键点
bool ElaNavigationNode::getIsChildHasKeyPoints() const
{
    for (auto childNnode : _pChildrenNodes)
    {
        if (childNnode->getKeyPoints())
        {
            return true; // 如果子节点有关键点，返回true
        }
        if (childNnode->getIsChildHasKeyPoints())
        {
            return true; // 如果子节点的子节点有关键点，返回true
        }
    }
    return false; // 否则返回false
}

// 获取原始节点
ElaNavigationNode* ElaNavigationNode::getOriginalNode()
{
    if (this->getParentNode()->getIsRootNode())
    {
        return this; // 如果父节点是根节点，返回当前节点
    }
    else
    {
        ElaNavigationNode* originalNode = this->getParentNode();
        while (!originalNode->getIsRootNode() && !originalNode->getParentNode()->getIsRootNode())
        {
            originalNode = originalNode->getParentNode(); // 向上查找直到找到根节点
        }
        return originalNode; // 返回原始节点
    }
}

// 获取是否为子节点
bool ElaNavigationNode::getIsChildNode(ElaNavigationNode* node)
{
    if (_pChildrenNodes.count() > 0)
    {
        if (_pChildrenNodes.contains(node))
        {
            return true; // 如果当前节点的子节点包含目标节点，返回true
        }
        for (auto childNnode : _pChildrenNodes)
        {
            if (childNnode->getIsChildNode(node))
            {
                return true; // 如果子节点的子节点包含目标节点，返回true
            }
        }
    }
    return false; // 否则返回false
}

// 获取节点在父节点中的行号
int ElaNavigationNode::getRow() const
{
    if (_pParentNode)
    {
        return _pParentNode->getChildrenNodes().indexOf(const_cast<ElaNavigationNode*>(this)); // 返回当前节点在父节点的子节点列表中的索引
    }
    return 0; // 如果没有父节点，返回0
}
