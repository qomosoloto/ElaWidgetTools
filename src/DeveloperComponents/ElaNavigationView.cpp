#include "ElaNavigationView.h"

#include <QDebug>
#include <QHeaderView>
#include <QModelIndex>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QScroller>

#include "ElaMenu.h"
#include "ElaNavigationModel.h"
#include "ElaNavigationNode.h"
#include "ElaNavigationStyle.h"
#include "ElaScrollBar.h"

// 构造函数，初始化ElaNavigationView对象
ElaNavigationView::ElaNavigationView(QWidget* parent)
    : QTreeView(parent)
{
    // 设置对象名称，用于样式表引用
    setObjectName("ElaNavigationView");
    // 设置背景透明
    setStyleSheet("#ElaNavigationView{background-color:transparent;}");
    // 启用动画效果
    setAnimated(true);
    // 隐藏表头
    setHeaderHidden(true);
    // 设置根节点不显示装饰
    setRootIsDecorated(false);
    // 双击不展开节点
    setExpandsOnDoubleClick(false);
    // 禁用自动滚动
    setAutoScroll(false);
    // 启用鼠标跟踪
    setMouseTracking(true);
    // 设置选择模式为不选择
    setSelectionMode(QAbstractItemView::NoSelection);

    // 滚动条设置
    // 禁用水平滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 创建垂直滚动条
    ElaScrollBar* vScrollBar = new ElaScrollBar(this);
    // 连接滚动条范围动画结束信号到布局更新槽
    connect(vScrollBar, &ElaScrollBar::rangeAnimationFinished, this, [=]() {
        doItemsLayout();
    });
    // 设置垂直滚动条
    setVerticalScrollBar(vScrollBar);
    // 禁用垂直滚动条
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 设置垂直滚动模式为按像素滚动
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    // 创建浮动垂直滚动条
    ElaScrollBar* floatVScrollBar = new ElaScrollBar(vScrollBar, this);
    // 启用浮动滚动条动画
    floatVScrollBar->setIsAnimation(true);

    // 创建导航样式对象
    _navigationStyle = new ElaNavigationStyle(this->style());
    // 设置导航视图
    _navigationStyle->setNavigationView(this);
    // 应用样式
    setStyle(_navigationStyle);

    // 捕获鼠标手势
    QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
    // 获取滚动器对象
    QScroller* scroller = QScroller::scroller(this->viewport());
    // 获取滚动属性
    QScrollerProperties properties = scroller->scrollerProperties();
    // 设置鼠标按下事件延迟为0
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    // 设置水平滚动溢出策略为始终关闭
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    // 设置垂直滚动溢出策略为始终开启
    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    // 设置溢出拖动阻力因子
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    // 设置溢出滚动时间
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    // 设置帧率为60fps
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    // 应用滚动属性
    scroller->setScrollerProperties(properties);

    // 连接滚动器状态变化信号到处理槽
    connect(scroller, &QScroller::stateChanged, this, [=](QScroller::State newstate) {
        if (newstate == QScroller::Pressed)
        {
            // 设置按下索引
            _navigationStyle->setPressIndex(indexAt(mapFromGlobal(QCursor::pos())));
            // 更新视图
            viewport()->update();
        }
        else if (newstate == QScroller::Scrolling || newstate == QScroller::Inactive)
        {
            // 清除按下索引
            _navigationStyle->setPressIndex(QModelIndex());
        }
    });

    // 设置上下文菜单策略为自定义
    setContextMenuPolicy(Qt::CustomContextMenu);
    // 连接自定义上下文菜单请求信号到处理槽
    connect(this, &ElaNavigationView::customContextMenuRequested, this, &ElaNavigationView::onCustomContextMenuRequested);
}

// 析构函数，清理资源
ElaNavigationView::~ElaNavigationView()
{
}

// 导航节点状态变化处理
void ElaNavigationView::navigationNodeStateChange(QVariantMap data)
{
    this->_navigationStyle->navigationNodeStateChange(data);
}

// 自定义上下文菜单请求处理
void ElaNavigationView::onCustomContextMenuRequested(const QPoint& pos)
{
    // 获取点击位置的索引
    QModelIndex posIndex = indexAt(pos);
    if (!posIndex.isValid())
    {
        return;
    }
    // 获取点击位置的节点
    ElaNavigationNode* posNode = static_cast<ElaNavigationNode*>(posIndex.internalPointer());
    if (!posNode->getIsExpanderNode())
    {
        // 创建菜单
        ElaMenu menu;
        // 设置菜单项高度
        menu.setMenuItemHeight(27);
        // 添加菜单项
        QAction* openAction = menu.addElaIconAction(ElaIconType::ObjectGroup, "在新窗口中打开");
        // 连接菜单项触发信号到处理槽
        connect(openAction, &QAction::triggered, this, [=]() {
            Q_EMIT navigationOpenNewWindow(posNode->getNodeKey());
        });
        // 显示菜单
        menu.exec(mapToGlobal(pos));
    }
}

// 鼠标双击事件处理
void ElaNavigationView::mouseDoubleClickEvent(QMouseEvent* event)
{
    // 设置按下索引
    _navigationStyle->setPressIndex(indexAt(event->pos()));
    // 更新视图
    viewport()->update();
    // 调用基类处理
    QTreeView::mouseDoubleClickEvent(event);
}

// 鼠标释放事件处理
void ElaNavigationView::mouseReleaseEvent(QMouseEvent* event)
{
    // 调用基类处理
    QTreeView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        // 获取点击位置的索引
        QModelIndex index = indexAt(event->pos());
        if (index.isValid())
        {
            // 发射导航点击信号
            Q_EMIT navigationClicked(index);
        }
        // 清除按下索引
        _navigationStyle->setPressIndex(QModelIndex());
    }
}
