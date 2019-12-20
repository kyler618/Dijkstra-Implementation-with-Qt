#include <QMouseEvent>
#include <QEvent>
#include <QtDebug>

#include "vertex.h"
#include "impl.h"
#include "edge.h"

Vertex::Vertex(int x, int y, QWidget *parent)
    : QLabel(parent)
{
    setVisible(true);
    setAlignment(Qt::AlignCenter);
    setGeometry(x-20, y-20, 40, 40);
    setStyleSheet("border : 2px solid;"
                  "border-radius : 20px;"
                  "border-color : black;"
                  );

    installEventFilter(this);
    connect(this, SIGNAL(selected(Vertex*,bool)), parent, SLOT(draw(Vertex*,bool)));

}

void
Vertex::changeColor(const QString& color){
    QString style{"border : 2px solid;"
                  "border-radius : 20px;"
                  "border-color : "};
    setStyleSheet(style.append(color));
}

void
Vertex::add_to_list(Edge* edge){
    adjacency_list.push_front(edge);
}

void
Vertex::remove_from_list(Edge* edge){
    adjacency_list.remove(edge);
}

bool
Vertex::contains(Vertex* vertex){
    for(Edge* edge : adjacency_list)
        if(edge->contains(vertex)) return true;
    return false;
}

std::forward_list <Edge*>*
Vertex::getList(){
    return &adjacency_list;
}

void
Vertex::drawingMode_trigger(bool drawing){
    send = drawing;
    if( !drawing && styleSheet().contains("border-color : yellow") )
        emit selected(this,true);
}

void
Vertex::filter_trigger(bool executing, Impl* impl){
    send = executing;
    if(executing)
    {
        disconnect(this, SIGNAL(selected(Vertex*,bool)), this->parentWidget(), SLOT(draw(Vertex*,bool)));
        connect(this, SIGNAL(selected(Vertex*, bool)), impl, SLOT(execution(Vertex*, bool)));
        emit selected(this);
    }
    else
    {
        setText("");
        if(!styleSheet().contains("border-color : black"))
            changeColor("black");
        for(Edge* edge : adjacency_list)
            edge->changeColor("black");
        installEventFilter(this);
        connect(this, SIGNAL(selected(Vertex*,bool)), this->parentWidget(), SLOT(draw(Vertex*,bool)));
    }
}

void
Vertex::send_self(){
    emit selected(this);
}

bool
Vertex::eventFilter(QObject *, QEvent *event){
    switch(event->type()) {
        case QEvent::Paint:
            QLabel::paintEvent(static_cast<QPaintEvent*>(event));
            return true;
        case QEvent::MouseButtonRelease:
            if(isHover)
                isHover = !isHover;
            break;
        case QEvent::MouseButtonPress:
            switch(static_cast<QMouseEvent*>(event)->button()){
                case Qt::LeftButton:
                    lastPoint = static_cast<QMouseEvent*>(event)->pos();
                    if(send)
                        emit selected(this, true);
                    else
                        isHover = true;
                    break;
                case Qt::RightButton:
                    if(send) break;
                    if(styleSheet().contains("border-color : yellow")) emit selected(this);
                    // delete all adjacent edge first
                    for(Edge* edge : adjacency_list)
                        edge->remove(edge->get_adjacentVertex(this));
                    adjacency_list.clear();
                    disconnect();
                    removeEventFilter(this);
                    deleteLater();
                    break;
                default: break;
            }
            break;
        case QEvent::MouseMove:
            if(isHover){
                int new_x{x() + static_cast<QMouseEvent*>(event)->pos().x() - lastPoint.x()};
                int new_y{y() + static_cast<QMouseEvent*>(event)->pos().y() - lastPoint.y()};
                if(!(new_x < 0 || new_y < 0 ||
                     parentWidget()->size().width()-40 < new_x ||
                     parentWidget()->size().height()-40 < new_y ))
                move(new_x, new_y);
                repaint();
            }
            break;
        default:
            break;
    }

    return true;

}
