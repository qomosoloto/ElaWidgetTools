#include "ElaWindow.h"

#include <QApplication>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScreen>
#include <QStackedWidget>
#include <QStyleOption>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>

#include "ElaApplication.h"
#include "ElaCentralStackedWidget.h"
#include "ElaEventBus.h"
#include "ElaMenu.h"
#include "ElaNavigationBar.h"
#include "ElaNavigationRouter.h"
#include "ElaTheme.h"
#include "ElaWindowStyle.h"
#include "private/ElaAppBarPrivate.h"
#include "private/ElaNavigationBarPrivate.h"
#include "private/ElaWindowPrivate.h"
#include "ElaGlobalConfig.h"
Q_PROPERTY_CREATE_Q_CPP(ElaWindow, int, ThemeChangeTime)
Q_PROPERTY_CREATE_Q_CPP(ElaWindow, ElaNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
Q_TAKEOVER_NATIVEEVENT_CPP(ElaWindow, d_func()->_appBar);

// 构造函数
ElaWindow::ElaWindow(QWidget* parent)
    : QMainWindow{parent}, d_ptr(new ElaWindowPrivate())
{
    Q_D(ElaWindow);

    d->q_ptr = this;

    // 设置窗口属性
    setProperty("ElaBaseClassName", "ElaWindow");
    resize(1020, 680); // 默认宽高

    // 初始化主题变更时间和导航栏显示模式
    d->_pThemeChangeTime = 700;
    d->_pNavigationBarDisplayMode = ElaNavigationType::NavigationDisplayMode::Auto;
    connect(this, &ElaWindow::pNavigationBarDisplayModeChanged, d, &ElaWindowPrivate::onDisplayModeChanged);

    // 自定义AppBar
    d->_appBar = new ElaAppBar(this);
    connect(d->_appBar, &ElaAppBar::routeBackButtonClicked, this, []()
    {
        ElaNavigationRouter::getInstance()->navigationRouteBack();
    });
    connect(d->_appBar, &ElaAppBar::closeButtonClicked, this, &ElaWindow::closeButtonClicked);
    // 导航栏
    d->_navigationBar = new ElaNavigationBar(this);
    // 返回按钮状态变更
    connect(ElaNavigationRouter::getInstance(), &ElaNavigationRouter::navigationRouterStateChanged, this, [d
            ](bool isEnable)
            {
                d->_appBar->setRouteBackButtonEnable(isEnable);
            });

    // 转发用户卡片点击信号
    connect(d->_navigationBar, &ElaNavigationBar::userInfoCardClicked, this, &ElaWindow::userInfoCardClicked);
    // 转发点击信号
    connect(d->_navigationBar, &ElaNavigationBar::navigationNodeClicked, this, &ElaWindow::navigationNodeClicked);
    connect(d->_navigationBar, &ElaNavigationBar::navigationNodeClickedV2, this, &ElaWindow::navigationNodeClickedV2);
    //跳转处理
    connect(d->_navigationBar, &ElaNavigationBar::navigationNodeClicked, d, &ElaWindowPrivate::onNavigationNodeClicked);
    //新增窗口
    connect(d->_navigationBar, &ElaNavigationBar::navigationNodeAdded, d, &ElaWindowPrivate::onNavigationNodeAdded);

    // 中心堆栈窗口
    d->_centerStackedWidget = new ElaCentralStackedWidget(this);
    d->_centerStackedWidget->setContentsMargins(0, 0, 0, 0);
    QWidget* centralWidget = new QWidget(this);
    d->_centerLayout = new QHBoxLayout(centralWidget);
    d->_centerLayout->setSpacing(0);
    d->_centerLayout->addWidget(d->_navigationBar);
    d->_centerLayout->addWidget(d->_centerStackedWidget);
    d->_centerLayout->setContentsMargins(d->_contentsMargins, 0, 0, 0);

    // 事件总线
    d->_focusEvent = new ElaEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();

    // 展开导航栏
    connect(d->_appBar, &ElaAppBar::navigationButtonClicked, d, &ElaWindowPrivate::onNavigationButtonClicked);

    // 主题变更动画
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaWindowPrivate::onThemeModeChanged);
    connect(d->_appBar, &ElaAppBar::themeChangeButtonClicked, d, &ElaWindowPrivate::onThemeReadyChange);
    d->_isInitFinished = true;
    setCentralWidget(centralWidget);
    centralWidget->installEventFilter(this);

    setObjectName("ElaWindow");
    setStyleSheet("#ElaWindow{background-color:transparent;}");
    setStyle(new ElaWindowStyle(style()));

    //延时渲染
    QTimer::singleShot(1, this, [=]
    {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, ElaThemeColor(d->_themeMode, WindowBase));
        this->setPalette(palette);
    });
    eApp->syncMica(this);
    connect(eApp, &ElaApplication::pIsEnableMicaChanged, this, [=]()
    {
        d->onThemeModeChanged(d->_themeMode);
    });
}

