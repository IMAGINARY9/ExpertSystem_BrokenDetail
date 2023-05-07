
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMessageBox>
#include <QMainWindow>
#include <QtCore>
#include <QtGui>

#include <QFile>
#include <QTextStream>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    void factorsAndDetails_init(QStringList &factorsAndDetailsList);
    void factorsList_build();
    void models_updale();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void exclusion_of_superfluous();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();


    void selectedFactorsAndDetailMap_reset();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QString, QStringList> *factorsAndDetailsMap;
    QMap<QString, QStringList> *selectedFactorsAndDetailsMap;
    QStringList *factorsList;
    QStringList *selectedFactorsList;
    QStringListModel *oldModel;
    QStringListModel *newModel;
};

#endif // MAINWINDOW_H
