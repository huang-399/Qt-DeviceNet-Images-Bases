#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QStyle>
#include <QTimer>
#include <Qdebug>
#include <QDesktopServices>
#include <QApplication> // 新增
#include <QDesktopWidget> // 新增

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- Custom Title Bar Implementation ---
    // 1. Hide the default window frame
    setWindowFlags(Qt::FramelessWindowHint);

    // 2. Connect the custom buttons to window slots
    connect(ui->minimizeButton, &QPushButton::clicked, this, &MainWindow::onMinimizeClicked);
    connect(ui->maximizeButton, &QPushButton::clicked, this, &MainWindow::onMaximizeClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::onCloseClicked);
    // connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::on_pushButton_6_clicked);
    //  --- End of Custom Title Bar Implementation ---

    m_firstResize = true;

    // 初始化 m_normalGeometry
    m_normalGeometry = geometry();
    m_isCustomMaximized = false; // 初始化自定义最大化状态为 false

    // --- auto-generated code to disable sidebar widgets ---
    QList<QWidget *> widgetsToDisable = {
        ui->lineEdit, ui->lineEdit_2, ui->lineEdit_3, ui->lineEdit_4,
        ui->lineEdit_5, ui->lineEdit_6, ui->lineEdit_7, ui->lineEdit_8,
        ui->lineEdit_9, ui->lineEdit_10, ui->lineEdit_11, ui->lineEdit_12,
        ui->lineEdit_13,
        ui->label_3, ui->label_6, ui->label_7, ui->label_8, ui->label_9,
        ui->label_10, ui->label_11, ui->label_12, ui->label_13, ui->label_14,
        ui->label_15, ui->label_16, ui->label_17, ui->label_18,
        ui->checkBox_3, ui->checkBox_4,
        ui->pushButton_2};

    for (QWidget *widget : widgetsToDisable)
    {
        widget->setEnabled(false);
    }
    // --- end of auto-generated code ---

    // --- auto-generated code for password field ---
    // Common style for all QLineEdits in the sidebar
    QString lineEditStyle = "QLineEdit {\n"
                            "    background-color: #f5f5f5;\n" // Light grey background
                            "    border: 1px solid #E0E0E0;\n"
                            "    border-radius: 5px;\n"
                            "    padding: 5px;\n"
                            "}\n"
                            "QLineEdit:focus {\n"
                            "    border: 1px solid #A0A0A0;\n"
                            "}";

    // Apply the style to all line edits in the sidebar
    QList<QLineEdit *> lineEdits = ui->widget_side->findChildren<QLineEdit *>();
    for (QLineEdit *lineEdit : lineEdits)
    {
        lineEdit->setStyleSheet(lineEditStyle);
    }

    ui->lineEdit_13->setEchoMode(QLineEdit::Password);
    m_passwordAction = ui->lineEdit_13->addAction(QIcon(":/images/eyes.png"), QLineEdit::TrailingPosition);
    connect(m_passwordAction, &QAction::triggered, this, &MainWindow::onTogglePasswordVisibility);

    // 在 MainWindow 构造函数中添加以下代码

    // Style for the "修改" button, matching the target image
    ui->pushButton_2->setStyleSheet(
        "QPushButton {\n"
        "    background-color: #FFCDD2;\n" // Very light red
        "    color: white;\n"              // White text
        "    border-radius: 5px;\n"
        "    padding: 5px;\n"
        "    border: none;\n"
        "}\n"
        "QPushButton:hover {\n"
        "    background-color: #EF9A9A;\n" // Slightly darker on hover
        "}\n"
        "QPushButton:pressed {\n"
        "    background-color: #E57373;\n" // Even darker when pressed
        "}");

    // --- Style for the filter search box (lineEdit_14) ---
    ui->lineEdit_14->setStyleSheet(
        "QLineEdit {\n"
        "    background-color: #f5f5f5;\n"
        "    border: 1px solid #E0E0E0;\n"
        "    border-radius: 0px;\n"
        "    padding-left: 5px;\n"
        "    padding-right: 25px;\n" // Make space for the icon on the right
        "}\n"
        "QLineEdit:focus {\n"
        "    border: 1px solid #A0A0A0;\n"
        "}");
    m_searchAction = ui->lineEdit_14->addAction(QIcon(":/images/search_icon.svg"), QLineEdit::TrailingPosition);
    ui->lineEdit_14->installEventFilter(this);

    ui->pushButton_4->setStyleSheet(
        "QPushButton {\n"
        "    background-color: #FFCDD2;\n" // Very light red
        "    color: white;\n"              // White text
        "    border-radius: 5px;\n"
        "    padding: 5px;\n"
        "    border: none;\n"
        "}\n"
        "QPushButton:hover {\n"
        "    background-color: #EF9A9A;\n" // Slightly darker on hover
        "}\n"
        "QPushButton:pressed {\n"
        "    background-color: #E57373;\n" // Even darker when pressed
        "}");

    ui->pushButton_5->setStyleSheet(
        "QPushButton {\n"
        "    background-color: #FFCDD2;\n" // Very light red
        "    color: white;\n"              // White text
        "    border-radius: 5px;\n"
        "    padding: 5px;\n"
        "    border: none;\n"
        "}\n"
        "QPushButton:hover {\n"
        "    background-color: #EF9A9A;\n" // Slightly darker on hover
        "}\n"
        "QPushButton:pressed {\n"
        "    background-color: #E57373;\n" // Even darker when pressed
        "}");
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);

    // --- end of auto-generated code ---

    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Set smooth, pixel-by-pixel scrolling for a more direct feel
    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    m_initSize = this->size(); // 保存初始大小

    // 禁止拖动改变窗口大小，但允许最大化
    // This is commented out because resizing is now handled by the custom title bar
    // this->setFixedSize(m_initSize);
    // this->setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    int sideWidth = ui->widget_side->width();
    int btnWidth = ui->pushButton->width();
    int btnHeight = ui->pushButton->height();
    int winWidth = this->width();
    int winHeight = this->height();
    int btnY = winHeight / 3 - btnHeight / 2;
    int sideHeight = ui->widget_side->height();

    // 默认展开：侧边栏在右侧展开位置
    ui->widget_side->move(winWidth - sideWidth, 40); // Adjusted for title bar
    // 按钮在 widget_side 左侧
    ui->pushButton->move(winWidth - sideWidth - btnWidth, btnY);

    // 初始化动画对象
    m_propertyAnimation = new QPropertyAnimation(ui->widget_side, "geometry", this);
    m_propertyAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_propertyAnimation->setDuration(800);

    m_propertyAnimation2 = new QPropertyAnimation(ui->pushButton, "geometry", this);
    m_propertyAnimation2->setEasingCurve(QEasingCurve::InOutSine);
    m_propertyAnimation2->setDuration(800);

    // 在其他动画初始化后面添加
    m_tableAnimation = new QPropertyAnimation(ui->tableWidget, "geometry", this);
    m_tableAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_tableAnimation->setDuration(800);

    m_lineEdit14Animation = new QPropertyAnimation(ui->lineEdit_14, "geometry", this);
    m_lineEdit14Animation->setEasingCurve(QEasingCurve::InOutSine);
    m_lineEdit14Animation->setDuration(800);

    m_pushButton3Animation = new QPropertyAnimation(ui->pushButton_3, "geometry", this);
    m_pushButton3Animation->setEasingCurve(QEasingCurve::InOutSine);
    m_pushButton3Animation->setDuration(800);

    m_pushButton4Animation = new QPropertyAnimation(ui->pushButton_4, "geometry", this);
    m_pushButton4Animation->setEasingCurve(QEasingCurve::InOutSine);
    m_pushButton4Animation->setDuration(800);

    m_pushButton5Animation = new QPropertyAnimation(ui->pushButton_5, "geometry", this);
    m_pushButton5Animation->setEasingCurve(QEasingCurve::InOutSine);
    m_pushButton5Animation->setDuration(800);

    m_bSideflag = true;           // 默认展开
    ui->pushButton->setText(">"); // 默认展开时按钮文本为 ">>"

    ui->tableWidget->setStyleSheet(
        "QTableWidget {\n"
        "   background-color: white;\n"             // Set background to white
        "   alternate-background-color: #f5f5f5;\n" // Set alternating to light grey
        "   gridline-color: #E0E0E0;\n"             // Lighter gridline color
        "   border: 1px solid #E0E0E0;\n"           // Lighter border
        "}\n"
        "QTableWidget::item {\n"
        "   padding: 4px;\n"
        "}\n"
        "QHeaderView::section {\n"
        "   background-color: #606060;\n"
        "   color: #D0D0D0;\n"
        "   border: 1px solid darkgrey;\n"
        "   padding: 4px;\n"
        "}\n"
        "QHeaderView::section:hover {\n"
        "    background-color: #757575;\n" // A slightly lighter grey for hover
        "}\n"
        "QHeaderView::down-arrow {\n"
        "    image: url(:/images/down_arrow.svg);\n"
        "    width: 12px;\n"
        "    height: 12px;\n"
        "}\n"
        "QHeaderView::up-arrow {\n"
        "    image: url(:/images/up_arrow.svg);\n"
        "    width: 12px;\n"
        "    height: 12px;\n"
        "}\n"
        "QScrollBar:vertical {\n"
        "    border: none;\n"
        "    background: #F5F5F5;\n"
        "    width: 12px;\n" // Wider scrollbar
        "    margin: 0px 0px 0px 0px;\n"
        "}\n"
        "QScrollBar::handle:vertical {\n"
        "    background: #BDBDBD;\n" // Grey handle
        "    min-height: 20px;\n"
        "    border-radius: 6px;\n"
        "}\n"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
        "    height: 0px;\n"
        "}\n"
        "QScrollBar:horizontal {\n"
        "    border: none;\n"
        "    background: #E0E0E0;\n" // Lighter grey for the track\n"        "    height: 12px;\n"        "    margin: 0px 0px 0px 0px;\n"        "}\n"        "QScrollBar::handle:horizontal {\n"        "    background: #A0A0A0;\n"      // Darker grey for the handle\n"        "    min-width: 20px;\n"        "    border-radius: 6px;\n"        "}\n"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {\n"
        "    width: 0px;\n"
        "}");

    // 设置表格属性
    ui->tableWidget->setRowCount(0);                                // 初始行数为0
    ui->tableWidget->setColumnCount(27);                            // 26列
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << " " // 新增的复选框列的标题，可以为空或“选择”
                                                             << "编号" << "设备类型" << "激活状态"
                                                             << "IP地址" << "端口" << "服务管理型端口" << "软件版本"
                                                             << "IPv4网关" << "HTTP端口" << "设备序列号"
                                                             << "子网掩码" << "物理地址" << "编码通道数" << "DSP版本"
                                                             << "启动时间" << "IPv6地址" << "IPv6网关" << "IPv6子网前缀长度"
                                                             << "是否支持IPv6" << "是否支持修改IPv6" << "是否支持DHCP"
                                                             << "IPv4 DHCP状态" << "支持萤石云" << "萤石云状态" << "萤石云绑定状态"
                                                             << "萤石云版本");
    // [cite:1]

    // 创建 CheckableHeaderView 实例，并指定复选框在第0列
    CheckableHeaderView *header = new CheckableHeaderView(Qt::Horizontal, 0, ui->tableWidget);
    ui->tableWidget->setHorizontalHeader(header);

    // 通过代码设置表头文本左对齐，这是最可靠的方法
    header->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Enable sorting for the table
    ui->tableWidget->setSortingEnabled(true);

    // Forcefully clear any initial sort indicator that might be set by default on the header
    header->setSortIndicator(-1, Qt::AscendingOrder);

    // 连接表头复选框的信号到槽函数
    connect(header, &CheckableHeaderView::checkStateChanged, this, &MainWindow::onHeaderCheckStateChanged);

    // 在 MainWindow 构造函数中添加以下代码

    // 在构造函数中设置 nolabel 的父级关系
    ui->nolabel->setParent(ui->tableWidget);
    ui->nolabel->raise(); // 确保 nolabel 在 tableWidget 的内容之上显示
    // 设置 NoData 标签样式
    ui->nolabel->setAlignment(Qt::AlignCenter);
    ui->nolabel->setStyleSheet("QLabel {\n"
                               "   color: gray;\n"
                               "   font-size: 18px;\n"
                               "   background-color: transparent;\n"
                               "}");
    // 初始隐藏 NoData 标签
    ui->nolabel->hide();

    // 添加 "NoData" 项
    updateNoDataItem();

    // 连接信号槽
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::updateNoDataItem);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::onItemCheckStateChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->lineEdit_14)
    {
        if (event->type() == QEvent::Enter)
        {
            // Mouse entered the QLineEdit
            m_searchAction->setIcon(QIcon(":/images/search_icon_red.svg"));
            return true;
        }
        if (event->type() == QEvent::Leave)
        {
            // Mouse left the QLineEdit
            m_searchAction->setIcon(QIcon(":/images/search_icon.svg"));
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::onTogglePasswordVisibility()
{
    if (ui->lineEdit_13->echoMode() == QLineEdit::Password)
    {
        ui->lineEdit_13->setEchoMode(QLineEdit::Normal);
        m_passwordAction->setIcon(QIcon(":/images/eyes_open.png"));
    }
    else
    {
        ui->lineEdit_13->setEchoMode(QLineEdit::Password);
        m_passwordAction->setIcon(QIcon(":/images/eyes.png"));
    }
}

void MainWindow::updateNoDataItem()
{
    // 清除表格之前的内容和跨行设置
    ui->tableWidget->clearContents();
    ui->tableWidget->clearSpans();

    // 关键改动：当没有数据时，直接将行数设置为 0
    // 我们的 StripedTableWidget 会自动处理空表时的背景绘制
    ui->tableWidget->setRowCount(0);

    // 当没有真实数据时，禁止选择
    // ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    // 根据是否有数据来显示或隐藏 NoData 标签
    // 现在判断条件就是行数是否为 0
    if (ui->tableWidget->rowCount() == 0)
    {
        ui->nolabel->show();
    }
    else
    {
        ui->nolabel->hide();
    }

    // 调整 NoData 标签的位置和大小
    adjustNoDataLabelPosition();
}

void MainWindow::adjustNoDataLabelPosition()
{
    // 获取水平表头的高度
    int headerHeight = ui->tableWidget->horizontalHeader()->height();

    // 获取表格的内部视口（不包括表头和滚动条）
    QWidget *viewport = ui->tableWidget->viewport();

    // 将 nolabel 的几何形状设置为与 viewport 完全重合
    // 它的坐标是相对于 tableWidget 的
    ui->nolabel->setGeometry(viewport->x(),       // 通常是 0 或边框宽度
                             viewport->y(),       // 通常等于表头高度
                             viewport->width(),   // 视口的宽度
                             viewport->height()); // 视口的高度

    // 确保标签在顶层（仅在子控件堆栈中）
    ui->nolabel->raise();
}

void MainWindow::loadData()
{
    // 1. 清空表格并将行数设置为0
    ui->tableWidget->clearSpans();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0); // <-- 先清零

    // 2. 在这里添加你的真实数据
    // 例如，添加3行新数据:
    ui->tableWidget->setRowCount(3); // 设置为你需要的行数

    // 填充第一行
    QTableWidgetItem *checkboxItem1 = new QTableWidgetItem();                  // 复选框列可以没有文本
    checkboxItem1->setFlags(checkboxItem1->flags() | Qt::ItemIsUserCheckable); // 使其可勾选
    checkboxItem1->setCheckState(Qt::Unchecked);                               // 设置初始状态
    ui->tableWidget->setItem(0, 0, checkboxItem1);                             // 设置到第0行，第0列 (复选框列)
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("001"));               // 原来的“编号”现在在第1列
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem("服务器"));            // 原来的“设备类型”现在在第2列
    // ... 填充其他列，列索引都需要加1

    // 填充第二行
    QTableWidgetItem *checkboxItem2 = new QTableWidgetItem();
    checkboxItem2->setFlags(checkboxItem2->flags() | Qt::ItemIsUserCheckable);
    checkboxItem2->setCheckState(Qt::Checked);
    ui->tableWidget->setItem(1, 0, checkboxItem2);
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("002"));
    ui->tableWidget->setItem(1, 2, new QTableWidgetItem("路由器"));
    // ... 填充其他列，列索引都需要加1

    // 填充第三行
    QTableWidgetItem *checkboxItem3 = new QTableWidgetItem();
    checkboxItem3->setFlags(checkboxItem3->flags() | Qt::ItemIsUserCheckable);
    checkboxItem3->setCheckState(Qt::Unchecked);
    ui->tableWidget->setItem(2, 0, checkboxItem3);
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("003"));
    ui->tableWidget->setItem(2, 2, new QTableWidgetItem("交换机"));
    // ... 填充其他列，列索引都需要加1

    // 启用选择模式
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // 隐藏 NoData 标签
    ui->nolabel->hide();

    // 加载数据后，更新表头复选框的状态（如果表格不为空）
    onItemCheckStateChanged(nullptr); // 传递nullptr来触发检查所有项
}

