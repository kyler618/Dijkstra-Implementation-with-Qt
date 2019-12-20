#ifndef EDGE_H
#define EDGE_H

#include <QLabel>

class Impl;
class Vertex;

class Edge : public QLabel
{

    Q_OBJECT

public:
    Edge(Vertex*, Vertex*, int, QWidget*);
    Vertex* get_adjacentVertex(Vertex*);
    bool contains(Vertex*);
    int getWeight();
    void changeColor(QColor);
    void remove(Vertex* = nullptr);

public slots:
    void filter_trigger(bool, Impl*);

protected slots:
    bool eventFilter(QObject*, QEvent*) override;
private:
    Vertex* vertex_A, * vertex_B;
    int weight;
    QColor color{Qt::black}, new_color{Qt::black};
    bool executing{false};
    double getAngle(QPoint*, QPoint*);
};

#endif // EDGE_H
