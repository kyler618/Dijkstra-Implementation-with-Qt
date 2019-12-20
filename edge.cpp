#include "edge.h"

#include <QBitmap>
#include <QPainter>
#include <cmath>

#include <vertex.h>

Edge::Edge(Vertex* vertex_A, Vertex* vertex_B, int weight, QWidget* parent)
    :QLabel(parent),
     vertex_A(vertex_A),
     vertex_B(vertex_B),
     weight(weight)
{
    setStyleSheet("background-color : none");
    setVisible(true);
    installEventFilter(this);
    lower();
}

Vertex*
Edge::get_adjacentVertex(Vertex* vertex)
{
    return (vertex==vertex_B)?vertex_A:vertex_B;
}

void
Edge::filter_trigger(bool executing, Impl*){
    this->executing = executing;
}

bool
Edge::contains(Vertex* vertex){
    return ((vertex_A==vertex)^(vertex_B==vertex));
}

int
Edge::getWeight(){
    return weight;
}

void
Edge::changeColor(QColor color){
    if(new_color!=color){
        new_color = color;
        update();
    }
}

void
Edge::remove(Vertex* vertex){
    if(vertex==nullptr){
        // vertexes removes each other
        vertex_A->remove_from_list(this);
        vertex_B->remove_from_list(this);
    }
    else
        vertex->remove_from_list(this);
    removeEventFilter(this);
    deleteLater();
}

double
Edge::getAngle(QPoint* center_A, QPoint* center_B)
{
    center_A->setX(vertex_A->x()+20);
    center_A->setY(vertex_A->y()+20);
    center_B->setX(vertex_B->x()+20);
    center_B->setY(vertex_B->y()+20);

    int offset{10};
    {
        // calculate width length
        int length{weight};
        while(length/10 != 0){
            offset += 10;
            length /= 10;
        }
    }

    // set topleft coordinate
    move(std::max(0, std::min(center_A->x(), center_B->x()) - offset ),
         std::max(0, std::min(center_A->y(), center_B->y())) - 20 );

    // set size
    int width{center_A->x() - center_B->x()};
    int height{center_A->y() - center_B->y()};

    if(width)
        resize(abs(width) + 2*offset, abs(height) + 40);
    else
        return 90.0 * M_PI / 180.0;

    return atan((static_cast<double>(height)/static_cast<double>(width)));
}

bool
Edge::eventFilter(QObject *, QEvent *event){
    switch(event->type()){
        default:
            break;
        case QEvent::MouseButtonPress:
            if(!executing &&
               static_cast<QMouseEvent*>(event)->button()== Qt::RightButton){
               remove();
            }
            break;
        case QEvent::Paint:
            /*
             * either vertex A or B moves, repaint line again.
             * when edge is just initiallized, mask is not setted. This toogles paint event.
            */
            if( ! ( vertex_A->isHover ^ vertex_B->isHover )
                    && !mask().isEmpty()
                    && color==new_color ) {
                QLabel::paintEvent(static_cast<QPaintEvent*>(event));
                break;
            }

            clearMask();
            QPoint center_A, center_B, textPoint;
            // calculate the angle
            double angle{getAngle(&center_A, &center_B)};

            QPointF offset{20.0 * cos(angle), 20.0 * sin(angle)};
            if( center_A.x() != center_B.x() ){
                if( center_A.x() < center_B.x() )
                    // vertex A is left hand side
                    offset *= -1;
            }
            else if ( center_A.y() < center_B.y() )
                // two vertexes on the same vertical line
                offset *= -1;

            // Draw
            QPixmap image(size());
            image.fill(Qt::transparent);
            QPainter painter(&image);
            QPen pen(new_color);
            color = new_color;

            // Draw line
            pen.setCapStyle(Qt::RoundCap);
            pen.setWidth(3);

            QLineF line(center_A-offset-geometry().topLeft(), center_B+offset-geometry().topLeft());
            painter.setPen(pen);
            painter.drawLine(line);

            // Draw Text
            pen.setColor(Qt::black);
            painter.setPen(pen);
            textPoint = (center_A+center_B)/2 - geometry().topLeft();

            int width{10};
            {
                // calculate width length
                int length{weight};
                while(length/10 != 0){
                    width += 10;
                    length /= 10;
                }
                // y = -x
                if(angle>0) width *= -1;
            }

            QRect textRect{textPoint-QPoint((width>0)?width:0,20), QSize(abs(width),20)};
            painter.setFont(QFont("Arial", 12, QFont::Thin));
            painter.drawText(textRect, Qt::AlignCenter, QString::number(weight));

            setPixmap(image);
            setMask(image.mask());
            QLabel::paintEvent(static_cast<QPaintEvent*>(event));

            break;

    }

    return true;

}
