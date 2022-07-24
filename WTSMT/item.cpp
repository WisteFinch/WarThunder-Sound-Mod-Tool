#include "item.h"
#include "ui_item.h"

Item::Item(QWidget *parent, QJsonObject obj, QMap<QString, QString> *langsList, QList<QString> *typesList) :
    QScrollArea(parent),
    ui(new Ui::Item)
{
    ui->setupUi(this);

    this->m_obj = obj;
    this->m_mapping = new QMap<QString, QMap<QString, QList<QString>>>;
    this->m_types = typesList;
    this->m_langs = langsList;

    this->file_name = this->m_obj.value("file_name").toString();

    this->setWidgetResizable(true);
    this->m_widget = new QWidget;
    this->setWidget(this->m_widget);
    this->m_layout = new QVBoxLayout;
    this->m_widget->setLayout(this->m_layout);
    this->m_l_id = new QHBoxLayout;
    this->m_id = new QLabel;
    this->m_id_edit = new QLineEdit;
    this->m_id->setText(tr("id"));
    this->m_id_edit->setText(obj.value("id").toString());
    this->m_l_id->addWidget(this->m_id);
    this->m_l_id->addWidget(this->m_id_edit);
    this->m_layout->addLayout(this->m_l_id);
    this->m_l_name = new QHBoxLayout;
    this->m_name = new QLabel;
    this->m_name_edit = new QLineEdit;
    this->m_name->setText(tr("Name"));
    this->m_name_edit->setText(obj.value("name").toString());
    this->m_l_name->addWidget(this->m_name);
    this->m_l_name->addWidget(this->m_name_edit);
    this->m_layout->addLayout(this->m_l_name);
    this->m_l_file = new QHBoxLayout;
    this->m_file = new QLabel;
    this->m_file_edit = new QLineEdit;
    this->m_file->setText(tr("File Name"));
    this->m_file_edit->setText(this->file_name);
    this->m_l_file->addWidget(this->m_file);
    this->m_l_file->addWidget(this->m_file_edit);
    this->m_layout->addLayout(this->m_l_file);
    this->m_l_t_mapping = new QHBoxLayout;
    this->m_t_mapping = new QLabel;
    this->m_l_t_mapping->addWidget(this->m_t_mapping);
    this->m_t_mapping->setText(tr("Mapping"));
    this->m_l_t_mapping->addStretch();
    this->m_layout->addLayout(this->m_l_t_mapping);
    this->m_l_mapping = new QVBoxLayout;
    this->m_layout->addLayout(this->m_l_mapping);
    this->m_l_mapping->setContentsMargins(10,0,0,0);


    this->m_id_edit->setDisabled(true);
    this->m_name_edit->setDisabled(true);
    this->m_file_edit->setDisabled(true);

    this->m_cap = obj.value("name").toString().isEmpty()? obj.value("id").toString():obj.value("name").toString();

    QJsonArray types = this->m_obj.value("mapping").toArray();
    bool hasTypes = false;
    for(int i = 0; i < types.size(); i++){
        QJsonObject type = types.at(i).toObject();
        if(this->m_types->contains(type.value("type").toString())){
            hasTypes = true;

            QHBoxLayout *layoutType = new QHBoxLayout;
            this->m_l_mapping->addLayout(layoutType);
            QLabel *labelType = new QLabel;
            labelType->setText(tr("Type"));
            layoutType->addWidget(labelType);
            QLineEdit *editType = new QLineEdit;
            editType->setText(type.value("type").toString());
            layoutType->addWidget(editType);
            editType->setDisabled(true);
            layoutType->addStretch();

            QJsonArray langs = type.value("langs").toArray();
            bool hasLangs = false;
            QVBoxLayout *layoutLangs = new QVBoxLayout;
            layoutLangs->setContentsMargins(10,0,0,0);
            this->m_l_mapping->addLayout(layoutLangs);
            QMap<QString, QList<QString>> langData;
            for(int j = 0; j < langs.size(); j++){
                QJsonObject lang = langs.at(j).toObject();
                if(this->m_langs->contains(lang.value("lang").toString())){
                    hasLangs = true;

                    QHBoxLayout *layoutLang = new QHBoxLayout;
                    layoutLangs->addLayout(layoutLang);
                    QLabel *labelLang = new QLabel;
                    labelLang->setText(tr("Language"));
                    layoutLang->addWidget(labelLang);
                    QLineEdit *editLang = new QLineEdit;
                    editLang->setText(lang.value("lang").toString());
                    layoutLang->addWidget(editLang);
                    editLang->setDisabled(true);
                    layoutLang->addStretch();

                    QVBoxLayout *layoutList = new QVBoxLayout;
                    layoutList->setContentsMargins(10,0,0,0);
                    layoutLangs->addLayout(layoutList);
                    QJsonArray list = lang.value("list").toArray();
                    QList<QString> listData;
                    if(list.empty()){
                        QLabel *l = new QLabel(tr("None"));
                        l->setStyleSheet("color:grey");
                        layoutList->addWidget(l);
                    }else{
                        for(int k = 0; k < list.size(); k++){
                            QLineEdit *edit = new QLineEdit;
                            layoutList->addWidget(edit);
                            edit->setText(list.at(k).toString());
                            listData.append(list.at(k).toString());
                            edit->setDisabled(true);
                        }
                    }
                    langData.insert(lang.value("lang").toString(), listData);
                }
            }
            if(!hasLangs){
                QLabel *l = new QLabel(tr("None"));
                l->setStyleSheet("color:grey");
                layoutLangs->addWidget(l);
            }
            this->m_mapping->insert(type.value("type").toString(), langData);
        }
    }
    if(!hasTypes){
        QLabel *l = new QLabel(tr("None"));
        l->setStyleSheet("color:grey");
        this->m_l_mapping->addWidget(l);
    }

    this->m_layout->addStretch();
}

Item::~Item()
{
    delete ui;
    m_langs = nullptr;
    m_types = nullptr;
    delete this->m_widget;
    delete m_mapping;
}
