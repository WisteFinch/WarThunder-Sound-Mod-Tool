#ifndef ITEM_H
#define ITEM_H

#include <QJsonObject>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QString>
#include <QScrollArea>
#include <QLineEdit>
#include <QJsonArray>

namespace Ui {
class Item;
}

class Item : public QScrollArea
{
    Q_OBJECT

public:
    explicit Item(QWidget *parent = nullptr, QJsonObject obj = QJsonObject(), QMap<QString, QString> *langsList = new QMap<QString, QString>, QList<QString> *typesList = new QList<QString>);
    ~Item();

    QJsonObject m_obj;
    QMap<QString, QMap<QString,QList<QString>>> *m_mapping;
    QMap<QString, QString> *m_langs;
    QList<QString> *m_types;
    bool is_reday = false;
    QString m_cap;
    QString file_name;

private:
    Ui::Item *ui;

    QHBoxLayout *m_l_id;
    QLabel *m_id;
    QLineEdit *m_id_edit;
    QHBoxLayout *m_l_name;
    QLabel *m_name;
    QLineEdit *m_name_edit;
    QHBoxLayout *m_l_file;
    QLabel *m_file;
    QLineEdit *m_file_edit;
    QWidget *m_widget;
    QVBoxLayout *m_layout;
    QLabel *m_t_mapping;
    QHBoxLayout *m_l_t_mapping;
    QVBoxLayout *m_l_mapping;
};

#endif // ITEM_H