void MainWindow::onHeaderCheckStateChanged(Qt::CheckState state)
{
    // 在设置所有项的状态之前断开 itemChanged 信号，以避免递归调用 onItemCheckStateChanged
    ui->tableWidget->blockSignals(true);

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        QTableWidgetItem *item = ui->tableWidget->item(row, 0); // 获取第一列的复选框项
        if (item && (item->flags() & Qt::ItemIsUserCheckable))
        {
            item->setCheckState(state); // 设置所有行复选框为表头复选框的状态
        }
    }
    // 重新连接信号
    ui->tableWidget->blockSignals(false);
    // 强制更新一下 nolabel 的状态，以防万一
    updateNoDataItem();
}

void MainWindow::onItemCheckStateChanged(QTableWidgetItem *item)
{
    // 如果 item 为空，表示是 loadData() 调用来更新初始状态，或从其他地方触发检查所有项
    // 否则，只处理第0列的项变化
    if (item && item->column() != 0)
    {
        return; // 只关心第一列的复选框
    }

    int checkedCount = 0;
    int rowCount = ui->tableWidget->rowCount();

    if (rowCount == 0)
    {
        // 如果没有数据行，表头复选框应为未选中状态
        CheckableHeaderView *header = qobject_cast<CheckableHeaderView *>(ui->tableWidget->horizontalHeader());
        if (header)
        {
            header->setCheckState(Qt::Unchecked);
        }
        return;
    }

    for (int row = 0; row < rowCount; ++row)
    {
        QTableWidgetItem *current_item = ui->tableWidget->item(row, 0);
        if (current_item && (current_item->flags() & Qt::ItemIsUserCheckable) && current_item->checkState() == Qt::Checked)
        {
            checkedCount++;
        }
    }

    CheckableHeaderView *header = qobject_cast<CheckableHeaderView *>(ui->tableWidget->horizontalHeader());
    if (header)
    {
        if (checkedCount == rowCount)
        {
            header->setCheckState(Qt::Checked); // 所有都勾选
        }
        else if (checkedCount > 0)
        {
            header->setCheckState(Qt::PartiallyChecked); // 部分勾选
        }
        else
        {
            header->setCheckState(Qt::Unchecked); // 都没勾选
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // Adjust title bar width on resize
    ui->titleBar->setGeometry(0, 0, this->width(), ui->titleBar->height());
    // 调整 layoutWidget 的几何形状，使其宽度与 titleBar 的宽度同步
    // 假设 layoutWidget 的 x 坐标为 10，右侧留 10px 边距
    ui->layoutWidget->setGeometry(10, 5, ui->titleBar->width() - 20, ui->layoutWidget->height());

    int sideWidth = ui->widget_side->width();
    int btnWidth = ui->pushButton->width();
    int btnHeight = ui->pushButton->height();
    int winWidth = this->width();
    int winHeight = this->height();
    int btnY = winHeight / 3 - btnHeight / 2;
    int sideHeight = winHeight;
    int visibleWidth = 10;

    int sideX = m_bSideflag ? (winWidth - sideWidth) : (winWidth - visibleWidth);

    ui->widget_side->setGeometry(sideX, 40, sideWidth, sideHeight - 40); // Adjusted for title bar
    ui->pushButton->setGeometry(sideX - btnWidth, btnY, btnWidth, btnHeight);

    // --- DYNAMIC BUTTON POSITIONING ---
    int rightEdge = sideX;
    int topButtonsY = ui->titleBar->height() + 10;
    ui->lineEdit_14->move(rightEdge - ui->lineEdit_14->width() - 10, topButtonsY);
    ui->pushButton_3->move(ui->lineEdit_14->x() - ui->pushButton_3->width() - 10, topButtonsY);
    ui->pushButton_4->move(ui->pushButton_3->x() - ui->pushButton_4->width() - 10, topButtonsY);
    ui->pushButton_5->move(ui->pushButton_4->x() - ui->pushButton_5->width() - 10, topButtonsY);
    // --- END DYNAMIC POSITIONING ---

    int tableWidth = sideX - ui->tableWidget->x();
    int tableY = topButtonsY + ui->pushButton_3->height() + 10;
    int tableHeight = winHeight - tableY;
    ui->tableWidget->setGeometry(ui->tableWidget->x(),
                                 tableY,
                                 tableWidth,
                                 tableHeight);
    // 调整 NoData 标签位置
    adjustNoDataLabelPosition();

    // Set initial column widths only on the first resize event
    if (m_firstResize)
    {
        ui->tableWidget->horizontalHeader()->setStretchLastSection(false);
        for (int i = 0; i < ui->tableWidget->columnCount(); ++i)
        {
            ui->tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Interactive);
        }
        ui->tableWidget->setColumnWidth(0, 20);   // CHECK (Changed to 80 for testing)
        ui->tableWidget->setColumnWidth(1, 85);   // 编号
        ui->tableWidget->setColumnWidth(2, 160);  // 设备类型
        ui->tableWidget->setColumnWidth(3, 100);  // 激活状态
        ui->tableWidget->setColumnWidth(4, 120);  // IP地址
        ui->tableWidget->setColumnWidth(5, 80);   // 端口
        ui->tableWidget->setColumnWidth(6, 160);  // 服务管理型端口
        ui->tableWidget->setColumnWidth(7, 150);  // 软件版本
        ui->tableWidget->setColumnWidth(8, 110);  // IPv4网关
        ui->tableWidget->setColumnWidth(9, 80);   // HTTP端口
        ui->tableWidget->setColumnWidth(10, 200); // 设备序列号
        ui->tableWidget->setColumnWidth(11, 110); // 子网掩码
        ui->tableWidget->setColumnWidth(12, 120); // 物理地址
        ui->tableWidget->setColumnWidth(13, 120); // 编码通道数
        ui->tableWidget->setColumnWidth(14, 110); // DSP版本
        ui->tableWidget->setColumnWidth(15, 150); // 启动时间
        ui->tableWidget->setColumnWidth(16, 200); // IPv6地址
        ui->tableWidget->setColumnWidth(17, 200); // IPv6网关
        ui->tableWidget->setColumnWidth(18, 120); // IPv6子网前缀长度
        ui->tableWidget->setColumnWidth(19, 100); // 是否支持IPv6
        ui->tableWidget->setColumnWidth(20, 120); // 是否支持修改IPv6
        ui->tableWidget->setColumnWidth(21, 100); // 是否支持DHCP
        ui->tableWidget->setColumnWidth(22, 120); // IPv4 DHCP状态
        ui->tableWidget->setColumnWidth(23, 100); // 支持萤石云
        ui->tableWidget->setColumnWidth(24, 100); // 萤石云状态
        ui->tableWidget->setColumnWidth(25, 120); // 萤石云绑定状态
        ui->tableWidget->setColumnWidth(26, 150); // 萤石云版本
        m_firstResize = false;
    }
}

void MainWindow::on_pushButton_clicked()
{
    int sideWidth = ui->widget_side->width();
    int btnWidth = ui->pushButton->width();
    int btnHeight = ui->pushButton->height();
    int winWidth = this->width();
    int winHeight = this->height();
    int btnY = winHeight / 3 - btnHeight / 2;
    int sideHeight = ui->widget_side->height();
    int visibleWidth = 10;

    int startX = m_bSideflag ? (winWidth - sideWidth) : (winWidth - visibleWidth);
    int endX = m_bSideflag ? (winWidth - visibleWidth) : (winWidth - sideWidth);

    // 侧边栏动画
    m_propertyAnimation->setStartValue(QRect(startX, 40, sideWidth, sideHeight)); // Adjusted for title bar
    m_propertyAnimation->setEndValue(QRect(endX, 40, sideWidth, sideHeight));     // Adjusted for title bar
    m_propertyAnimation->start();

    // 按钮动画，始终贴在侧边栏左侧
    m_propertyAnimation2->setStartValue(QRect(startX - btnWidth, btnY, btnWidth, btnHeight));
    m_propertyAnimation2->setEndValue(QRect(endX - btnWidth, btnY, btnWidth, btnHeight));
    m_propertyAnimation2->start();

    // 表格宽度动画
    int startTableWidth = startX - ui->tableWidget->x();
    int endTableWidth = endX - ui->tableWidget->x();
    m_tableAnimation->setStartValue(QRect(ui->tableWidget->x(), ui->tableWidget->y(), startTableWidth, ui->tableWidget->height()));
    m_tableAnimation->setEndValue(QRect(ui->tableWidget->x(), ui->tableWidget->y(), endTableWidth, ui->tableWidget->height()));
    m_tableAnimation->start();

    // lineEdit_14 动画
    int lineEdit14StartX = ui->lineEdit_14->x();
    int lineEdit14EndX = lineEdit14StartX - (startX - endX);
    m_lineEdit14Animation->setStartValue(QRect(lineEdit14StartX, ui->lineEdit_14->y(), ui->lineEdit_14->width(), ui->lineEdit_14->height()));
    m_lineEdit14Animation->setEndValue(QRect(lineEdit14EndX, ui->lineEdit_14->y(), ui->lineEdit_14->width(), ui->lineEdit_14->height()));
    m_lineEdit14Animation->start();

    // pushButton_3 动画
    int pushButton3StartX = ui->pushButton_3->x();
    int pushButton3EndX = pushButton3StartX - (startX - endX);
    m_pushButton3Animation->setStartValue(QRect(pushButton3StartX, ui->pushButton_3->y(), ui->pushButton_3->width(), ui->pushButton_3->height()));
    m_pushButton3Animation->setEndValue(QRect(pushButton3EndX, ui->pushButton_3->y(), ui->pushButton_3->width(), ui->pushButton_3->height()));
    m_pushButton3Animation->start();

    // pushButton_4 动画
    int pushButton4StartX = ui->pushButton_4->x();
    int pushButton4EndX = pushButton4StartX - (startX - endX);
    m_pushButton4Animation->setStartValue(QRect(pushButton4StartX, ui->pushButton_4->y(), ui->pushButton_4->width(), ui->pushButton_4->height()));
    m_pushButton4Animation->setEndValue(QRect(pushButton4EndX, ui->pushButton_4->y(), ui->pushButton_4->width(), ui->pushButton_4->height()));
    m_pushButton4Animation->start();

    // pushButton_5 动画
    int pushButton5StartX = ui->pushButton_5->x();
    int pushButton5EndX = pushButton5StartX - (startX - endX);
    m_pushButton5Animation->setStartValue(QRect(pushButton5StartX, ui->pushButton_5->y(), ui->pushButton_5->width(), ui->pushButton_5->height()));
    m_pushButton5Animation->setEndValue(QRect(pushButton5EndX, ui->pushButton_5->y(), ui->pushButton_5->width(), ui->pushButton_5->height()));
    m_pushButton5Animation->start();

    // 连接动画完成信号来调整标签位置
    connect(m_tableAnimation, &QPropertyAnimation::finished, this, &MainWindow::adjustNoDataLabelPosition);

    ui->pushButton->setText(m_bSideflag ? "<" : ">");
    m_bSideflag = !m_bSideflag;
}

void MainWindow::on_pushButton_3_clicked()
{
    // 1. 保存原始样式
    const QString originalStyle = ui->pushButton_3->styleSheet();

    // 2. 设置为灰色背景
    ui->pushButton_3->setStyleSheet("background-color: #e7e5e5ff;");

    // 3. 设置一个2秒的定时器
    QTimer::singleShot(1200, this, [=]()
                       {
        // 4. 1.2秒后恢复原始样式
        ui->pushButton_3->setStyleSheet(originalStyle); });
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange)
    {
        // This is a placeholder, no specific action needed for now
    }
}

