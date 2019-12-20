#include "impl.h"
#include "mainwindow.h"
#include "vertex.h"
#include "edge.h"
#include "set.h"
#include "priority_queue.h"
#include "formdialog.h"

#include <forward_list>
#include <QInputDialog>
#include <QMessageBox>

Impl::Impl(Ui::MainWindow *ui)
{
    this->ui = ui;
}

void
Impl::addVertex(int x, int y, QWidget* canvas){
    Vertex* vertex{new Vertex{x,y, canvas}};
    connect(this, SIGNAL(executing(bool, Impl*)), vertex, SLOT(filter_trigger(bool, Impl*)));
    connect(this, SIGNAL(drawing_edge(bool)), vertex, SLOT(drawingMode_trigger(bool)));
    connect(this, SIGNAL(receive_all()), vertex, SLOT(send_self()));
}

void
Impl::addEdge(Vertex* vertex, QWidget* canvas){
    if(!coefficient.first) return;
    for(std::pair<Vertex*, Edge*> pair : vertexes){
        if(vertex->contains(pair.first)) continue;
        Edge* edge{new Edge{vertex, pair.first, rand() % (coefficient.second), canvas}};
        vertex->add_to_list(edge);
        pair.first->add_to_list(edge);
        connect(this, SIGNAL(executing(bool, Impl*)), edge, SLOT(filter_trigger(bool, Impl*)));
        coefficient.first -= 1;
        progress->setValue(progress->value()+1);
        if(!coefficient.first)
        {
            progress->close();
            delay.start(1);
            stop.exec();
            disconnect(&delay, SIGNAL(timeout()), &stop, SLOT(quit()));

            ui->draw_vertex->setEnabled(true);
            ui->draw_edge->setEnabled(true);
            ui->rand_generate->setEnabled(true);
            ui->show_step->setEnabled(true);
            ui->start->setEnabled(true);
        }

    }
    vertexes[vertex] = nullptr;
}

void
Impl::addEdge(Vertex* vertex_A, Vertex* vertex_B, QWidget* canvas){
    vertex_B->changeColor("black");

    // check edge whether exist
    if(vertex_A->contains(vertex_B))
    {
        QMessageBox error{
            QMessageBox::Critical,
            tr("Error"),
            tr("Their edge already exist."),
            QMessageBox::Ok,
            canvas->parentWidget(),
        };
        error.exec();
    }
    else
    {
        bool ok{false};
        int weight{QInputDialog::getInt(
                        canvas->parentWidget(),
                        "Input Weight",
                        "Input Weight",
                        0, 0, 2147483647, 1, &ok,
                        Qt::MSWindowsFixedSizeDialogHint
                     )};

        if (ok) {
            Edge* edge{new Edge{vertex_A, vertex_B, weight, canvas}};
            vertex_A->add_to_list(edge);
            vertex_B->add_to_list(edge);

            connect(this, SIGNAL(executing(bool, Impl*)), edge, SLOT(filter_trigger(bool, Impl*)));
        }
    }

    vertex_A->changeColor(tr("black"));
}

void
Impl::trigger_start(QWidget* canvas){

    if(!ui->start->isChecked()){
        ui->start->setChecked(false);
        clear();
        return;
    }

    trigger_drawVertex(canvas, false);
    trigger_drawEdge(false);
    vertexes.clear();
    emit executing(true, this);

    // control buttons enablility
    ui->draw_vertex->setEnabled(false);
    ui->draw_edge->setEnabled(false);
    ui->rand_generate->setEnabled(false);
    ui->show_step->setEnabled(false);

    ui->draw_vertex->setChecked(false);
    ui->draw_edge->setChecked(false);
    ui->start->setChecked(ui->start->isChecked());
}

void
Impl::clear(){
    if(ui->result->isChecked()){
        ui->result->setChecked(false);
        for(std::pair<Vertex*, Edge*> pair : vertexes){
            disconnect(pair.first, SIGNAL(selected(Vertex*, bool)), this, SLOT(show_path(Vertex*, bool)));
            pair.first->removeEventFilter(pair.first);
            if(pair.second!=nullptr)
                pair.second->changeColor(Qt::black);
        }
    }

    if(ui->show_step->isChecked())
    {
        if(stop.isRunning())
            stop.exit(1);
        disconnect(ui->nextStep, SIGNAL(released()), &stop, SLOT(quit()));
    }
    else
        disconnect(&delay, SIGNAL(timeout()), &stop, SLOT(quit()));

    vertexes.clear();
    emit executing(false, this);

    // control buttons enablility
    ui->draw_vertex->setEnabled(true);
    ui->draw_edge->setEnabled(true);
    ui->rand_generate->setEnabled(true);
    ui->show_step->setEnabled(true);
    ui->start->setEnabled(true);
    ui->clear->setEnabled(false);
    ui->nextStep->setEnabled(false);
    ui->result->setEnabled(false);
}

void
Impl::trigger_result(){
    if(!ui->result->isChecked()){
        ui->result->setChecked(false);
        for(std::pair<Vertex*, Edge*> pair : vertexes){
            disconnect(pair.first, SIGNAL(selected(Vertex*, bool)), this, SLOT(show_path(Vertex*, bool)));
            pair.first->removeEventFilter(pair.first);
            if(pair.second!=nullptr)
                pair.second->changeColor(Qt::black);
        }
        return;
    }
    ui->result->setChecked(true);
    for(std::pair<Vertex*, Edge*> pair : vertexes){
        connect(pair.first, SIGNAL(selected(Vertex*, bool)), this, SLOT(show_path(Vertex*, bool)));
        pair.first->installEventFilter(pair.first);
    }
}