// 析构函数
ElaWindow::~ElaWindow()
{
}

// 设置窗口是否置顶
void ElaWindow::setIsStayTop(bool isStayTop)
{
    Q_D(ElaWindow);
    d->_appBar->setIsStayTop(isStayTop);
    Q_EMIT pIsStayTopChanged();
}

// 获取窗口是否置顶
bool ElaWindow::getIsStayTop() const
{
    return d_ptr->_appBar->getIsStayTop();
}

// 设置窗口是否固定大小
void ElaWindow::setIsFixedSize(bool isFixedSize)
{
    Q_D(ElaWindow);
    d->_appBar->setIsFixedSize(isFixedSize);
    Q_EMIT pIsFixedSizeChanged();
}

// 获取窗口是否固定大小
bool ElaWindow::getIsFixedSize() const
{
    return d_ptr->_appBar->getIsFixedSize();
}

// 设置窗口是否默认关闭
void ElaWindow::setIsDefaultClosed(bool isDefaultClosed)
{
    Q_D(ElaWindow);
    d->_appBar->setIsDefaultClosed(isDefaultClosed);
    Q_EMIT pIsDefaultClosedChanged();
}

// 获取窗口是否默认关闭
bool ElaWindow::getIsDefaultClosed() const
{
    return d_ptr->_appBar->getIsDefaultClosed();
}

// 设置AppBar高度
void ElaWindow::setAppBarHeight(int appBarHeight)
{
    Q_D(ElaWindow);
    d->_appBar->setAppBarHeight(appBarHeight);
    Q_EMIT pAppBarHeightChanged();
}

// 获取AppBar高度
int ElaWindow::getAppBarHeight() const
{
    Q_D(const ElaWindow);
    return d->_appBar->getAppBarHeight();
}

// 获取自定义Widget
QWidget* ElaWindow::getCustomWidget() const
{
    Q_D(const ElaWindow);
    return d->_appBar->getCustomWidget();
}

// 设置自定义Widget最大宽度
void ElaWindow::setCustomWidgetMaximumWidth(int width)
{
    Q_D(ElaWindow);
    d->_appBar->setCustomWidgetMaximumWidth(width);
    Q_EMIT pCustomWidgetMaximumWidthChanged();
}

// 获取自定义Widget最大宽度
int ElaWindow::getCustomWidgetMaximumWidth() const
{
    Q_D(const ElaWindow);
    return d->_appBar->getCustomWidgetMaximumWidth();
}

// 设置中心堆栈窗口是否透明
void ElaWindow::setIsCentralStackedWidgetTransparent(bool isTransparent)
{
    Q_D(ElaWindow);
    d->_centerStackedWidget->setIsTransparent(isTransparent);
}

// 获取中心堆栈窗口是否透明
bool ElaWindow::getIsCentralStackedWidgetTransparent() const
{
    Q_D(const ElaWindow);
    return d->_centerStackedWidget->getIsTransparent();
}

// 移动窗口到屏幕中心
void ElaWindow::moveToCenter()
{
    if (isMaximized() || isFullScreen())
    {
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto geometry = screen()->availableGeometry();
#else
    auto geometry = qApp->screenAt(this->geometry().center())->geometry();
#endif
    setGeometry((geometry.left() + geometry.right() - width()) / 2, (geometry.top() + geometry.bottom() - height()) / 2,
                width(), height());
}

// 设置自定义Widget
void ElaWindow::setCustomWidget(ElaAppBarType::CustomArea customArea, QWidget* widget)
{
    Q_D(ElaWindow);
    d->_appBar->setCustomWidget(customArea, widget);
    Q_EMIT customWidgetChanged();
}

// 设置导航栏是否启用
void ElaWindow::setIsNavigationBarEnable(bool isVisible)
{
    Q_D(ElaWindow);
    d->_isNavigationEnable = isVisible;
    d->_navigationBar->setVisible(isVisible);
    d->_centerLayout->setContentsMargins(isVisible ? d->_contentsMargins : 0, 0, 0, 0);
    d->_centerStackedWidget->setIsHasRadius(isVisible);
}

// 获取导航栏是否启用
bool ElaWindow::getIsNavigationBarEnable() const
{
    return d_ptr->_isNavigationEnable;
}

// 设置用户卡片是否可见
void ElaWindow::setUserInfoCardVisible(bool isVisible)
{
    Q_D(ElaWindow);
    d->_navigationBar->setUserInfoCardVisible(isVisible);
}

// 设置用户卡片图片
void ElaWindow::setUserInfoCardPixmap(QPixmap pix)
{
    Q_D(ElaWindow);
    d->_navigationBar->setUserInfoCardPixmap(pix);
}

// 设置用户卡片标题
void ElaWindow::setUserInfoCardTitle(QString title)
{
    Q_D(ElaWindow);
    d->_navigationBar->setUserInfoCardTitle(title);
}

// 设置用户卡片副标题
void ElaWindow::setUserInfoCardSubTitle(QString subTitle)
{
    Q_D(ElaWindow);
    d->_navigationBar->setUserInfoCardSubTitle(subTitle);
}

// 添加扩展节点
ElaNavigationType::NodeOperateReturnType ElaWindow::addExpanderNode(QString expanderTitle, QString& expanderKey,
                                                                    ElaIconType::IconName awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addExpanderNode(expanderTitle, expanderKey, awesome);
}

// 添加扩展节点
ElaNavigationType::NodeOperateReturnType ElaWindow::addExpanderNode(QString expanderTitle, QString& expanderKey,
                                                                    QString targetExpanderKey,
                                                                    ElaIconType::IconName awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesome);
}

