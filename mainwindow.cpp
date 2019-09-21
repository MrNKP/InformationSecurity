#include "mainwindow.h"
#include "ui_mainwindow.h"

#define DEBUG   false

#include <QMessageBox>

#include <cmath>
#include <vector>

using namespace std;

QString src, encrypted, decrypted;
QString cnt, order;
int cntInt;
vector<int> orderInt;
int lastSymbols;
int cntChars;
vector<QString> newLine;
vector<QString> splitLine;

vector<int> getValues(QString src)
{
    vector<int> result;

    int lastPos=0;
    int value;
    QString tmp;
    for (int i=0; i<src.size(); i++)
    {
        if (src[i] == ' ')
        {
            tmp = src.mid(lastPos, i-lastPos);
            value = tmp.toInt();
            result.push_back(value);
            lastPos = i;
        }
    }
    tmp = src.mid(lastPos, src.size()-lastPos);
    value = tmp.toInt();
    result.push_back(value);

    return result;
}

bool checkOrder(vector<int> src)
{
    bool result;

    result = true;
    int i=0;
    while (result && i<src.size())
    {
        result = (src[i]!=0);
        if (result) i++;
    }

    return result;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //encrypt
    int cntSymbols, srcSize;
    src = ui->lineEdit->text();
    cnt = ui->lineEdit_4->text();
    order = ui->lineEdit_5->text();
    if (src != "" && cnt != "" && order != "")
    {
    cntInt = cnt.toInt();
    orderInt = getValues(order);
    if (DEBUG)
    {
        QMessageBox::information(this, "DEBUG", QString::number(cntInt), QMessageBox::Ok);
        for (int i=0; i<orderInt.size(); i++)
            QMessageBox::information(this, "DEBUG", QString::number(orderInt[i]), QMessageBox::Ok);
        if (checkOrder(orderInt)) QMessageBox::information(this, "DEBUG", "Good", QMessageBox::Ok);
        else QMessageBox::information(this, "DEBUG", "Bad", QMessageBox::Ok);
    }
    //QMessageBox::information(this, "OK", "OK", QMessageBox::Ok);
    if (cntInt != 0 && checkOrder(orderInt))
    {
        cntSymbols = src.size()%cntInt;
        srcSize = src.size()-1;
        if (cntSymbols != 0)
        {
            //for (int i=0; i<cntSymbols; i++)
            //    src[srcSize+i] = ' ';
            lastSymbols = (src.size()/cntInt+1)*cntInt-srcSize-1;
            for (int i=0; i<lastSymbols; i++)
                src += ' ';
        }
        //QMessageBox::information(this, "DEBUG", "SRC new size = " + QString::number(src.size()), QMessageBox::Ok);
        cntChars = src.size() / cntInt;
        for (int i=0; i<cntInt; i++)
        {
            splitLine.push_back(src.mid(i*cntChars,cntChars));
            //QMessageBox::information(this, "DEBUG", splitLine[i], QMessageBox::Ok);
        }
        encrypted = "";
        for (int i=0; i<cntInt; i++)
        {
            newLine.push_back(splitLine[orderInt[i]-1]);
            encrypted += newLine[i];
        }
        ui->lineEdit_2->setText(encrypted);
        newLine.clear();
        splitLine.clear();
    }
    else
        QMessageBox::critical(this, "Error", "Bad block count", QMessageBox::Ok);
    }
    else {
        QMessageBox::critical(this, "Error", "Not Enough data", QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    //decrypt
    QMessageBox::StandardButton answer = QMessageBox::information(this, "Choise", "New string?", QMessageBox::Yes | QMessageBox::No);
    if (answer == QMessageBox::Yes)
    {
        src = ui->lineEdit->text();
        //QMessageBox::information(this, "OK", "OK?", QMessageBox::Ok);
        cnt = ui->lineEdit_4->text();
        order = ui->lineEdit_5->text();
        if (cnt != "" && order != "")
        {
            cntInt = cnt.toInt();
            orderInt = getValues(order);
            if (src.size() % cntInt == 0)
            {
                if (cntInt != 0 && checkOrder(orderInt))
                {
                    cntChars = src.size() / cntInt;
                    for (int i=0; i<cntInt; i++)
                    {
                        splitLine.push_back(src.mid(i*cntChars,cntChars));
                        newLine.push_back("");
                    }
                    for (int i=0; i<cntInt; i++)
                        newLine[orderInt[i]-1]=splitLine[i];
                    decrypted = "";
                    for (int i=0; i<cntInt; i++) decrypted += newLine[i];
                    ui->lineEdit_3->setText(decrypted);
                    newLine.clear();
                    splitLine.clear();
                }
                else
                    QMessageBox::critical(this, "Error", "Can't parse block count or order!", QMessageBox::Ok);
            }
            else
                QMessageBox::critical(this, "Error", "Bad src string length!", QMessageBox::Ok);
        }
        else
            QMessageBox::critical(this, "Error", "Bad block count or order!", QMessageBox::Ok);
    }
    else
    {
        encrypted = ui->lineEdit_2->text();
        cnt = ui->lineEdit_4->text();
        order = ui->lineEdit_5->text();
        if (cnt != "" && order != "")
        {
            cntInt = cnt.toInt();
            orderInt = getValues(order);
            if (cntInt != 0 && checkOrder(orderInt))
            {
                cntChars = encrypted.size() / cntInt;
                for (int i=0; i<cntInt; i++)
                {
                    splitLine.push_back(encrypted.mid(i*cntChars,cntChars));
                    newLine.push_back("");
                }
                for (int i=0; i<cntInt; i++)
                    newLine[orderInt[i]-1]=splitLine[i];
                decrypted = "";
                for (int i=0; i<cntInt; i++) decrypted += newLine[i];
                ui->lineEdit_3->setText(decrypted);
            }
            else
                QMessageBox::critical(this, "Error", "Can't parse block count or order", QMessageBox::Ok);
        }
        else
            QMessageBox::critical(this, "Error", "Bad block count or order", QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    src.clear();
    encrypted.clear();
    decrypted.clear();
    cnt.clear();
    order.clear();
    newLine.clear();
    splitLine.clear();
    cntInt = 0;
    orderInt.clear();
}
