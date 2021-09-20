#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include "ui_mainwindow.h"
#include "imagewidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
protected:
    void closeEvent(QCloseEvent *e);
    void paintEvent(QPaintEvent *paintevent);

private slots:

private:
    void CreateActions();
    void CreateMenus();
    void CreateToolBars();
    void CreateStatusBar();

private:

    QMenu		*menu_file_;
    QMenu		*menu_edit_;
    QMenu		*menu_help_;
    QToolBar	*toolbar_file_;
    QToolBar	*toolbar_edit_;
    QAction		*action_new_;
    QAction		*action_open_;
    QAction		*action_save_;
    QAction		*action_saveas_;
    QAction		*action_invert_;
    QAction		*action_mirror_;
    QAction		*action_gray_;

    QAction     *action_choose_idw;
    QAction     *action_choose_rbf;
    QAction     *action_warp;
    QAction		*action_restore_;

    ImageWidget		*imagewidget_;
};

#endif // MAINWINDOW_H