// 添加页面节点
ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page,
                                                                ElaIconType::IconName awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, awesome);
}

// 添加页面节点
ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page,
                                                                QString targetExpanderKey,
                                                                ElaIconType::IconName awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, awesome);
}

// 添加页面节点
ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page, int keyPoints,
                                                                ElaIconType::IconName awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, keyPoints, awesome);
}

// 添加页面节点
ElaNavigationType::NodeOperateReturnType ElaWindow::addPageNode(QString pageTitle, QWidget* page,
                                                                QString targetExpanderKey, int keyPoints,
                                                                ElaIconType::IconName awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, keyPoints, awesome);
}

// 添加页脚节点
ElaNavigationType::NodeOperateReturnType ElaWindow::addFooterNode(QString footerTitle, QString& footerKey,
                                                                  int keyPoints, ElaIconType::IconName awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addFooterNode(footerTitle, nullptr, footerKey, keyPoints, awesome);
}

// 添加页脚节点
ElaNavigationType::NodeOperateReturnType ElaWindow::addFooterNode(QString footerTitle, QWidget* page,
                                                                  QString& footerKey, int keyPoints,
                                                                  ElaIconType::IconName awesome) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->addFooterNode(footerTitle, page, footerKey, keyPoints, awesome);
}

// 设置节点关键点
void ElaWindow::setNodeKeyPoints(QString nodeKey, int keyPoints)
{
    Q_D(ElaWindow);
    d->_navigationBar->setNodeKeyPoints(nodeKey, keyPoints);
}

// 获取节点关键点
int ElaWindow::getNodeKeyPoints(QString nodeKey) const
{
    Q_D(const ElaWindow);
    return d->_navigationBar->getNodeKeyPoints(nodeKey);
}

// 导航到指定页面
void ElaWindow::navigation(QString pageKey)
{
    Q_D(ElaWindow);
    d->_navigationBar->navigation(pageKey);
}

// 设置窗口按钮标志
void ElaWindow::setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable)
{
    Q_D(ElaWindow);
    d->_appBar->setWindowButtonFlag(buttonFlag, isEnable);
}

// 设置窗口按钮标志
void ElaWindow::setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags)
{
    Q_D(ElaWindow);
    d->_appBar->setWindowButtonFlags(buttonFlags);
}

// 获取窗口按钮标志
ElaAppBarType::ButtonFlags ElaWindow::getWindowButtonFlags() const
{
    return d_ptr->_appBar->getWindowButtonFlags();
}

// 关闭窗口
void ElaWindow::closeWindow()
{
    Q_D(ElaWindow);
    d->_isWindowClosing = true;
    d->_appBar->closeWindow();
}

// 获取当前页面Widget
QWidget* ElaWindow::getCurrentPageWidget()
{
    Q_D(ElaWindow);
    return d->getCurrentWidget(); // 放在使用的地方再做类型转换
}

// 事件过滤器
bool ElaWindow::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(ElaWindow);
    switch (event->type())
    {
    case QEvent::Resize:
        {
            d->_doNavigationDisplayModeChange();
            break;
        }
    default:
        {
            break;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

// 创建弹出菜单
QMenu* ElaWindow::createPopupMenu()
{
    ElaMenu* menu = nullptr;
    QList<QDockWidget*> dockwidgets = findChildren<QDockWidget*>();
    if (dockwidgets.size())
    {
        menu = new ElaMenu(this);
        for (int i = 0; i < dockwidgets.size(); ++i)
        {
            QDockWidget* dockWidget = dockwidgets.at(i);
            if (dockWidget->parentWidget() == this)
            {
                menu->addAction(dockwidgets.at(i)->toggleViewAction());
            }
        }
        menu->addSeparator();
    }

    QList<QToolBar*> toolbars = findChildren<QToolBar*>();
    if (toolbars.size())
    {
        if (!menu)
        {
            menu = new ElaMenu(this);
        }
        for (int i = 0; i < toolbars.size(); ++i)
        {
            QToolBar* toolBar = toolbars.at(i);
            if (toolBar->parentWidget() == this)
            {
                menu->addAction(toolbars.at(i)->toggleViewAction());
            }
        }
    }
    if (menu)
    {
        menu->setMenuItemHeight(28);
    }
    return menu;
}
