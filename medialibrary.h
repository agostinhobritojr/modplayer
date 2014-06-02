#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QStack>
#include <QDir>
#include <QMediaPlayer>
#include <QString>

namespace Ui {
class MediaLibrary;
}

class MediaLibrary : public QWidget
{
  Q_OBJECT

public:
  explicit MediaLibrary(QWidget *parent = 0);
  ~MediaLibrary();
  void searchPath(QString path);
private:
  Ui::MediaLibrary *ui;
  QMediaPlayer *auxplayer;
public slots:
  void onAddFolder(QString folder);
};

#endif // PLAYLIST_H
