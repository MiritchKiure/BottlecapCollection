#include "cap.h"

Cap::Cap()
{
    //Заполняем пустыми значениями
    id = 0;                 //ИД
    serie = "";          //Условная серия, к которой относится пробка
    name = "";           //Условное имя пробки
    firm = 0;               //Код фирмы из БД
    prime_color = 0;        //Код первичного цвета из БД
    second_color = 0;       //Код вторичного цвета из БД
    header_text = "";    //Текст на пробке, если есть
    find_date = "";      //Когда найдено (кодировка в текст)
    add_date = "";       //Когда добавлено (кодировка в текст)
    commentary = "";     //Комментарий
    photo_way = "";      //Путь к фото
    papka = 0;              //номер папки
    list = 0;               //Номер листа
    stolb = 0;              //номер столбца
}

int Cap::addPhoto(QString pathExt, QString appDir)              //Добавление фото по внешней ссылке
{
    QFileInfo fi(pathExt);
    QString newPhotoPath = appDir + "/Photos/" + QString::number(id).rightJustified(5, '0') + "." + fi.completeSuffix();
    if (!QFile::copy(pathExt, newPhotoPath)) return 1;//QMessageBox::warning(0, "Информация", "Ошибка копирования файла: из " + photoPath + " в " + newPhotoPath);
    photo_way = newPhotoPath;
    return 0;
}

int Cap::deletePhoto()                          //Удаление фото из хранилища
{
    if(!QFile::remove(photo_way)) return 1;
    return 0;
}
