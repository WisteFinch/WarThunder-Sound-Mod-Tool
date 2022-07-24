#include "build.h"
#include "ui_build.h"

Build::Build(QWidget *parent, QMap<QString, QString> *filesList, QMap<QString, QString> *langsList, QList<QString> *typesList, QList<Item*> *itemList, QString title) :
    QDialog(parent),
    ui(new Ui::Build)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    this->m_files_list = filesList;
    this->m_langs = langsList;
    this->m_list = itemList;
    this->m_types = typesList;

    this->setWindowTitle(tr("Build:") + " " + title);
    this->setWindowModality(Qt::ApplicationModal);

    this->m_layout_main = new QVBoxLayout;
    this->setLayout(this->m_layout_main);
    this->m_scroll = new QScrollArea;
    this->m_layout_main->addWidget(this->m_scroll);
    this->m_widget = new QWidget;
    this->m_scroll->setWidget(this->m_widget);
    this->m_layout_checkbox = new QHBoxLayout;
    this->m_widget->setLayout(this->m_layout_checkbox);
    this->m_scroll->setWidgetResizable(true);
    this->m_layout_conf = new QVBoxLayout;
    this->m_layout_checkbox->addLayout(this->m_layout_conf);
    this->m_label_conf = new QLabel(tr("Configuration"));
    this->m_layout_conf->addWidget(this->m_label_conf);
    this->m_layout_conf->addWidget(new Line());
    this->m_layout_type = new QVBoxLayout;
    this->m_layout_checkbox->addLayout(this->m_layout_type);
    this->m_label_type = new QLabel(tr("Type"));
    this->m_layout_type->addWidget(this->m_label_type);
    this->m_layout_type->addWidget(new Line());
    this->m_layout_lang = new QVBoxLayout;
    this->m_layout_checkbox->addLayout(this->m_layout_lang);
    this->m_label_lang = new QLabel(tr("Language"));
    this->m_layout_lang->addWidget(this->m_label_lang);
    this->m_layout_lang->addWidget(new Line());
    this->m_layout_button = new QHBoxLayout;
    this->m_layout_main->addLayout(this->m_layout_button);
    this->m_bar = new QProgressBar;
    this->m_layout_button->addWidget(this->m_bar);
    this->m_start = new QPushButton;
    this->m_start->setText(tr("Start"));
    this->m_layout_button->addWidget(this->m_start);

    this->m_c_fill_empty = new QCheckBox;
    this->m_c_fill_empty->setText(tr("Fill in missing files"));
    this->m_c_fill_empty->setChecked(false);
    this->m_layout_conf->addWidget(this->m_c_fill_empty);
    this->m_c_replace_type = new QComboBox;
    this->m_c_replace_type->addItem(tr("Replace all"));
    this->m_c_replace_type->addItem(tr("Completes files"));
    //this->m_c_replace_type->addItem(tr("Check MD5"));
    this->m_c_replace_type->setCurrentIndex(0);
    this->m_layout_conf->addWidget(this->m_c_replace_type);
    this->m_layout_conf->addStretch();

    connect(this->m_c_fill_empty, &QCheckBox::clicked, this, [=]{this->fill_empty = this->m_c_fill_empty->isChecked();});
    connect(this->m_c_replace_type, &QComboBox::currentTextChanged, this, [=]{this->replace_type = this->m_c_replace_type->currentIndex();});
    connect(this->m_start, &QPushButton::clicked, this, &Build::start);

    for(int i = 0; i < this->m_types->size(); i++){
        QCheckBox *c = new QCheckBox;
        c->setChecked(true);
        c->setText(this->m_types->at(i));
        this->m_enable_type.insert(this->m_types->at(i), true);
        this->m_layout_type->addWidget(c);
        connect(c, &QCheckBox::clicked, this, [=]{this->m_enable_type.insert(this->m_types->at(i), c->isChecked());});
    }
    this->m_layout_type->addStretch();
    QMap<QString, QString>::Iterator iter = this->m_langs->begin();
    while(iter!=this->m_langs->end()){
        QCheckBox *c = new QCheckBox;
        c->setChecked(true);
        c->setText(iter.value());
        this->m_enable_lang.insert(iter.key(), true);
        this->m_layout_lang->addWidget(c);
        connect(c, &QCheckBox::clicked, this, [=]{this->m_enable_lang.insert(iter.key(), c->isChecked());});
        iter++;
    }
    this->m_layout_lang->addStretch();

    this->m_bar->setMinimum(0);
    this->m_bar->setMaximum(0);
    this->m_bar->setValue(0);
    this->m_bar->reset();
    this->m_bar->setFormat(tr("Waiting"));
}

Build::~Build()
{
    delete ui;
    m_list = nullptr;
    m_files_list = nullptr;
    m_langs = nullptr;
    m_types = nullptr;
    delete this->m_layout_main;
}

void Build::start(){

    if(this->m_c_fill_empty->isChecked()){
        QFile f("./mute.wav");
        if(!f.exists()){
            QMessageBox::warning(this, tr("System File Missing"), tr("Missing system file \"mute.wav\", please reinstall the application to solve the problem"));
            return;
        }
    }

    this->m_start->setDisabled(true);
    this->m_bar->setMinimum(0);
    this->m_bar->setMaximum(this->m_list->size());
    this->m_bar->setValue(0);
    this->m_bar->reset();
    this->m_bar->setFormat(tr("%p%"));

    QString path = QFileDialog::getExistingDirectory(this, tr("Select export directory"), ".");
    if(!path.isEmpty()){
        for(int i = 0; i < this->m_list->size(); i++){
            Item *item = this->m_list->at(i);
            if(!item->is_reday&&!this->fill_empty){
                this->m_bar->setValue(i+1);
                continue;
            }
            QString source;
            if(item->is_reday)
                source = this->m_files_list->value(item->file_name);
            else
                source = "./mute.wav";
            QMap<QString, QMap<QString,QList<QString>>> *mapping = item->m_mapping;
            QMap<QString, QMap<QString,QList<QString>>>::Iterator iterType = mapping->begin();
            while(iterType != mapping->end()){
                if(this->m_enable_type.value(iterType.key())){
                    QMap<QString,QList<QString>>::Iterator iterLang = iterType.value().begin();
                    while(iterLang != iterType.value().end()){
                        if(this->m_enable_lang.value(iterLang.key())){
                            for(int j = 0; j < iterLang.value().size(); j++){
                                QString filePath = QDir(QDir(path).absoluteFilePath(iterLang.key())).absoluteFilePath(iterLang.value().at(j));
                                QFile file(filePath);
                                createMultipleFolders(QFileInfo(filePath).path());
                                if(file.exists()){
                                    if(this->replace_type == 0)
                                    {
                                        file.remove();
                                        QFile::copy(source, filePath);
                                    }
                                }else{
                                    QFile::copy(source, filePath);
                                }
                            }
                        }
                        iterLang++;
                    }
                }
                iterType++;
            }
            this->m_bar->setValue(i+1);
        }
        this->m_bar->setFormat(tr("Done"));
    }else{
        this->m_bar->setFormat(tr("Cancel"));
    }
    this->m_start->setDisabled(false);
}

QString Build::createMultipleFolders(QString path)
{
    QDir dir(path);
    if (dir.exists(path)) {
        return path;
    }

    QString parentDir = createMultipleFolders(path.mid(0, path.lastIndexOf('/')));
    QString dirName = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);
    if (!dirName.isEmpty())
    {
        parentPath.mkpath(dirName);
    }
    return parentDir + "/" + dirName;
}
