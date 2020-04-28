#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IPv4address.h"
#include "IPv4parser.h"
#include "SubnetsCalculatorV4.h"
#include "IPstructs.h"
#include <QSpinBox>
#include <QLabel>

using namespace core;


namespace widgetApp {

    IPv4address address;
    IPv4parser parser;
    SubnetsCalculatorV4 calculatorIPv4;
    QList<Subnetv4*> subnets;

    QWidget *addressWidget;
    QWidget *maskWidget;
    QWidget *binaryAddressWidget;
    QWidget *binaryMaskWidget;

    QSpinBox *subnetCountBox;
    QWidget *subnetScrollContent;
    QWidget *subnetGraphContent;

    QFrame *subnetsGraphFrame;

    QVBoxLayout *subnetsPanelLayout = new QVBoxLayout();
    QHBoxLayout *graphPanelLayout = new QHBoxLayout();

    bool isHorizontal = false;


    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        addressWidget = ui->Address4Widget;

        subnetScrollContent = ui->subnetScroll;
        subnetScrollContent->setLayout(subnetsPanelLayout);

        subnetGraphContent = ui->graphScroll;

        subnetCountBox = ui->hostNumberSpinBox;

        binaryAddressWidget = ui->BinaryAddress4Widget;

        maskWidget = ui->Mask4Widget;

        binaryMaskWidget = ui->BinaryMask4Widget;
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }
}

void widgetApp::MainWindow::on_CalculateButton_clicked()
{
    calculateBitsetFromInput(addressWidget, binaryAddressWidget);
    calculateBitsetFromInput(maskWidget, binaryMaskWidget);
}

void widgetApp::MainWindow::on_drawButton_clicked()
{
    drawNetworkGraph(isHorizontal);
}
void widgetApp::MainWindow::on_hostNumberSpinBox_valueChanged(int subnetCount)
{
    subnets = QList<Subnetv4*>();

    QLayoutItem *child;
    while ((child = subnetScrollContent->layout()->takeAt(0)) != NULL)
    {
        if(child->widget() != NULL)
        {
            delete child->widget();
            delete child;
        }
    }

    for (int i = 0; i < subnetCount; i++)
    {
        QHBoxLayout *textLayout = new QHBoxLayout();

        QString labelText = QString::fromStdString("Subnet " + std::to_string(1 + i));
        QLabel *subnetLabel = new QLabel(labelText);
        subnetLabel->setLayout(textLayout);
        subnetLabel->font().bold();

        subnetsPanelLayout->addWidget(subnetLabel);

        QHBoxLayout *frameLayout = new QHBoxLayout();
        QFrame *frame = new QFrame();

        frame->setMinimumHeight(30);
        frame->setLayout(frameLayout);

        QLabel *label = new QLabel("Hosts number: ");
        label->setMinimumHeight(20);
        QSpinBox *spinBox = new QSpinBox();
        spinBox->setStyleSheet("background-color: rgb(180, 180, 180);\nncolobackground-color: rgb(180, 180, 180);"
                                "\ncolor: rgb(240, 221, 168);r: rgb(220, 221, 168)");
        spinBox->setMinimumHeight(20);

        frameLayout->addWidget(label);
        frameLayout->addWidget(spinBox);

        subnetsPanelLayout->addWidget(frame);

        Subnetv4 *subnet = new Subnetv4;
        subnet->HostNumber = 3;
        subnets.append(subnet);
        SubnetsCalculatorV4();
    }
}

void widgetApp::MainWindow::on_radioButton_clicked(bool checked)
{
    isHorizontal = (checked) ? true : false;
}

void widgetApp::MainWindow::drawNetworkGraph(bool isVertical)
{
    subnetGraphContent->setLayout(graphPanelLayout);

    subnetsGraphFrame = new QFrame();

    QLayoutItem *child;
    while ((child = subnetGraphContent->layout()->takeAt(0)) != NULL)
    {
        if(child->widget() != NULL)
        {
            delete child->widget();
        }
        delete child;

    }

    QFrame *graphNetworkFrame = new QFrame();
    graphNetworkFrame->setStyleSheet("background-color: rgb(218, 86, 88)");
    subnetGraphContent->layout()->addWidget(graphNetworkFrame);
    graphNetworkFrame->setLayout(new QVBoxLayout());

    QString routerButtonText = QString::fromStdString("R" + std::to_string(1));
    QPushButton *routerButton = new QPushButton(routerButtonText);
    routerButton->setStyleSheet("background-color: rgb(218, 86, 88)");
    routerButton->setMinimumSize(QSize(50,50));
    routerButton->setMaximumSize(QSize(50,50));
    graphNetworkFrame->layout()->addWidget(routerButton);

    subnetsGraphFrame->setStyleSheet("background-color: rgb(227, 229, 115, 100)");

    if(isVertical)
        subnetsGraphFrame->setLayout(new QVBoxLayout());
    else  subnetsGraphFrame->setLayout(new QHBoxLayout());

    subnetsGraphFrame->layout()->setSpacing(50);
    graphNetworkFrame->layout()->addWidget(subnetsGraphFrame);

    for(int i = 0; i < subnets.count(); i++)
    {
         QVBoxLayout *frameLayout = new QVBoxLayout();

         QFrame *subnetFrame = new QFrame();
         subnetFrame->setLayout(frameLayout);

         subnetsGraphFrame->layout()->addWidget(subnetFrame);

         QString subButtonText = QString::fromStdString("S" + std::to_string(1 + i));
         QPushButton *subnetButton = new QPushButton(subButtonText);

         subnetButton->setStyleSheet("background-color: rgb(227, 229, 115)");
         subnetButton->setMinimumSize(QSize(40,40));
         subnetButton->setMaximumSize(QSize(40,40));
         subnetFrame->layout()->addWidget(subnetButton);

         QFrame *hostFrame = new QFrame();
         frameLayout -> addWidget(hostFrame);
         QHBoxLayout *hostsLayout = new QHBoxLayout();
         hostFrame->setLayout(hostsLayout);
         hostFrame->setStyleSheet("background-color: rgb(15, 159, 116,100)");

         for(int j = 0; j < subnets[i]->HostNumber; j++)
         {
            QString hostButtonText = QString::fromStdString("H" + std::to_string(1 + j));
            QPushButton *hostButton = new QPushButton(hostButtonText);
            hostButton->setStyleSheet("background-color: rgb(15, 159, 116)");
            hostButton->setMinimumSize(QSize(20,20));
            hostButton->setMaximumSize(QSize(20,20));
            hostsLayout->addWidget(hostButton);
         }
    }
}

void widgetApp::MainWindow::calculateBitsetFromInput(QWidget *inputWidget, QWidget *displayWidget)
{
    QList<QLineEdit*> addressLines = inputWidget->findChildren<QLineEdit*>();

    QString inputAddress= "";

    for (int i = 0; i < addressLines.count(); i++)
    {
        if(addressLines[i] != NULL)
        {
            inputAddress += addressLines[i]->text();
            if(i < addressLines.count() - 1) inputAddress += ".";
        }
    }

    std::string textAsString = inputAddress.toLocal8Bit().constData();
    address = IPv4address(parser.ipFromString(textAsString));

    std::string bitsetString;
    boost::to_string(address._IpAddress, bitsetString);

    QList<QLineEdit*> maskLines = displayWidget->findChildren<QLineEdit*>();

    for (int i = 0; i < maskLines.count(); i++)
    {
        if(addressLines[i] != NULL)
        {
            QString octText = QString::fromStdString(bitsetString.substr(i * 8, 8));
            maskLines[i]->setText(octText);
        }
    }
}
