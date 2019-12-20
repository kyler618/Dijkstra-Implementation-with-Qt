#ifndef FORMDIALOG_H
#define FORMDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QSlider;

class FormDialog : public QDialog
{

    Q_OBJECT

public:
    explicit FormDialog(QWidget *parent = nullptr);
    static std::tuple<int, int, int> get_coefficient(QWidget *parent, bool *ok = nullptr);

private:
    QLabel* label;
    QLineEdit* vertex_No, * max_weight;
    QSlider* slider;
};

#endif // FORMDIALOG_H
