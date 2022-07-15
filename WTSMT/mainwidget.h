#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "model.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QLayout>
#include <QListWidget>
#include <QPushButton>
#include <QDir>
#include <QDirIterator>
#include "public.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    QList<Model*> m_models;
    QMap<QString, QString> *m_files_list;

    void import();
    void checkFiles();
private:
    Ui::MainWidget *ui;
    QHBoxLayout *m_layout_main;
    QVBoxLayout *m_layout_models;
    QVBoxLayout *m_layout_files;
    QHBoxLayout *m_layout_files_edit;
    QListWidget *m_files;
    QPushButton *m_import;
    QPushButton *m_delete;
    QPushButton *m_clear;
    QPushButton *m_url;
};
#endif // MAINWIDGET_H
