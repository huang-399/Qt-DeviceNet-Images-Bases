#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QToolButton>
#include "stripedtablewidget.h"
#include "checkableheader.h"
#include <QTableWidgetItem>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPoint>
#include <QDialog>
#include <QComboBox>
#include <QDialogButtonBox>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void updateNoDataItem();
    void loadData();
    void adjustNoDataLabelPosition();
    void onHeaderCheckStateChanged(Qt::CheckState state);
    void onItemCheckStateChanged(QTableWidgetItem *item);
    void onTogglePasswordVisibility();
    void on_pushButton_6_clicked();

    // Slots for custom title bar actions
    void onMinimizeClicked();
    void onMaximizeClicked();
    void onCloseClicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
    QPropertyAnimation *m_propertyAnimation;
    QPropertyAnimation *m_propertyAnimation2;
    bool m_bSideflag;
    QSize m_initSize;
    QAction *m_passwordAction;
    bool m_firstResize;
    QAction *m_searchAction;
    QToolButton *m_searchButton; // Button for the search icon

    QPropertyAnimation *m_tableAnimation;

    QPropertyAnimation *m_lineEdit14Animation;
    QPropertyAnimation *m_pushButton3Animation;
    QPropertyAnimation *m_pushButton4Animation;
    QPropertyAnimation *m_pushButton5Animation;

    // Point to store mouse position for dragging
    QPoint m_dragPosition;
    bool m_isDragging;

    // 新增：用于存储窗口正常状态下的几何信息
    QRect m_normalGeometry;
    bool m_isCustomMaximized; // 新增：自定义最大化状态标志

protected:
    void changeEvent(QEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override; // Override event filter
    void resizeEvent(QResizeEvent *event) override;             // 重写窗口大小改变事件
};
#endif // MAINWINDOW_H
