#include "stripedtablewidget.h"

StripedTableWidget::StripedTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    // 确保交替行颜色属性是开启的，这样当有数据时，系统会自动处理
    setAlternatingRowColors(true);
}

void StripedTableWidget::paintEvent(QPaintEvent *event)
{
    // 1. 首先调用基类的 paintEvent，让它完成所有常规绘制（如表头、有数据的行等）
    QTableWidget::paintEvent(event);

    // 2. 如果表格的行数为0，我们才进行自定义绘制
    if (model()->rowCount() == 0)
    {
        QPainter painter(this->viewport());
        if (!painter.isActive()) {
            return;
        }

        // 3. 使用在样式表中定义的颜色
        QColor baseColor = QColor("white");
        QColor alternateColor = QColor("#f5f5f5");

        // 4. 获取行高
        // 如果表头可见，则使用表头定义的行高，否则使用一个合理的默认值
        int rowHeight = verticalHeader()->isVisible() ? verticalHeader()->defaultSectionSize() : 25;
        if (rowHeight <= 0) {
            return; // 防止除零或无限循环
        }


        // 5. 从 viewport 的顶部开始，循环绘制背景条纹
        int top = 0;
        int width = viewport()->width();
        bool alternate = false; // 用于切换颜色

        while (top < viewport()->height())
        {
            QRect rect(0, top, width, rowHeight);

            // 根据 alternate 标志选择颜色并填充矩形
            if (alternate) {
                painter.fillRect(rect, alternateColor);
            } else {
                painter.fillRect(rect, baseColor);
            }

            top += rowHeight;      // 移动到下一行
            alternate = !alternate; // 切换颜色标志
        }
    }
}
