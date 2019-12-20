#ifndef IMPL_H
#define IMPL_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QProgressDialog>
#include <map>

#include "ui_mainwindow.h"

class Vertex;
class Edge;

class Impl : public QObject
{
    Q_OBJECT

signals:
    void executing(bool, Impl*);
    void drawing_edge(bool);
    void receive_all();

public:
    Impl(Ui::MainWindow *ui);
    void addVertex(int,int, QWidget*);
    void addEdge(Vertex*, QWidget*);
    void addEdge(Vertex*, Vertex*, QWidget*);
    void trigger_start(QWidget*);
    void clear();
    void trigger_result();
    void trigger_drawVertex(QWidget*, bool);
    void trigger_drawEdge(bool);
    void rand_gen_vertex(QWidget*);

public slots:
    void execution(Vertex*, bool);
    void show_path(Vertex*, bool);

private:
    Ui::MainWindow *ui;
    std::map<Vertex*, Edge*> vertexes;
    std::pair<int,int> coefficient;
    QEventLoop stop;
    QTimer delay;
    QProgressDialog* progress;
};

#endif // IMPL_H
