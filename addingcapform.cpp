#include "addingcapform.h"
#include "ui_addingcapform.h"

AddingCapForm::AddingCapForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddingCapForm)
{
    ui->setupUi(this);

    //Заполняем даты
    ui->dateEdit_addingDate->setDate(QDate::currentDate());
    ui->dateEdit_findingDate->setDate(QDate::currentDate());

    //Заполняем списки из дополнительных таблиц
    QSqlQuery query;
    QString str = "SELECT * FROM %1;"; str = str.arg("colorstable");
    if(query.exec(str))
    {
        //считываем
        QSqlRecord rec = query.record();
        while(query.next())    //продолжаем только если есть хотя бы один ответ
        {
            ui->comboBox_mainColor->addItem(query.value(rec.indexOf("color_name")).toString());
            ui->comboBox_secondColor->addItem(query.value(rec.indexOf("color_name")).toString());
        }
    }
    else qDebug()<<"shit!"<<query.lastError();

    str = "SELECT * FROM %1;"; str = str.arg("firmstable");
    if(query.exec(str))
    {
        //считываем
        QSqlRecord rec = query.record();
        while(query.next())    //продолжаем только если есть хотя бы один ответ
        {
            ui->comboBox_manufacturer->addItem(query.value(rec.indexOf("firm_name")).toString());
        }
    }
    else qDebug()<<"shit!"<<query.lastError();
}

AddingCapForm::~AddingCapForm()
{
    delete ui;
}

void AddingCapForm::on_pushButton_addNewCap_clicked()   //Обработка нажатия кнопки Добавить
{
    QMessageBox messageBox(QMessageBox::Warning,
                "Предупреждение",
                "Произойдет запись в базу. Уверены?",
                QMessageBox::Yes | QMessageBox::No,
                this);
    messageBox.setButtonText(QMessageBox::Yes, "Да");
    messageBox.setButtonText(QMessageBox::No, "Нет");
    int n =messageBox.exec();
    if (n == QMessageBox::Yes)
    {
        int getID = 0;          //номер для кртинки
        //Создание новой строчки в базе, получение ИД
        QSqlQuery query;
        QString str = "INSERT INTO %1 (serie, name, firm, prime_color, second_color, header_text, find_date, add_date, commentary, photo_way, papka, list, stolb)"
                " VALUES ('%2', '%3', %4, %5, %6, '%7', '%8', '%9', '%10', '%11', %12, %13, %14);";// RETURNING id;";
        str = str.arg("maintable").arg(ui->lineEdit_seriaField->text()).arg(ui->lineEdit_nameField->text())
                .arg(ui->comboBox_manufacturer->currentIndex()+1).arg(ui->comboBox_mainColor->currentIndex()+1).arg(ui->comboBox_secondColor->currentIndex()+1)
                .arg(ui->lineEdit_headerField->text()).arg(ui->dateEdit_findingDate->date().toString("yyyy-MM-dd")).arg(ui->dateEdit_addingDate->date().toString("yyyy-MM-dd")).arg(ui->textEdit_commField->document()->toRawText()).arg("")
                .arg(ui->lineEdit_papkaField->text().toInt()).arg(ui->lineEdit_listField->text().toInt()).arg(ui->lineEdit_stolbecField->text().toInt());
                //+1 надо добавлять, потому что счет в связанных таблицах по айди идет не с нуля
        if(query.exec(str))
        {
            //считываем
            QSqlRecord rec = query.record();
            getID = query.lastInsertId().toInt();
        }
        else qDebug()<<"shit!"<<query.lastError()<<query.lastQuery();

        //сохранение фото
        if(photoPath.length()>0)
        {
            int flagError = 0;          //флажок сигнализации об ошибках копирования фотки
            QFileInfo fi(photoPath);
            QString newPhotoPath = QCoreApplication::applicationDirPath() + "/Photos/" + QString::number(getID).rightJustified(5, '0') + "." + fi.completeSuffix();

            if (QFile::exists(newPhotoPath)) {

                    if (!QFile::remove(newPhotoPath)) {
                        qDebug() << "Не удалось удалить существующий файл:";
                        flagError = 1;
                    }
                    else
                    if (!QFile::copy(photoPath, newPhotoPath)) {QMessageBox::warning(0, "Информация", "Ошибка копирования файла: из " + photoPath + " в " + newPhotoPath); flagError = 1;}
                }
            else
            if (!QFile::copy(photoPath, newPhotoPath)) {QMessageBox::warning(0, "Информация", "Ошибка копирования файла: из " + photoPath + " в " + newPhotoPath); flagError = 1; }
            if(flagError==0)    //отправляем информацию в бд о пути к фотке
            {
                QSqlQuery query;
                QString str = "UPDATE maintable SET photo_way='%1' WHERE id == %2;";
                str = str.arg("/Photos/" + QString::number(getID).rightJustified(5, '0') + "." + fi.completeSuffix()).arg(getID);
                if(!query.exec(str)) qDebug()<<"shit!"<<query.lastError()<<query.lastQuery();
            }
        }

        QMessageBox::information(0, "Информация", "Записано успешно!");

        //Если процесс записи успешен, то очищаем все поля и готовимся к новому вводу
        ui->dateEdit_addingDate->setDate(QDate::currentDate());
        ui->dateEdit_findingDate->setDate(QDate::currentDate());
        ui->comboBox_manufacturer->setCurrentIndex(0);
        ui->comboBox_mainColor->setCurrentIndex(0);
        ui->comboBox_secondColor->setCurrentIndex(0);
        ui->lineEdit_seriaField->setText("");
        ui->lineEdit_nameField->setText("");
        ui->lineEdit_headerField->setText("");
        ui->textEdit_commField->setText("");
        ui->lineEdit_papkaField->setText("");
        ui->lineEdit_listField->setText("");
        ui->lineEdit_stolbecField->setText("");
        ui->label_pictureField->clear();
        photoPath = "";
    }
}

void AddingCapForm::on_pushButton_exit_clicked()        //Обработка нажатия кнопки Назад
{
    this->close();
}

void AddingCapForm::on_pushButton_addPhoto_clicked()    //Обработка нажатия кнопки Загрузка фото
{
    photoPath = QFileDialog::getOpenFileName(0, "Выбор фотографии", "", "жыпег (*.jpg) ;; пынга (*.png) ;; Все файлы (*.*)");
    if(photoPath.length()>0)
    {
        QPixmap pixmap(photoPath);
        ui->label_pictureField->setPixmap(pixmap);
    }
}