// --- Custom Title Bar Implementation ---
void MainWindow::onMinimizeClicked()
{
    showMinimized();
}

void MainWindow::onMaximizeClicked()
{
    if (m_isCustomMaximized)
    {
        // 如果当前是自定义最大化状态，则恢复到正常大小
        setGeometry(m_normalGeometry); // 使用保存的正常几何信息
        m_isCustomMaximized = false;
    }
    else
    {
        // 如果当前是正常状态，则进行自定义最大化
        m_normalGeometry = geometry(); // 在最大化前保存当前正常几何信息
        // 获取屏幕的可用几何区域（不包括任务栏等）
        QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        setGeometry(availableGeometry); // 将窗口设置为可用区域的大小
        m_isCustomMaximized = true;
    }
}

void MainWindow::onCloseClicked()
{
    close();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // Only start dragging if the left button is pressed over the title bar's empty space
    if (event->button() == Qt::LeftButton && ui->titleBar->geometry().contains(event->pos()))
    {
        // Check if the press was on any of the title bar's child widgets
        if (ui->titleIcon->geometry().contains(ui->titleBar->mapFrom(this, event->pos())) ||
            ui->titleText->geometry().contains(ui->titleBar->mapFrom(this, event->pos())) ||
            ui->minimizeButton->geometry().contains(ui->titleBar->mapFrom(this, event->pos())) ||
            ui->maximizeButton->geometry().contains(ui->titleBar->mapFrom(this, event->pos())) ||
            ui->closeButton->geometry().contains(ui->titleBar->mapFrom(this, event->pos())))
        {
            m_isDragging = false;
            return; // Don't drag if the click is on a button or label
        }

        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        m_isDragging = true;
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 只有在左键按下、正在拖动且窗口未处于自定义最大化状态时才移动窗口
    if (event->buttons() & Qt::LeftButton && m_isDragging && !m_isCustomMaximized)
    {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_isDragging = false;
    QMainWindow::mouseReleaseEvent(event);
}
// --- End of Custom Title Bar Implementation ---

void MainWindow::on_pushButton_6_clicked()
{
    // 创建 QMessageBox 实例
    QMessageBox msgBox(this);

    // 设置消息框的图标类型
    QPixmap pixmap(":/images/title_white.png"); // 从资源文件加载图标
    msgBox.setIconPixmap(pixmap);               // 设置自定义图标
    // 设置消息框的标题
    msgBox.setWindowTitle("在线客服");

    // 设置消息框的主要文本内容
    msgBox.setTextFormat(Qt::PlainText); // 显式设置为纯文本格式
    msgBox.setText("名称：设备网络搜索\n版本：v3.1.4.4build20250310");

    // 设置消息框的附加信息文本（包含链接）
    QString informativeText = "<a href='https://example.com/software_log'>查看软件日志</a><br>"
                              "<a href='https://example.com/open_source_license'>开源许可证</a><br>"
                              "<a href='https://example.com/user_manual'>用户手册</a>";
    msgBox.setInformativeText(informativeText);
    msgBox.setTextFormat(Qt::RichText); // 允许使用富文本格式，以便链接可以点击

    // 添加 Close 按钮并设为默认按钮
    QPushButton *closeButton = msgBox.addButton(QMessageBox::Close);
    msgBox.setDefaultButton(closeButton);

    // 显示消息框
    msgBox.exec();
}

void MainWindow::on_pushButton_7_clicked()
{
    qDebug() << "on_pushButton_7_clicked() entered.";

    // 禁用 pushButton_7，防止重复点击
    ui->pushButton_7->setEnabled(false);

    // 创建 QDialog 实例
    QDialog dialog(this);

    // 移除问号（帮助）按钮
    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // 设置对话框的布局
    QVBoxLayout *dialogLayout = new QVBoxLayout(&dialog);

    // 添加语言选择标签和下拉框
    QLabel *languageLabel = new QLabel("语言：", &dialog);
    QComboBox *languageComboBox = new QComboBox(&dialog);
    languageComboBox->addItems({"简体中文", "English"}); // 根据需要添加更多选项

    QHBoxLayout *languageRow = new QHBoxLayout();
    languageRow->addWidget(languageLabel);
    languageRow->addWidget(languageComboBox);
    dialogLayout->addLayout(languageRow);

    // 添加按钮区域
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    dialogLayout->addWidget(buttonBox);

    // 修改按钮文本
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);

    okButton->setText("确定");
    cancelButton->setText("取消");

    // 设置按钮固定宽度（直接设置具体数值，避免依赖原始宽度）
    okButton->setFixedWidth(80);
    cancelButton->setFixedWidth(80);

    // 设置按钮样式
    okButton->setStyleSheet("QPushButton {\n"
                            "    background-color: #FF4081;\n" // 红色背景
                            "    color: white;\n"              // 白色文本
                            "    border-radius: 5px;\n"
                            "    padding: 5px;\n"
                            "    border: none;\n"
                            "}\n"
                            "QPushButton:hover {\n"
                            "    background-color: #F50057;\n" // 鼠标悬停时颜色加深
                            "}\n"
                            "QPushButton:pressed {\n"
                            "    background-color: #C51162;\n" // 按下时颜色更深
                            "}");

    cancelButton->setStyleSheet("QPushButton {\n"
                                "    background-color: #E0E0E0;\n" // 灰色背景
                                "    color: black;\n"              // 黑色文本
                                "    border-radius: 5px;\n"
                                "    padding: 5px;\n"
                                "    border: none;\n"
                                "}\n"
                                "QPushButton:hover {\n"
                                "    background-color: #BDBDBD;\n" // 鼠标悬停时颜色加深
                                "}\n"
                                "QPushButton:pressed {\n"
                                "    background-color: #9E9E9E;\n" // 按下时颜色更深
                                "}");

    // 连接按钮信号
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // 设置对话框标题
    dialog.setWindowTitle("语言选择");

    // 设置对话框大小
    dialog.resize(300, 170);

    // 显示对话框并等待结果
    int result = dialog.exec();

    // 处理对话框结果（可选）
    if (result == QDialog::Accepted)
    {
        QString selectedLanguage = languageComboBox->currentText();
        // 根据 selectedLanguage 进行相应的处理
        qDebug() << "Selected Language:" << selectedLanguage;
    }
    qDebug() << "on_pushButton_7_clicked() exited.";

    // 重新启用 pushButton_7
    ui->pushButton_7->setEnabled(true);
}
void MainWindow::on_pushButton_8_clicked()
{
    // 使用 Qt 的桌面服务打开默认浏览器并跳转到百度
    QDesktopServices::openUrl(QUrl("https://www.baidu.com"));
}
