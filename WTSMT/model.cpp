#include "model.h"
#include "ui_model.h"

Model::Model(QWidget *parent, QJsonObject obj, QMap<QString, QString> *filesList, int index) :
    QWidget(parent),
    ui(new Ui::Model)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    this->index = index;

    this->m_obj = obj;
    this->m_files_list = filesList;

    this->m_types = new QList<QString>;
    this->m_langs = new QMap<QString, QString>;
    this->m_list = new QList<Item*>;

    this->show();

    this->m_layout_main = new QHBoxLayout;
    this->m_list_widget = new QListWidget;
    this->m_layout_list = new QVBoxLayout;
    this->m_layout_button = new QHBoxLayout;
    this->m_build = new QPushButton;
    this->m_build->setText(tr("Build"));
    this->m_stack = new QStackedLayout;

    this->setLayout(this->m_layout_main);
    this->m_layout_main->addLayout(this->m_layout_list);
    this->m_layout_list->addWidget(this->m_list_widget);
    this->m_layout_list->addLayout(this->m_layout_button);
    this->m_layout_button->addWidget(this->m_build);
    this->m_layout_button->addStretch();
    this->m_layout_main->addLayout(this->m_stack);

    this->m_name = obj.value("name").toString();
    this->setWindowTitle(tr("Model:") + " " + this->m_name);

    QJsonArray types = this->m_obj.value("types").toArray();
    for(int i = 0; i < types.size(); i ++){
        this->m_types->append(types.at(i).toString());
    }

    QJsonArray langs = this->m_obj.value("langs").toArray();
    for(int i = 0; i < langs.size(); i ++){
        QJsonObject o = langs.at(i).toObject();
        this->m_langs->insert(o.value("id").toString(), o.value("name").toString());
    }
    this->m_langs->remove("");

    QJsonArray array = this->m_obj.value("items").toArray(QJsonArray());
    QJsonArray::iterator i = array.begin();
    while(i != array.end()){
        Item *item = new Item(nullptr, i->toObject(), this->m_langs, this->m_types);
        i++;
        this->m_list->append(item);
        this->m_list_mapping.insert(item->file_name, this->m_list->size() - 1);
        QListWidgetItem *listItem = new QListWidgetItem();
        listItem->setText(item->m_cap);
        listItem->setBackground(QColor("red"));
        this->m_list_widget->addItem(listItem);
        this->m_stack->addWidget(item);
    }

    this->m_stack->setCurrentIndex(0);

    connect(this->m_list_widget, &QListWidget::currentRowChanged, this, [=]{this->m_stack->setCurrentIndex(this->m_list_widget->currentRow());});
    connect(this->m_build, &QPushButton::clicked, this, [=]{Build *b = new Build(nullptr, this->m_files_list, this->m_langs, this->m_types, this->m_list, this->m_name); b->show();});

    this->activateWindow();
    this->setWindowState(this->windowState() & Qt::WindowActive);
}

Model::~Model()
{
    delete ui;
    m_files_list = nullptr;
    while(!this->m_list->isEmpty()){
        delete this->m_list->last();
        this->m_list->pop_back();
    }
    delete this->m_list;
    delete this->m_langs;
    delete this->m_types;
    delete m_layout_main;
    emit widgetClose(this->index);
}

void Model::checkFiles(int type)
{
    QStringList l;
    checkFiles(type, l);
}

void Model::checkFiles(int type, QStringList l)
{
    this->setWindowTitle(tr("Model:") + " " + this->m_name + " | " + tr("Checking Files..."));
    QList<int> workingList;
    for(int i = 0; i < l.size(); i++){
        if(this->m_list_mapping.contains(l.at(i)))
            workingList.append(this->m_list_mapping.value(l.at(i)));
    }
    if(type == 0){
        for(int i = 0; i < this->m_list->size(); i++)
        {
            this->m_list_widget->setCurrentRow(i);
            this->m_list->at(i)->is_reday = this->m_files_list->contains(this->m_list->at(i)->file_name);
            if(this->m_list->at(i)->is_reday){
                this->m_list_widget->currentItem()->setBackground(QColor("white"));
            }else{
                this->m_list_widget->currentItem()->setBackground(QColor("red"));
            }
        }
    }
    else if(type == -1){
        for(int i = 0; i < this->m_list->size(); i++)
        {
            this->m_list_widget->setCurrentRow(i);
            this->m_list->at(i)->is_reday = false;
            this->m_list_widget->currentItem()->setBackground(QColor("red"));
        }
    }
    else if(type == 1){
        for(int i = 0; i < workingList.size(); i++)
        {
            this->m_list_widget->setCurrentRow(workingList.at(i));
            this->m_list->at((workingList.at(i)))->is_reday = true;
            this->m_list_widget->currentItem()->setBackground(QColor("white"));
        }
    }
    else if(type == 2){
        for(int i = 0; i < workingList.size(); i++)
        {
            this->m_list_widget->setCurrentRow(workingList.at(i));
            this->m_list->at((workingList.at(i)))->is_reday = false;
            this->m_list_widget->currentItem()->setBackground(QColor("red"));
        }
    }
    if(!this->m_list_widget->size().isEmpty())
        this->m_list_widget->setCurrentRow(0);
    this->setWindowTitle(tr("Model:") + " " + this->m_name);
}
