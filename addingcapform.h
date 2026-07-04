#ifndef ADDINGCAPFORM_H
#define ADDINGCAPFORM_H

#include <QDialog>
#include <QtWidgets>
#include <QFile>
#include <QFileInfo>
//Для базы данных
#include <QtSql/QSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql/QSqlDatabase>
#include <QSqlError>

namespace Ui {
class AddingCapForm;
}

class AddingCapForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddingCapForm(QWidget *parent = nullptr);
    ~AddingCapForm();

private slots:
    void on_pushButton_addNewCap_clicked();
    void on_pushButton_exit_clicked();

    void on_pushButton_addPhoto_clicked();

private:
    Ui::AddingCapForm *ui;

    QString photoPath;                                  //путь-имя к картинке пробки
};

#endif // ADDINGCAPFORM_H
