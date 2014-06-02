#include "medialibrary.h"
#include "ui_medialibrary.h"

MediaLibrary::MediaLibrary(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MediaLibrary)
{
  ui->setupUi(this);
}

MediaLibrary::~MediaLibrary()
{
  delete ui;
}

void MediaLibrary::onAddFolder(QString folder){
  searchPath(folder);
}

void MediaLibrary::searchPath(QString path)
{
/*  QFileSystemModel *model;
  path = "/home/ambj";
  model = new QFileSystemModel;
  model->setRootPath(path);
  //model->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
//  view.setModel(model);
  ui->treeViewLibrary->setModel(model);
  ui->treeViewLibrary->setRootIndex(model->index(path));
  */
  QDir dir;
  dir.setPath(path);
  QStack<QString> stack;
  stack.push(dir.absolutePath());
  qDebug() << "foi";
  while (!stack.isEmpty()) {
    QString sSubdir = stack.pop();
    QDir subdir(sSubdir);

    // Check for the files.
    QStringList entries = subdir.entryList(QStringList() << "*mp3" << "*flac", QDir::Files);
    for (int i = 0; i < entries.size(); i++) {
        qDebug("Found %s matching pattern.", qPrintable(entries[i]));
    }

    QFileInfoList infoEntries = subdir.entryInfoList(QStringList(),
                                                     QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    for (int i = 0; i < infoEntries.size(); i++) {
      QFileInfo& item = infoEntries[i];
      stack.push(item.absoluteFilePath());
    }
  }
}
