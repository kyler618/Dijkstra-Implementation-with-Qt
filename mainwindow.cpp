#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include "impl.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      impl(new Impl(ui)),
      canvas(new Canvas(this, impl))
{
    ui->setupUi(this);
    ui->grid->addWidget(canvas,0,0,1,6);
}

MainWindow::~MainWindow()
{
    qDeleteAll(canvas->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly));
    delete ui;
}

void MainWindow::on_start_clicked()
{
    impl->trigger_start(canvas);
}

void MainWindow::on_result_clicked()
{
    impl->trigger_result();
}

void MainWindow::on_clear_clicked()
{
    impl->clear();
}

void MainWindow::on_draw_vertex_clicked()
{
    ui->draw_edge->setChecked(false);
    impl->trigger_drawEdge(false);
    ui->draw_vertex->setChecked(ui->draw_vertex->isChecked());
    impl->trigger_drawVertex(canvas, ui->draw_vertex->isChecked());
}

void MainWindow::on_draw_edge_clicked()
{
    ui->draw_vertex->setChecked(false);
    impl->trigger_drawVertex(canvas, false);
    ui->draw_edge->setChecked(ui->draw_edge->isChecked());
    impl->trigger_drawEdge(ui->draw_edge->isChecked());
}

void MainWindow::on_rand_generate_clicked()
{
    impl->rand_gen_vertex(canvas);
}
