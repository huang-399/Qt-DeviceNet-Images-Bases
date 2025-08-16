#include "checkableheader.h"
#include <QStyleOptionButton>
#include <QApplication>

// 构造函数实现
CheckableHeaderView::CheckableHeaderView(Qt::Orientation orientation, int checkColumn, QWidget *parent)
    : QHeaderView(orientation, parent),
      m_checkColumn(checkColumn),
      m_checkState(Qt::Unchecked), // 初始状态为未勾选
      m_isHoveringOverCheckbox(false)
{
    setSectionsClickable(true);
    setHighlightSections(false);
    setMouseTracking(true); // 开启鼠标追踪以接收悬停事件
}

// 获取当前复选框状态的实现
Qt::CheckState CheckableHeaderView::checkState() const
{
    return m_checkState;
}

// 设置复选框状态的实现
void CheckableHeaderView::setCheckState(Qt::CheckState state)
{
    if (m_checkState != state)
    {
        m_checkState = state;
        viewport()->update();
    }
}

void CheckableHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    // 先调用基类方法绘制默认的表头（背景、文字等）
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();

    if (logicalIndex == m_checkColumn)
    {
        // 1. 无论如何，都先绘制一个标准的复选框作为底层
        QRect checkBoxRect(rect.left() + (rect.width() - 20) / 2,
                           rect.top() + (rect.height() - 20) / 2,
                           20, 20);
        QStyleOptionButton checkBoxOption;
        checkBoxOption.rect = checkBoxRect;
        checkBoxOption.state = QStyle::State_Enabled;

        // 根据实际状态（或在悬停时强制为未选中）来设置
        if (m_isHoveringOverCheckbox && this->model() && this->model()->rowCount() == 0) {
            checkBoxOption.state |= QStyle::State_Off;
        } else {
            if (m_checkState == Qt::Checked)
            {
                checkBoxOption.state |= QStyle::State_On;
            }
            else if (m_checkState == Qt::PartiallyChecked)
            {
                checkBoxOption.state |= QStyle::State_NoChange;
            }
            else
            {
                checkBoxOption.state |= QStyle::State_Off;
            }
        }
        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxOption, painter);

        // 2. 如果满足悬停条件，就在上面叠加一个半透明的红色方块
        if (this->model() && this->model()->rowCount() == 0 && m_isHoveringOverCheckbox)
        {
            // 定义一个10x10的、居中的小方块区域
            QRect smallSquareRect(rect.left() + (rect.width() - 12) / 2,
                                  rect.top() + (rect.height() - 12) / 2,
                                  11, 11);
            // 使用半透明的红色填充
            painter->fillRect(smallSquareRect, QColor(255, 0, 0, 100));
        }
    }
}

// 处理鼠标点击事件的实现
void CheckableHeaderView::mousePressEvent(QMouseEvent *event)
{
    int logicalIndex = logicalIndexAt(event->pos());

    if (logicalIndex == m_checkColumn)
    {
        if (this->model() && this->model()->rowCount() > 0)
        {
            QRect sectionRect(sectionPosition(logicalIndex), 0, sectionSize(logicalIndex), height());
            QRect checkBoxRect = QRect(sectionRect.left() + (sectionRect.width() - 20) / 2,
                                       sectionRect.top() + (sectionRect.height() - 20) / 2,
                                       20, 20);

            if (checkBoxRect.contains(event->pos()))
            {
                if (m_checkState == Qt::Checked)
                {
                    m_checkState = Qt::Unchecked;
                }
                else
                {
                    m_checkState = Qt::Checked;
                }
                viewport()->update();
                emit checkStateChanged(m_checkState);
            }
        }
        return;
    }

    QHeaderView::mousePressEvent(event);
}

void CheckableHeaderView::mouseMoveEvent(QMouseEvent *event)
{
    // 只要表格为空，鼠标在任何表头上方移动都视为悬停状态
    bool isHovering = (this->model() && this->model()->rowCount() == 0);

    // 如果悬停状态发生变化，则更新并重绘
    if (m_isHoveringOverCheckbox != isHovering)
    {
        m_isHoveringOverCheckbox = isHovering;
        viewport()->update();
    }

    QHeaderView::mouseMoveEvent(event);
}

void CheckableHeaderView::leaveEvent(QEvent *event)
{
    if (m_isHoveringOverCheckbox)
    {
        m_isHoveringOverCheckbox = false;
        viewport()->update();
    }
    QHeaderView::leaveEvent(event);
}
