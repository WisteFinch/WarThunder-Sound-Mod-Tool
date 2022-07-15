#ifndef MODEL_H
#define MODEL_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QLayout>
#include <QListWidget>
#include <QList>
#include <QStackedLayout>
#include <QPushButton>
#include "item.h"
#include "build.h"

namespace Ui {
class Model;
}

class Model : public QWidget
{
    Q_OBJECT

public:
    explicit Model(QWidget *parent = nullptr, QJsonObject obj = QJsonObject(), QMap<QString, QString> *filesList = new QMap<QString, QString>);
    ~Model();

    QJsonObject m_obj;

    QList<Item*> *m_list;
    QString m_name;
    QMap<QString, QString> *m_files_list;
    QMap<QString, QString> *m_langs;
    QList<QString> *m_types;

    void checkFiles();

private:
    Ui::Model *ui;
    QHBoxLayout *m_layout_main;
    QVBoxLayout *m_layout_list;
    QHBoxLayout *m_layout_button;
    QPushButton *m_build;
    QListWidget *m_list_widget;
    QStackedLayout *m_stack;
};

#endif // MODEL_H
