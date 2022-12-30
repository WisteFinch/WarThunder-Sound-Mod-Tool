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
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QPair>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    QList<QPair<Model*,QJsonObject>> m_models;  ///< 模型列表
    QMap<QString, QString> *m_files_list;       ///< 音频文件映射

    void import();                              ///< 导入音频文件
    void checkFiles(int type);                  ///< 检查文件
    void checkFiles(int type, QStringList l);   ///< 检查多个文件
    void closeEvent(QCloseEvent *event);        ///< 关闭信号

    void showModel(int index);                  ///< 显示模型
    void closeModel(int index);                ///< 关闭模型

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
    QPushButton *m_github;
    QPushButton *m_usage;
};
#endif // MAINWIDGET_H
