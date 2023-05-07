
#include "mainwindow.h"
#include "ui_mainwindow.h"

QStringList Read(QString Filename, QString splitSymbol)
{
    QFile mFile(Filename);
    QStringList result;

    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "couldn't open file for reading";
    }
    else
    {
        QTextStream in(&mFile);
        QString mText = in.readAll();
        result = mText.split(splitSymbol);

        mFile.flush();
        mFile.close();
    }

    return result;
}

void MainWindow::factorsAndDetails_init(QStringList &factorsAndDetailsList) {
    for (const QString &str : factorsAndDetailsList) {
        QString strLeft = str.left(str.indexOf(": "));
        QStringList strRight = (str.right(str.length() - str.indexOf(": ") - 2)).split(", ");
        factorsAndDetailsMap->insert(strLeft, strRight);
        selectedFactorsAndDetailsMap->insert(strLeft, strRight);
    }

    factorsList_build();
}

void MainWindow::selectedFactorsAndDetailMap_reset()
{
    selectedFactorsAndDetailsMap->clear();
    for (const auto &key : factorsAndDetailsMap->keys()) {
        selectedFactorsAndDetailsMap->insert(key, factorsAndDetailsMap->value(key));
    }
}

void MainWindow::factorsList_build() {
    factorsList->clear();
    for (const auto &key : selectedFactorsAndDetailsMap->keys()) {
        for (const QString &factor : selectedFactorsAndDetailsMap->value(key)) {
            bool uniq = true;
            for (const QString &str : *factorsList) {
                if (str == factor)
                    uniq = false;
            }

            if (uniq)
                factorsList->push_back(factor);
        }
    }
    for (const QString &str : *selectedFactorsList) {
        for (int i = 0; i < factorsList->size(); i++) {
            if(str == factorsList->at(i))
            {
                factorsList->removeAt(i);
                break;
            }
        }
    }
}

void MainWindow::models_updale() {
    oldModel->setStringList(*factorsList);
    newModel->setStringList(*selectedFactorsList);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    factorsAndDetailsMap = new QMap<QString, QStringList>();
    selectedFactorsAndDetailsMap = new QMap<QString, QStringList>();
    factorsList = new QStringList();
    selectedFactorsList = new QStringList();
    oldModel = new QStringListModel(this);
    newModel = new QStringListModel(this);

    QString factorsAndDetailsFile = ":/details&factors/details&factors.txt";
    QStringList factorsAndDetailsList = Read(factorsAndDetailsFile, "\n");

    factorsAndDetails_init(factorsAndDetailsList);

    models_updale();

    ui->listView->setModel(oldModel);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView_2->setModel(newModel);
    ui->listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete factorsAndDetailsMap;
    delete factorsList;
    delete selectedFactorsList;
    delete ui;
}


void MainWindow::exclusion_of_superfluous() {
    for (const QStringList &value : selectedFactorsAndDetailsMap->values())
    {
        int counter = 0;
        for (const QString &str : value)
        {
            if(selectedFactorsList->contains(str))
                counter++;
        }
        if (counter != selectedFactorsList->size())
        {
            QString toDelete = selectedFactorsAndDetailsMap->key(value);
            selectedFactorsAndDetailsMap->remove(toDelete);
        }
    }
    factorsList_build();
}


void MainWindow::on_pushButton_clicked() {
    //Choose
    QModelIndex selectedIndex = ui->listView->currentIndex();
    if(selectedIndex.row() == -1)
    {
        QMessageBox::warning(this, "Attention!", "Choose one from the corresponding column.");
        return;
    }

    QString selectedFactor = selectedIndex.data().toString();

    selectedFactorsList->push_back(selectedFactor);

    exclusion_of_superfluous();
    models_updale();

    if(selectedFactorsAndDetailsMap->size() == 1)
    {
        QString result = "Your broken detail is ";
        result.append(selectedFactorsAndDetailsMap->firstKey());
        QMessageBox::information(this, "System success", result);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    //Remove
    QModelIndex selectedIndex = ui->listView_2->currentIndex();
    if(selectedIndex.row() == -1)
    {
        QMessageBox::warning(this, "Attention!", "Choose one from the corresponding column.");
        return;
    }

    selectedFactorsList->remove(selectedIndex.row());

    selectedFactorsAndDetailMap_reset();

    exclusion_of_superfluous();
    models_updale();

}


void MainWindow::on_pushButton_3_clicked()
{
    //Clear
    selectedFactorsList->clear();
    selectedFactorsAndDetailMap_reset();

    exclusion_of_superfluous();
    models_updale();
}

