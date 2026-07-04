#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QtWidgets>

//Для базы данных
#include <QtSql/QSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QSqlRelationalTableModel>

#include "SmallClasses/cap.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSqlError CreateConnection();            //открытие базы

private slots:
    void on_pushButton_FindProbka_clicked();
    void on_pushButton_AddProbka_clicked();
    void on_pushButton_updateCap_clicked();
    void on_pushButton_deleteCap_clicked();
    void on_pushButton_exitMain_clicked();
    void on_pushButton_changeView_clicked();

    void on_tableView_bc_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel* MainModel;
};
#endif // MAINWINDOW_H
