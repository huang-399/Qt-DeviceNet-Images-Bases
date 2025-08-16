#ifndef STRIPEDTABLEWIDGET_H
#define STRIPEDTABLEWIDGET_H

#include <QTableWidget>
#include <QPainter>
#include <QHeaderView>

class StripedTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit StripedTableWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // STRIPEDTABLEWIDGET_H
