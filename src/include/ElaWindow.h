#ifndef ELAWINDOW_H
#define ELAWINDOW_H

#include <QMainWindow> // 包含QMainWindow头文件，ElaWindow继承自QMainWindow

#include "Def.h" // 包含自定义定义的头文件
#include "ElaAppBar.h" // 包含ElaAppBar头文件，可能定义了应用栏的相关类
#include "ElaNavigationNode.h" // 包含ElaNavigationNode头文件，可能定义了导航节点的相关类
#include "stdafx.h" // 包含预编译头文件
class ElaWindowPrivate; // 前向声明ElaWindowPrivate类，用于实现Pimpl模式

// ELA_EXPORT宏用于导出类，使其可以在其他模块中使用
class ELA_EXPORT ElaWindow : public QMainWindow
{
    Q_OBJECT // 使类支持Qt的信号和槽机制
    // Q_Q_CREATE宏用于创建Qt元对象系统相关的代码
    Q_Q_CREATE(ElaWindow)


    // Q_PROPERTY_CREATE_Q_H宏用于创建Qt属性系统相关的代码
    Q_PROPERTY_CREATE_Q_H(bool, IsStayTop) // 是否置顶
    Q_PROPERTY_CREATE_Q_H(bool, IsFixedSize) // 是否固定大小
    Q_PROPERTY_CREATE_Q_H(bool, IsDefaultClosed) // 是否默认关闭
    Q_PROPERTY_CREATE_Q_H(int, AppBarHeight) // 应用栏高度
    Q_PROPERTY_CREATE_Q_H(int, CustomWidgetMaximumWidth) // 自定义部件最大宽度
    Q_PROPERTY_CREATE_Q_H(int, ThemeChangeTime) // 主题切换时间
    Q_PROPERTY_CREATE_Q_H(bool, IsCentralStackedWidgetTransparent) // 中央堆叠部件是否透明
    Q_PROPERTY_CREATE_Q_H(ElaNavigationType::NavigationDisplayMode, NavigationBarDisplayMode) // 导航栏显示模式
    Q_TAKEOVER_NATIVEEVENT_H // 用于处理原生事件

public:
    // 构造函数，parent为父窗口指针
    explicit ElaWindow(QWidget* parent = nullptr);
    // 析构函数
    ~ElaWindow();

    // 将窗口移动到屏幕中心
    void moveToCenter();

    // 设置自定义部件，customArea指定区域，customWidget为自定义部件指针
    void setCustomWidget(ElaAppBarType::CustomArea customArea, QWidget* customWidget);
    // 获取自定义部件
    QWidget* getCustomWidget() const;
    // 设置导航栏是否启用
    void setIsNavigationBarEnable(bool isEnable);
    // 获取导航栏是否启用
    bool getIsNavigationBarEnable() const;
    // 设置用户信息卡可见性
    void setUserInfoCardVisible(bool isVisible);
    // 设置用户信息卡图片
    void setUserInfoCardPixmap(QPixmap pix);
    // 设置用户信息卡标题
    void setUserInfoCardTitle(QString title);
    // 设置用户信息卡副标题
    void setUserInfoCardSubTitle(QString subTitle);
    // 添加扩展节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey,
                                                             ElaIconType::IconName awesome = ElaIconType::None) const;
    // 添加扩展节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey,
                                                             QString targetExpanderKey,
                                                             ElaIconType::IconName awesome = ElaIconType::None) const;
    // 添加页面节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page,
                                                         ElaIconType::IconName awesome = ElaIconType::None) const;
    // 添加页面节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey,
                                                         ElaIconType::IconName awesome = ElaIconType::None) const;
    // 添加页面节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, int keyPoints = 0,
                                                         ElaIconType::IconName awesome = ElaIconType::None) const;
    // 添加页面节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey,
                                                         int keyPoints = 0,
                                                         ElaIconType::IconName awesome = ElaIconType::None) const;
    // 添加页脚节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QString& footerKey, int keyPoints = 0,
                                                           ElaIconType::IconName awesome = ElaIconType::None) const;
    // 添加页脚节点，返回操作结果
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QWidget* page, QString& footerKey,
                                                           int keyPoints = 0,
                                                           ElaIconType::IconName awesome = ElaIconType::None) const;

    // 设置节点关键点
    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    // 获取节点关键点
    int getNodeKeyPoints(QString nodeKey) const;

    // 导航到指定页面
    void navigation(QString pageKey);
    // 设置窗口按钮标志
    void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true);
    // 设置窗口按钮标志
    void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags);
    // 获取窗口按钮标志
    ElaAppBarType::ButtonFlags getWindowButtonFlags() const;

    // 关闭窗口
    void closeWindow();
    // 获取当前页面部件
    QWidget* getCurrentPageWidget();
Q_SIGNALS:
    // 用户信息卡点击信号
    Q_SIGNAL void userInfoCardClicked();
    // 关闭按钮点击信号
    Q_SIGNAL void closeButtonClicked();
    // 导航节点点击信号
    Q_SIGNAL void navigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey);
    // 导航节点点击信号（版本2）
    Q_SIGNAL void navigationNodeClickedV2(ElaNavigationType::NavigationNodeType nodeType, ElaNavigationNode* node);
    // 自定义部件变化信号
    Q_SIGNAL void customWidgetChanged();

protected:
    // 事件过滤器，用于拦截和处理事件
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    // 创建弹出菜单
    virtual QMenu* createPopupMenu() override;
};

#endif // ELAWINDOW_H
