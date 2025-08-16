# DeviceNet 监控软件

这是一个使用 C++ 和 Qt 6 开发的DeviceNet监控软件桌面应用程序。

## ✨ 功能

*   自定义窗口标题栏，支持拖动、最小化、最大化和关闭。
*   可伸缩的侧边栏菜单，带有平滑的动画效果。
*   带有斑马条纹和复选框的表格视图，用于清晰地显示数据。
*   表格标题栏支持全选/取消全选。
*   行内密码可见性切换。
*   动态搜索功能。

## 🛠️ 构建项目

你需要一个支持 C++11 的编译器和 Qt 6 开发环境。

1.  **克隆仓库**
    ```bash
    git clone <your-repository-url>
    cd DeviceNet
    ```

2.  **使用 qmake 生成 Makefile**
    ```bash
    qmake DeviceNet.pro
    ```

3.  **编译项目**
    *   在 Windows 上 (使用 MinGW):
        ```bash
        mingw32-make
        ```
    *   在 macOS 或 Linux 上:
        ```bash
        make
        ```

4.  **运行程序**
    编译完成后，可执行文件将位于 `debug/` 或 `release/` 目录下。

## 📄 许可证

该项目当前没有许可证。请添加一个 `LICENSE` 文件来定义其他人如何使用、修改和分发你的代码。
