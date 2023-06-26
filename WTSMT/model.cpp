#include "model.h"
#include "ui_model.h"

Model::Model(QWidget *parent, QJsonObject obj, QMap<QString, QString> *filesList, int index) :
    QWidget(parent),
    ui(new Ui::Model)
{
    qDebug()<<"\n=== Creating model ===";
    qDebug()<<"Init ui";
    ui->setupUi(this);
    // 关闭窗口时销毁
    setAttribute(Qt::WA_DeleteOnClose);

    // 接收数据
    this->index = index;
    this->m_obj = obj;
    this->m_files_list = filesList;

    // 初始化
    this->m_types = new QList<QString>;
    this->m_langs = new QMap<QString, QString>;
    this->m_list = new QList<Item*>;

    // 显示
    this->show();

    // 创建界面
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

    // 解析模型
    qDebug()<<"Analysing model:"<<obj.value("name").toString();
    // 解析类型
    qDebug()<<"[1/3] Type";

    this->setWindowTitle(tr("Model:") + " " + this->m_name + " | " + tr("Progress:") + " 1/3 ");
    QJsonArray types = this->m_obj.value("types").toArray();
    for(int i = 0; i < types.size(); i ++){
        this->m_types->append(types.at(i).toString());
    }
    // 解析语种
    qDebug()<<"[2/3] Language";
    this->setWindowTitle(tr("Model:") + " " + this->m_name + " | " + tr("Progress:") + " 2/3 ");
    QJsonArray langs = this->m_obj.value("langs").toArray();
    for(int i = 0; i < langs.size(); i ++){
        QJsonObject o = langs.at(i).toObject();
        this->m_langs->insert(o.value("id").toString(), o.value("name").toString());
    }
    this->m_langs->remove("");
    // 解析条目
    qDebug()<<"[3/3] Item";
    QJsonArray array = this->m_obj.value("items").toArray(QJsonArray());
    QJsonArray::iterator i = array.begin();
    int size = array.size();
    while(i != array.end()){
        // 创建新条目
        Item *item = new Item(nullptr, i->toObject(), this->m_langs, this->m_types);
        this->m_list->append(item);
        this->m_list_mapping.insert(item->file_name, this->m_list->size() - 1);
        QListWidgetItem *listItem = new QListWidgetItem();
        listItem->setText(item->m_cap);
        listItem->setBackground(QColor("red"));
        this->m_list_widget->addItem(listItem);
        i++;
        this->setWindowTitle(tr("Model:") + " " + this->m_name + " | " + tr("Progress:") + " 3/3 " + QString::number(this->m_list->size()) + "/" + QString::number(size));
        qDebug()<<"Item Process"<<this->m_list->size()<<"/"<<size;
    }
    this->setWindowTitle(tr("Model:") + " " + this->m_name);
    qDebug()<<"Analysing done";
    // 重置索引
    this->m_stack->setCurrentIndex(0);
    // 连接信号
    qDebug()<<"Init connection";
    connect(this->m_list_widget, &QListWidget::currentRowChanged, this, [=]{if(!this->m_stack->isEmpty()){this->m_stack->removeWidget(this->m_stack->currentWidget());}this->m_stack->addWidget(this->m_list->at(this->m_list_widget->currentRow()));});
    connect(this->m_build, &QPushButton::clicked, this, [=]{Build *b = new Build(nullptr, this->m_files_list, this->m_langs, this->m_types, this->m_list, this->m_name); b->show();});
    // 置顶窗口
    this->activateWindow();
    this->setWindowState(this->windowState() & Qt::WindowActive);
    qDebug()<<"=== Create model done:"<<obj.value("name").toString()<<"===";
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
    qDebug()<<"\n=== Checking files, type"<<type<<"===";
    // 修改标题
    this->setWindowTitle(tr("Model:") + " " + this->m_name + " | " + tr("Checking Files..."));
    // 检查所有条目
    if(type == 0){
        for(int i = 0; i < this->m_list->size(); i++)
        {
            this->m_list->at(i)->is_reday = this->m_files_list->contains(this->m_list->at(i)->file_name);
            if(this->m_list->at(i)->is_reday){
                this->m_list_widget->item(i)->setBackground(QColor("white"));
            }else{
                this->m_list_widget->item(i)->setBackground(QColor(255,0,0,100));
            }
        }
    }
    // 重置所有条目
    else if(type == -1){
        for(int i = 0; i < this->m_list->size(); i++)
        {
            this->m_list->at(i)->is_reday = false;
            this->m_list_widget->item(i)->setBackground(QColor(255,0,0,100));
        }
    }
    else{
        // 筛选条目
        qDebug()<<"Filtering"<<type;
        QList<int> workingList;
        for(int i = 0; i < l.size(); i++){
            if(this->m_list_mapping.contains(l.at(i)))
                workingList.append(this->m_list_mapping.value(l.at(i)));
        }
        // 检查指定条目
        if(type == 1){
            for(int i = 0; i < workingList.size(); i++)
            {
                this->m_list->at((workingList.at(i)))->is_reday = true;
                this->m_list_widget->item(workingList.at(i))->setBackground(QColor("white"));
            }
        }
        // 重置指定条目
        else if(type == 2){
            for(int i = 0; i < workingList.size(); i++)
            {
                this->m_list_widget->setCurrentRow(workingList.at(i));
                this->m_list->at((workingList.at(i)))->is_reday = false;
                this->m_list_widget->item(workingList.at(i))->setBackground(QColor(255,0,0,100));
            }
        }
    }
    // 重置索引
    if(!this->m_list_widget->size().isEmpty())
        this->m_list_widget->setCurrentRow(0);
    // 重置标题
    this->setWindowTitle(tr("Model:") + " " + this->m_name);
    qDebug()<<"=== Checking done ===";
}
