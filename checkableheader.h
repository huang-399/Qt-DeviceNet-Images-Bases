#ifndef CHECKABLEHEADER_H
#define CHECKABLEHEADER_H

#include <QHeaderView>
#include <QCheckBox>
#include <QPainter>
#include <QMouseEvent>

// CheckableHeaderView 类继承自 QHeaderView
class CheckableHeaderView : public QHeaderView
{
    Q_OBJECT

public:
    // 构造函数：orientation 指定是水平还是垂直表头，parent 是父控件
    // checkColumn 参数指定哪一列将包含复选框（这里通常是第0列）
    CheckableHeaderView(Qt::Orientation orientation, int checkColumn, QWidget *parent = nullptr);

    // 获取当前复选框的状态
    Qt::CheckState checkState() const;
    // 设置复选框的状态
    void setCheckState(Qt::CheckState state);

signals:
    // 当表头复选框状态改变时发出的信号
    void checkStateChanged(Qt::CheckState state);

protected:
    // 重写 paintSection 方法来绘制复选框
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
    // 重写 mousePressEvent 方法来处理鼠标点击事件
    void mousePressEvent(QMouseEvent *event) override;
    // 重写 mouseMoveEvent 来处理鼠标悬停
    void mouseMoveEvent(QMouseEvent *event) override;
    // 重写 leaveEvent 来处理鼠标离开
    void leaveEvent(QEvent *event) override;

private:
    int m_checkColumn;          // 指定哪一列是带有复选框的列
    Qt::CheckState m_checkState; // 存储复选框的当前状态
    bool m_isHoveringOverCheckbox; // 追踪鼠标是否悬停在复选框上
};

#endif // CHECKABLEHEADER_H
