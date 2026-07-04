#include "mainwindow.h"
#include "addingcapform.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //заголовок проекта
    this->setWindowTitle("Коллекция пробок v" + QString::fromLocal8Bit(APP_VERSION));

    //Открытие базы
    if(CreateConnection().type()==QSqlError::NoError) ui->label_statusLabel->setText("База загружена.");

    //Создание модели и заполнение таблицы
    MainModel = new QSqlRelationalTableModel;
    MainModel->setTable("maintable");
    MainModel->setRelation(3, QSqlRelation("firmstable", "id", "firm_name"));               //Связи с внешними таблицами
    MainModel->setRelation(4, QSqlRelation("colorstable", "id", "color_name"));
    MainModel->setRelation(5, QSqlRelation("colorstable", "id", "color_name"));
    MainModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    MainModel->select();
    MainModel->setHeaderData(1, Qt::Horizontal, tr("Серия"));
    MainModel->setHeaderData(2, Qt::Horizontal, tr("Название"));
    MainModel->setHeaderData(3, Qt::Horizontal, tr("Фирма"));
    MainModel->setHeaderData(4, Qt::Horizontal, tr("Основной цвет"));
    MainModel->setHeaderData(5, Qt::Horizontal, tr("Вторичный цвет"));
    MainModel->setHeaderData(6, Qt::Horizontal, tr("Надпись"));
    MainModel->setHeaderData(7, Qt::Horizontal, tr("Найдено"));
    MainModel->setHeaderData(8, Qt::Horizontal, tr("Добавлено"));
    MainModel->setHeaderData(9, Qt::Horizontal, tr("Комментарий"));
    MainModel->setHeaderData(11, Qt::Horizontal, tr("Папка, №"));
    MainModel->setHeaderData(12, Qt::Horizontal, tr("Лист, №"));
    MainModel->setHeaderData(13, Qt::Horizontal, tr("Столбец, №"));
    ui->tableView_bc->setModel(MainModel);
    //ui->tableView_bc->setAlternatingRowColors(true);
    //ui->tableView_bc->resizeRowsToContents();
    ui->tableView_bc->resizeColumnsToContents();
    ui->tableView_bc->hideColumn(0); // don't show the ID
    ui->tableView_bc->hideColumn(10);
    ui->tableView_bc->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_FindProbka_clicked()         //Обработка нажатия кнопки Найти
{
    ;
}

void MainWindow::on_pushButton_AddProbka_clicked()          //Обработка нажатия кнопки Добавить
{
    AddingCapForm* addcapForm = new AddingCapForm(this);
    connect(addcapForm, &QDialog::rejected,
            [=]()
            {
                MainModel->select();                        //По выходу командуем основной форме обновить данные в таблице
                ui->label_statusLabel->setText("Таблица обновлена.");
            });
    addcapForm->show();
}

void MainWindow::on_pushButton_updateCap_clicked()  //обработка нажатия кнопки Изменить
{
    if(ui->tableView_bc->currentIndex().row()>=0)        //Если строка выделена
    {
        QMessageBox::information(0, "Информация", QString::number(MainModel->record(ui->tableView_bc->currentIndex().row()).value("id").toInt()) );
    }
    else QMessageBox::information(0, "Информация", "Строка не выделена");
}

void MainWindow::on_pushButton_deleteCap_clicked()  //обработка нажатия кнопки Удалить
{
    if(ui->tableView_bc->currentIndex().row()>=0)        //Если строка выделена
    {
        QMessageBox messageBox(QMessageBox::Warning,
                    "Предупреждение",
                    "Запись будет полностью удалена. Уверены?",
                    QMessageBox::Yes | QMessageBox::No,
                    this);
        messageBox.setButtonText(QMessageBox::Yes, "Да");
        messageBox.setButtonText(QMessageBox::No, "Нет");
        int n =messageBox.exec();
        if (n == QMessageBox::Yes)
        {
            ui->label_statusLabel->setText("Запись удалена.");
            QMessageBox::information(0, "Информация", "Запись удалена.");
        }
    }
    else QMessageBox::information(0, "Информация", "Строка не выделена!");
}

void MainWindow::on_pushButton_changeView_clicked() //Обработка нажатия кнопки Сменить вид
{
    ; //Задел на будущее - чтобы таблица менялась на палитру квадратиков с картинками (возможно даже с перелистыванием страниц и разбиением на рельные положения в файликах)
}

void MainWindow::on_pushButton_exitMain_clicked()   //Обработка нажатия кнопки Выход
{
    this->close();
}

void MainWindow::on_tableView_bc_clicked(const QModelIndex &index)      //Обработка нажатия на таблицу
{
    if(index.row()>=0)
    {
        ui->label_NameText->setText(MainModel->record(index.row()).value("name").toString());
        ui->label_SerieText->setText(MainModel->record(index.row()).value("serie").toString());
        //ui->label_firmText->setText(MainModel->relationModel(3)->record(MainModel->record(index.row()).value("firm").toInt()).value("firm_name").toString());
        ui->label_firmText->setText(MainModel->relationModel(3)->record(2).value("firm_name").toString());
        ui->label_headerText->setText(MainModel->record(index.row()).value("header_text").toString());
qDebug()<<MainModel->record(index.row()).value("firm").toInt()<<MainModel->record(index.row()).value("prime_color").toInt()<<MainModel->record(index.row()).value("second_color").toInt();
        //MainModel->relationModel(1)->record(index.row()).value("firm_name").toString();

        //MainModel->record(index.row()).value("firm").toInt()
    }
}

QSqlError MainWindow::CreateConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/tools/BottlecapCollection/BottleTable/BottleCapDataBase.db");

    if (!db.open()) return db.lastError();

qDebug()<<db.tables();

    QSqlQuery query;
    QString str = "SELECT sqlite_version();";
    if(query.exec(str))
    {
        //считываем
        QSqlRecord rec = query.record();
        if(query.next())    //продолжаем только если есть хотя бы один ответ
        {
qDebug()<<"sqlite_version: "<<query.value(rec.indexOf("sqlite_version()")).toString();
        }
    }

    return QSqlError();
}