void
Impl::trigger_drawVertex(QWidget* canvas, bool checked){
    (checked)?
        canvas->installEventFilter(canvas):
        canvas->removeEventFilter(canvas);
}

void
Impl::trigger_drawEdge(bool checked){
    emit drawing_edge(checked);
}

void
Impl::rand_gen_vertex(QWidget* canvas){

    connect(&delay, SIGNAL(timeout()), &stop, SLOT(quit()));

    vertexes.clear();

    std::map<std::pair<int,int>, bool> record;

    std::tuple<int, int, int> input{FormDialog::get_coefficient(canvas->parentWidget())};
    if (std::get<0>(input) == -1) return;

    ui->draw_vertex->setEnabled(false);
    ui->draw_edge->setEnabled(false);
    ui->rand_generate->setEnabled(false);
    ui->show_step->setEnabled(false);
    ui->start->setEnabled(false);

    coefficient.first = std::get<0>(input) * (std::get<0>(input)-1)/2 * std::get<1>(input) / 10 ;
    coefficient.second = std::get<2>(input) + 1;

    progress = new QProgressDialog("Generating...", nullptr, 0, coefficient.first);
    progress->setAttribute(Qt::WA_DeleteOnClose);
    progress->setVisible(true);

    delay.start(1);
    stop.exec();

    for(int n{0}; n < std::get<0>(input); ++n){
        int x,y;
        do{
            x = rand()%(canvas->size().width()-60) + 40;
            y = rand()%(canvas->size().height()-60) + 40;

            if(record[std::pair<int,int>(x,y)]) continue;

            for(int i{x-40}; i<=x+40; ++i)
                for(int j{y-40}; j<=y+40; ++j)
                    record[std::pair<int,int>(i,j)] = true;
            break;

        }while(true);
        addVertex(x,y,canvas);
    }

    emit receive_all();
}

void
Impl::execution(Vertex* vertex, bool selected){
    if(!selected){
        vertexes[vertex] = nullptr; return;
    }

    bool showStep{ui->show_step->isChecked()};

    if(showStep){
        ui->nextStep->setEnabled(true);
        connect(ui->nextStep, SIGNAL(released()), &stop, SLOT(quit()));
    }
    else{
        ui->nextStep->setEnabled(false);
        connect(&delay, SIGNAL(timeout()), &stop, SLOT(quit()));
    }

    ui->start->setChecked(false);
    ui->start->setEnabled(false);
    ui->clear->setEnabled(true);

    vertex->setFont(QFont("Agency FB", 20));
    vertex->setNum(0);
    Set set{static_cast<unsigned int>(vertexes.size())};

    for(std::pair<Vertex*, Edge*> pair : vertexes){
        disconnect(pair.first, SIGNAL(selected(Vertex*, bool)), this, SLOT(execution(Vertex*, bool)));
        pair.first->removeEventFilter(pair.first);
        set.insert(pair.first);
    }

    Priority_Queue priority_queue;
    priority_queue.insert(vertex, 0);

    Vertex* current;

    while(true)
    {
       current = priority_queue.retrieve();
       if(current==nullptr) break;
       if(!set.exist(current, true)) continue;
       current->changeColor("blue");
       if(!showStep) delay.start(250);
       if(stop.isRunning()) return;
       else if(stop.exec()) return;
       std::forward_list<Edge*>* edgeList{current->getList()};
       for(std::forward_list<Edge*>::iterator edge{edgeList->begin()};
           edge != edgeList->end(); ++edge)
       {
           (*edge)->changeColor(Qt::blue);
           Vertex* neighbor_vertex {(*edge)->get_adjacentVertex(current)};
           if(!set.exist(neighbor_vertex)) {
               (*edge)->changeColor(Qt::black);
               continue;
           }
           int distance{(*edge)->getWeight() + current->text().toInt()};
           if( neighbor_vertex->text().isEmpty() ||
               distance < neighbor_vertex->text().toInt() )
           {
               neighbor_vertex->setNum(distance);
               priority_queue.insert(neighbor_vertex, distance);
               int fontSize{20};
               while(distance/10 != 0){
                   fontSize -= 2;
                   distance /= 10;
                   if(fontSize<=0) {
                       fontSize = 1;
                       break;
                   }
               }
               neighbor_vertex->setFont(QFont("Agency FB", fontSize));
               vertexes[neighbor_vertex] = *edge;
               neighbor_vertex->changeColor("rgb(100, 200, 100)");
           }
           else if ( !neighbor_vertex->text().isEmpty() )
               neighbor_vertex->changeColor("red");

           if(!showStep) delay.start(250);
           if(stop.isRunning()) return;
           else if(stop.exec()) return;

           neighbor_vertex->changeColor("cyan");
           (*edge)->changeColor(Qt::black);
       }
       current->changeColor("gray");
    }

    QMessageBox finish{
        QMessageBox::NoIcon,
        tr("Finished"),
        tr("The algorithm is finished."),
        QMessageBox::Ok
    };
    finish.exec();

    ui->result->setEnabled(true);

    if(showStep){
        ui->nextStep->setEnabled(false);
        disconnect(ui->nextStep, SIGNAL(released()), &stop, SLOT(quit()));
    }

}

void
Impl::show_path(Vertex* vertex, bool){
    for(std::pair<Vertex*, Edge*> pair : vertexes)
        if(pair.second!=nullptr)
            pair.second->changeColor(Qt::black);
    while(vertexes[vertex]!=nullptr){
        vertexes[vertex]->changeColor(Qt::blue);
        vertex = vertexes[vertex]->get_adjacentVertex(vertex);
    }
}

