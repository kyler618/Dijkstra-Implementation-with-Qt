#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Impl;
class Canvas;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_start_clicked();
    void on_result_clicked();
    void on_clear_clicked();
    void on_draw_vertex_clicked();
    void on_draw_edge_clicked();
    void on_rand_generate_clicked();

private:
    Ui::MainWindow* ui;
    Impl* impl;
    Canvas* canvas;
};
#endif // MAINWINDOW_H
