#ifndef BUILD_H
#define BUILD_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QMap>
#include <QList>
#include <QPushButton>
#include <QFile>
#include <QScrollArea>
#include <QCheckBox>
#include "item.h"
#include <QProgressBar>
#include <QComboBox>
#include "public.h"
#include "QFileDialog"
#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Build;
}

class Build : public QDialog
{
    Q_OBJECT

public:
    explicit Build(QWidget *parent = nullptr, QMap<QString, QString> *filesList = new QMap<QString, QString>, QMap<QString, QString> *langsList = new QMap<QString, QString>, QList<QString> *typesList = new QList<QString>, QList<Item*> *itemList = new QList<Item*>, QString title = QString());
    ~Build();

    QList<Item*> *m_list;
    QMap<QString, QString> *m_files_list;
    QMap<QString, QString> *m_langs;
    QList<QString> *m_types;

    bool fill_empty = false;
    int replace_type = 0;

    QCheckBox *m_c_fill_empty;
    QComboBox *m_c_replace_type;

    QMap<QString, bool> m_enable_type;
    QMap<QString, bool> m_enable_lang;

    QVBoxLayout *m_layout_main;
    QScrollArea *m_scroll;
    QWidget *m_widget;
    QHBoxLayout *m_layout_checkbox;
    QVBoxLayout *m_layout_type;
    QVBoxLayout *m_layout_lang;
    QVBoxLayout *m_layout_conf;
    QHBoxLayout *m_layout_button;
    QPushButton *m_start;
    QProgressBar *m_bar;
    QLabel *m_label_conf;
    QLabel *m_label_type;
    QLabel *m_label_lang;

    QString createMultipleFolders(QString path);

public slots:
    void start();

private:
    Ui::Build *ui;
};

#endif // BUILD_H
