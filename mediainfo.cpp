#include "mediainfo.h"
#include "ui_mediainfo.h"
#include <QDebug>
MediaInfo::MediaInfo(QWidget *parent) :
  AbstractMediaInfo(parent),
  ui(new Ui::MediaInfo){
  ui->setupUi(this);
  model= new QStandardItemModel(1,2);
  model->setHorizontalHeaderItem(0,new QStandardItem(QString("Propriedade")));
  model->setHorizontalHeaderItem(1,new QStandardItem(QString("Valor")));
  ui->tableView->verticalHeader()->setVisible(false);
  ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
  ui->tableView->horizontalHeader()->setStretchLastSection(true);
  ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableView->setAlternatingRowColors(true);
  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableView->setModel(model);
  ui->tableView->setFont(QFont("Times"));
}

MediaInfo::~MediaInfo(){
  delete ui;
}

void MediaInfo::setAtribute(QString property, QString value){
  QList <QStandardItem*> list;
  QStandardItem *item;
  list = model->findItems(property);
  if(!list.empty()){
    int row;
    row = list.at(0)->row();
    model->item(row,0)->setText(property);
    model->item(row,1)->setText(value);
  }
  else{
    int count;
    count = model->rowCount();
    count++;
    item = new QStandardItem(property);
    model->setItem(count,0,item);
    item = new QStandardItem(value);
    model->setItem(count,1,item);
    model->sortRole();
    model->sort(0);
  }
}

void MediaInfo::loadArt(void){

}
