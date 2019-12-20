#include <QMouseEvent>

#include "canvas.h"
#include "impl.h"
#include "vertex.h"

Canvas::Canvas(QWidget *parent, Impl* impl)
    : QLabel(parent),
      impl(impl)
{
    setStyleSheet("QLabel { background-color : white; }");
}

void
Canvas::draw(Vertex* v, bool draw_edge)
{
    if(draw_edge){
        (v->styleSheet().contains("border-color : yellow"))?
            v->changeColor(tr("black")):
            v->changeColor(tr("yellow"));
        if(vertex == nullptr) {vertex = v;return;}
        if(vertex == v) {vertex = nullptr;return;}
        impl->addEdge(vertex, v, this);
        vertex = nullptr;
    }
    else
        impl->addEdge(v, this);
}

bool
Canvas::eventFilter(QObject *, QEvent *event){
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* e{static_cast<QMouseEvent*>(event)};
        if(e->button()==Qt::LeftButton)
        {
            impl->addVertex(e->x(),e->y(),this);
            return true;
        }
    }
    return false;
}

