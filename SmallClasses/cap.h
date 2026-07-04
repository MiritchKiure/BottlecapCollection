#ifndef CAP_H
#define CAP_H

#include <QString>
#include <QFileInfo>

class Cap
{
    public:
        Cap();

        int id;                 //ИД
        QString serie;          //Условная серия, к которой относится пробка
        QString name;           //Условное имя пробки
        int firm;               //Код фирмы из БД
        int prime_color;        //Код первичного цвета из БД
        int second_color;       //Код вторичного цвета из БД
        QString header_text;    //Текст на пробке, если есть
        QString find_date;      //Когда найдено (кодировка в текст)
        QString add_date;       //Когда добавлено (кодировка в текст)
        QString commentary;     //Комментарий
        QString photo_way;      //Путь к фото
        int papka;              //номер папки
        int list;               //Номер листа
        int stolb;              //номер столбца

        int addPhoto(QString pathExt, QString appDir);      //Добавить картинку в хранилище (скопировать и переименовать)
        int deletePhoto();                  //Удалить сопряженное с этим объектом изображение в хранилище (и занулить переменную)

};

#endif // CAP_H
