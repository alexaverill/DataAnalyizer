#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "data.h"
#include "datapoint.h"
#include "database.h"
#include<QFileDialog>
#include<stack>
#include<QDebug>
#include "localsetting.h"
#include<QSettings>
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //loat in settings.
    QSettings settings("Data", "Data");
    localsettings ls;
    ls.setUserName(settings.value("username").toString());
    ls.setPassword(settings.value("password").toString());
    ls.setDatabaseName(settings.value("databaseName").toString());
    ls.setHostName(settings.value("host").toString());
    ui->setupUi(this);
    setupTempDropBox();
    setupSamplesBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setupTempDropBox()
{
    Database db;
   QStringList temps = db.returnTemperatures();
   temps.insert(0,"All");
   ui->tempEntryBox->addItems(temps);
   ui->tempView->addItems(temps);
}
void MainWindow::setupSamplesBox()
{
    Database db;
    QStringList samples = db.returnSamples();
    ui->sampleEntryBox->addItems(samples);
    ui->compoundView->addItems(samples);
}
void MainWindow::setupTrialDropbox()
{
    Database db;
    std::stack<QString> entries = db.returnTrials();
    //ui->trialEntryBox->addItem();
}
void MainWindow::on_processData_clicked()
{

    dataPoint dP;
    ui->progressBar->setValue(10);
    dP.temperature = ui->tempEntryBox->currentText().toInt();
    dP.trial = ui->trialEntryBox->currentText().toInt();
    dP.compound = ui->sampleEntryBox->currentText();
    if(dP.trial <=0 || dP.compound <=0 || dP.temperature <=0)
    {
        return; //Not all are filled in.
    }
    Data d(this->fileIn);
    d.processFile(&dP);
    ui->progressBar->setValue(100);
}

void MainWindow::on_trialEntryBox_currentIndexChanged(int index)
{
    this->dP.trial = index;
    ui->progressBar->setValue(1);
}

void MainWindow::on_tempEntryBox_currentIndexChanged(int index)
{
    this->dP.temperature = index;
    ui->progressBar->setValue(1);
}

void MainWindow::on_trialEntryBox_activated(int index)
{

}


void MainWindow::on_selectFile_clicked()
{
  this->fileIn = QFileDialog::getOpenFileName(this,tr("Open File"),"");
  ui->fileNameDisp->setText(fileIn);
  ui->progressBar->setValue(1);
}

void MainWindow::on_sampleEntryBox_currentTextChanged(const QString &arg1)
{

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stdBar->setValue(10);
    Database d;
    d.setStdDev();
    ui->stdBar->setValue(100);
}

void MainWindow::on_ExportCSV_clicked()
{
    this->fileOut = QFileDialog::getSaveFileName(this,tr("Export to CSV"),"",tr("CSV (.csv)"));
    int rxnArea=ui->areaBox->value();
    QString sample= ui->compoundView->currentText();
    Database d;
    if(ui->tempView->currentText() == "All"){
        d.specificExport(fileOut,sample,rxnArea);
    }else{
        int temp =ui->tempView->currentText().toInt();
        d.specificExport(fileOut,sample,rxnArea,temp);
    }
}


void MainWindow::on_settings_clicked()
{
    settings set;
    set.exec();
}



void MainWindow::on_avgBtn_clicked()
{
    Database d;
    int rxnArea=ui->areaBox->value();
    QString sample= ui->compoundView->currentText();

    int temp =ui->tempView->currentText().toInt();
    float av = d.returnAvg(sample,temp,rxnArea);
    qDebug()<<av;
    QString dispAvg = QString::number(av);
    ui->avgDisp->setText(dispAvg);
}

void MainWindow::on_calcAvg_clicked()
{
    ui->avgBar->setValue(10);
    Database d;
    d.calculateAvg();
    ui->avgBar->setValue(100);
}
