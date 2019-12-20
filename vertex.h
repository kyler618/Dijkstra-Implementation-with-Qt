#ifndef VERTEX_H
#define VERTEX_H

#include <QLabel>
#include <QList>
#include <forward_list>

class Impl;
class Edge;

class Vertex : public QLabel
{

    Q_OBJECT

signals:
    void selected(Vertex*, bool=false);

public:
    bool isHover{false};
    Vertex(int, int, QWidget*);
    void changeColor(const QString&);
    void add_to_list(Edge*);
    void remove_from_list(Edge*);
    bool contains(Vertex*);
    std::forward_list <Edge*>* getList();

public slots:
    void drawingMode_trigger(bool);
    void filter_trigger(bool, Impl*);
    void send_self();

protected slots:
    bool eventFilter(QObject*, QEvent*) override;

private:
    QPoint lastPoint;
    std::forward_list <Edge*> adjacency_list;
    bool send{false};

};

#endif // VERTEX_H
