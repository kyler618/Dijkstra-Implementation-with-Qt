#include "formdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QDialogButtonBox>
#include <QIntValidator>
#include <QFormLayout>
#include <sstream>

FormDialog::FormDialog(QWidget *parent) : QDialog(parent)
{
    QFormLayout *form{new QFormLayout(this)};
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);

    vertex_No = new QLineEdit(this);
    vertex_No->setText("1");
    vertex_No->setPlaceholderText("1");
    // After testing, k80 is the largest acceptable range without out of memory
    vertex_No->setValidator( new QIntValidator(1, 80, this) );
    connect(vertex_No, &QLineEdit::textChanged, this, [this](){
        if( 80 < vertex_No->text().toInt() )
            vertex_No->setText(QString::number(80));
    });
    form->addRow(new QLabel(tr("Number of Vertex :   "), this), vertex_No);

    label = new QLabel(tr("Density of Edge : 0.5"), this);
    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(0);
    slider->setMaximum(10);
    slider->setValue(5);
    connect(slider, &QSlider::valueChanged, this, [this](){
        std::string s = "Density of Edge : ";
        std::ostringstream  double_string;
        double_string.precision(1);
        double_string << std::fixed << slider->value()/10.0;
        s.append(double_string.str());
        label->setText(QString::fromStdString(s));
    });
    form->addRow(label, slider);

    max_weight = new QLineEdit(this);
    max_weight->setText("0");
    max_weight->setPlaceholderText("0");
    max_weight->setValidator( new QIntValidator(0, 2147483647, this) );
    form->addRow(new QLabel(tr("Maximum Weight :    "), this), max_weight);

    QDialogButtonBox *buttonBox {
        new QDialogButtonBox
        ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this )
    };

    form->addWidget(buttonBox);

    bool conn {
        connect(buttonBox, &QDialogButtonBox::accepted,
        this, &FormDialog::accept)
    };
    Q_ASSERT(conn);
    conn = connect(buttonBox, &QDialogButtonBox::rejected,
                   this, &FormDialog::reject);
    Q_ASSERT(conn);

    setLayout(form);
}

std::tuple<int, int, int>
FormDialog::get_coefficient(QWidget *parent, bool *ok)
{
    FormDialog *dialog{new FormDialog(parent)};

    const int ret{dialog->exec()};
    if (ok)
        *ok = !!ret;

    std::tuple<int, int, int> result{-1,-1,-1};
    if(ret)
        result = std::make_tuple(
                    (dialog->vertex_No->text().isEmpty())?
                        1:dialog->vertex_No->text().toInt(),
                    dialog->slider->value(),
                    (dialog->max_weight->text().isEmpty())?
                        0:dialog->max_weight->text().toInt());

    dialog->deleteLater();
    return result;
}
