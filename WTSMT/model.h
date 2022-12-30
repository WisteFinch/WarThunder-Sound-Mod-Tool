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
#include <QDebug>
#include "item.h"
#include "build.h"

namespace Ui {
class Model;
}

class Model : public QWidget
{
    Q_OBJECT

public:
    explicit Model(QWidget *parent = nullptr, QJsonObject obj = QJsonObject(), QMap<QString, QString> *filesList = new QMap<QString, QString>, int index = 0);
    ~Model();

    QJsonObject m_obj;
    int index;

    QList<Item*> *m_list;
    QMap<QString, int> m_list_mapping;
    QString m_name;
    QMap<QString, QString> *m_files_list;
    QMap<QString, QString> *m_langs;
    QList<QString> *m_types;

    void checkFiles(int type);
    void checkFiles(int type, QStringList l);

signals:
    void widgetClose(int index);

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
