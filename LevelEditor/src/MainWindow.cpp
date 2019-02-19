#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ObjectsList.h"

#include <QDockWidget>

namespace
{

template <class WidgetType>
QDockWidget* createDockWidget(QWidget* parent)
{
    auto docWidget = new QDockWidget(parent);
    auto widget = new WidgetType();
    docWidget->setWidget(widget);

    return docWidget;
}

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->addDockWidget(Qt::RightDockWidgetArea, createDockWidget<ObjectsList>(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
