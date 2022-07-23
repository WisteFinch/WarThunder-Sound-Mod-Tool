#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("WarThunder Sound Mod Builder"));
    this->m_layout_main = new QHBoxLayout;
    this->m_layout_files = new QVBoxLayout;
    this->m_layout_models = new QVBoxLayout;
    this->m_layout_files_edit = new QHBoxLayout;
    this->m_files = new QListWidget;
    this->m_import = new QPushButton;
    this->m_import->setText(tr("import"));
    this->m_delete = new QPushButton;
    this->m_delete->setText(tr("delete"));
    this->m_clear = new QPushButton;
    this->m_clear->setText(tr("clear"));
    setLayout(this->m_layout_main);
    this->m_layout_main->addLayout(this->m_layout_files);
    this->m_layout_files->addWidget(this->m_files);
    this->m_layout_files->addLayout(this->m_layout_files_edit);
    this->m_layout_main->addLayout(this->m_layout_models);
    this->m_layout_files_edit->addWidget(this->m_import);
    this->m_layout_files_edit->addWidget(this->m_delete);
    this->m_layout_files_edit->addWidget(this->m_clear);


    this->m_files_list = new QMap<QString, QString>;

    QStringList models;
    QString path = "models";
    QDir dir(path);
    if(dir.exists()){
        QStringList filters;
        filters<<QString("*.json");

        QDirIterator dir_iterator(path,
            filters,
            QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot,
            QDirIterator::Subdirectories);
        while(dir_iterator.hasNext())
        {
            dir_iterator.next();
            QFileInfo file_info = dir_iterator.fileInfo();
            QString absolute_file_path = file_info.absoluteFilePath();

            QJsonDocument document;
            QFile file(absolute_file_path);
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            if(file.isOpen()){
                QString value = file.readAll();
                file.close();
                QJsonParseError parseJsonErr;
                document = QJsonDocument::fromJson(value.toUtf8(),&parseJsonErr);
                if(parseJsonErr.error == QJsonParseError::NoError){
                    Model *m = new Model(nullptr, document.object(), this->m_files_list);
                    this->m_models.append(m);
                    QPushButton *b = new QPushButton();
                    b->setText(m->m_name);
                    this->m_layout_models->addWidget(b);
                    connect(b, &QPushButton::clicked, this, [=]{m->show();m->checkFiles();});
               }
            }
        }
    }else{
        QPushButton *b = new QPushButton();
        b->setText(tr("No model files"));
        b->setDisabled(true);
        this->m_layout_models->addWidget(b);
    }
    this->m_layout_models->addStretch();

    this->m_url = new QPushButton;
    this->m_url->setText(tr("Check for Update"));
    this->m_layout_models->addWidget(this->m_url);
    connect(this->m_url, &QPushButton::clicked, this, [=]{QDesktopServices::openUrl(QUrl("https://github.com/WisteFinch/WarThunder-Sound-Mod-Tool", QUrl::TolerantMode));});

    connect(this->m_import, &QPushButton::clicked, this, [=]{import();});
    connect(this->m_delete, &QPushButton::clicked, this, [=]{if(this->m_files->currentItem()!= nullptr){this->m_files_list->remove(this->m_files->currentItem()->text()); delete this->m_files->currentItem();checkFiles();}});
    connect(this->m_clear, &QPushButton::clicked, this, [=]{this->m_files->clear();this->m_files_list->clear();checkFiles();});

}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::import(){
    QStringList list = QFileDialog::getOpenFileNames(this, tr("Select sound files"), ".", "Sound(*.wav)");
    for(int i = 0; i < list.size();i++)
    {
        QString name = QFileInfo(list.at(i)).fileName();
        if(!this->m_files_list->contains(name)){
            this->m_files_list->insert(name, list.at(i));
            this->m_files->addItem(name);
        }
    }
    checkFiles();
}

void MainWidget::checkFiles()
{
    for(int i = 0; i < this->m_models.size(); i++){
        if(this->m_models.at(i)->isVisible())
            this->m_models.at(i)->checkFiles();
    }
}
