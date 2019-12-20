#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>

class Impl;
class Vertex;

class Canvas : public QLabel
{

    Q_OBJECT

public:
    Canvas(QWidget*, Impl*);

public slots:
    void draw(Vertex*, bool);

protected slots:
    bool eventFilter(QObject*, QEvent*) override;

private:
    Impl* impl;
    Vertex* vertex{nullptr};
};

#endif // CANVAS_H
