#ifndef ELANAVIGATIONVIEW_H
#define ELANAVIGATIONVIEW_H

#include <QObject>
#include <QTreeView>

// 声明ElaScrollBar类，用于自定义滚动条
class ElaScrollBar;
// 声明ElaNavigationStyle类，用于自定义导航视图的样式
class ElaNavigationStyle;
// ElaNavigationView类继承自QTreeView，用于实现自定义的导航视图
class ElaNavigationView : public QTreeView
{
    Q_OBJECT // 使类支持信号和槽机制
public:
    // 构造函数，用于创建ElaNavigationView对象，可选参数parent指定父窗口
    explicit ElaNavigationView(QWidget* parent = nullptr);
    // 析构函数，用于销毁ElaNavigationView对象
    ~ElaNavigationView();
    // 用于处理导航节点状态变化的函数，参数data包含状态信息
    void navigationNodeStateChange(QVariantMap data);

    // 自定义上下文菜单请求的槽函数，参数pos为鼠标点击位置
    Q_SLOT void onCustomContextMenuRequested(const QPoint& pos);
Q_SIGNALS:
    // 导航节点被点击时发出的信号，参数index为被点击的节点索引
    Q_SIGNAL void navigationClicked(const QModelIndex& index);
    // 导航节点要求打开新窗口时发出的信号，参数nodeKey为节点标识
    Q_SIGNAL void navigationOpenNewWindow(QString nodeKey);

protected:
    // 重写mouseDoubleClickEvent函数，处理鼠标双击事件
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    // 重写mouseReleaseEvent函数，处理鼠标释放事件
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
    // 指向自定义导航样式的指针，初始值为nullptr
    ElaNavigationStyle* _navigationStyle{nullptr};
};

#endif // ELANAVIGATIONVIEW_H
