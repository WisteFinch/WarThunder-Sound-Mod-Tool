#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // 创建界面
    this->setWindowTitle(tr("WarThunder Sound Mod Tool"));
    this->m_layout_main = new QHBoxLayout;
    this->m_layout_files = new QVBoxLayout;
    this->m_layout_models = new QVBoxLayout;
    this->m_layout_files_edit = new QHBoxLayout;
    this->m_files = new QListWidget;
    this->m_import = new QPushButton;
    this->m_import->setText(tr("Import"));
    this->m_delete = new QPushButton;
    this->m_delete->setText(tr("Delete"));
    this->m_clear = new QPushButton;
    this->m_clear->setText(tr("Clear"));
    setLayout(this->m_layout_main);
    this->m_layout_main->addLayout(this->m_layout_files);
    this->m_layout_files->addWidget(this->m_files);
    this->m_layout_files->addLayout(this->m_layout_files_edit);
    this->m_layout_main->addLayout(this->m_layout_models);
    this->m_layout_files_edit->addWidget(this->m_import);
    this->m_layout_files_edit->addWidget(this->m_delete);
    this->m_layout_files_edit->addWidget(this->m_clear);


    // 创建文件映射
    this->m_files_list = new QMap<QString, QString>;

    // 读取模型
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
                    // 创建新模型
                    Model *m = nullptr;
                    this->m_models.append(QPair<Model*,QJsonObject>(m,document.object()));
                    // 加入按钮
                    QPushButton *b = new QPushButton();
                    b->setText(document.object().value("name").toString());
                    this->m_layout_models->addWidget(b);
                    int index = this->m_models.size()-1;
                    connect(b, &QPushButton::clicked, this, [=]{showModel(index);});
               }
            }
        }
    }
    // 无模型文件
    else{
        QPushButton *b = new QPushButton();
        b->setText(tr("No model files"));
        b->setDisabled(true);
        this->m_layout_models->addWidget(b);
    }

    // 更新按钮
    this->m_layout_models->addStretch();
    this->m_github = new QPushButton;
    this->m_github->setText(tr("Update && GitHub"));
    this->m_layout_models->addWidget(this->m_github);
    this->m_usage = new QPushButton;
    this->m_usage->setText(tr("Usage"));
    this->m_layout_models->addWidget(this->m_usage);

    // 版本信息
    this->m_version = new QLabel(tr("Version: ") + QCoreApplication::applicationVersion());
    this->m_layout_models->addWidget(this->m_version);
    this->m_version->setAlignment(Qt::AlignCenter);

    // 绑定信号
    connect(this->m_github, &QPushButton::clicked, this, [=]{QDesktopServices::openUrl(QUrl("https://github.com/WisteFinch/WarThunder-Sound-Mod-Tool", QUrl::TolerantMode));});
    connect(this->m_usage, &QPushButton::clicked, this, [=]{QDesktopServices::openUrl(QUrl("https://github.com/WisteFinch/WarThunder-Sound-Mod-Tool/blob/main/README.md", QUrl::TolerantMode));});
    connect(this->m_import, &QPushButton::clicked, this, &MainWidget::import);
    connect(this->m_delete, &QPushButton::clicked, this, [=]{if(this->m_files->currentItem()!= nullptr){QStringList l;l.append(this->m_files->currentItem()->text());this->m_files_list->remove(this->m_files->currentItem()->text()); delete this->m_files->currentItem();checkFiles(2, l);}});
    connect(this->m_clear, &QPushButton::clicked, this, [=]{this->m_files->clear();this->m_files_list->clear();checkFiles(-1);});

}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::import(){
    // 获取导入文件列表
    QStringList list = QFileDialog::getOpenFileNames(this, tr("Select sound files"), ".", "Sound(*.wav)");
    QStringList l;
    for(int i = 0; i < list.size();i++)
    {
        QString name = QFileInfo(list.at(i)).fileName();
        if(!this->m_files_list->contains(name)){
            this->m_files_list->insert(name, list.at(i));
            this->m_files->addItem(name);
            l.append(name);
        }
    }
    // 检查文件
    checkFiles(1, l);
}

void MainWidget::checkFiles(int type)
{
    QStringList l;
    checkFiles(type, l);
}

void MainWidget::checkFiles(int type, QStringList l)
{
    // 开启的模型检查文件
    for(int i = 0; i < this->m_models.size(); i++){
        if(this->m_models.at(i).first != nullptr)
            this->m_models.at(i).first->checkFiles(type, l);
    }
}

void MainWidget::showModel(int index){
    // 显示模型界面
    Model *m = this->m_models.at(index).first;
    if(m == nullptr){
        m = new Model(nullptr, this->m_models.at(index).second, this->m_files_list, index);
        connect(m, &Model::widgetClose, this, &MainWidget::closeModel);
    }
    this->m_models[index].first = m;
    m->checkFiles(0);
}

void MainWidget::closeModel(int index){
    this->m_models[index].first = nullptr;
}

void MainWidget::closeEvent(QCloseEvent *event){
    qApp->quit();
}
