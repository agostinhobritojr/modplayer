#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractItemModel>
#include <QMediaPlaylist>

class PlaylistModel : public QAbstractItemModel
{
  Q_OBJECT
private:
  QMediaPlaylist *m_playlist;
  QMap<QModelIndex, QVariant> m_data;
public:
  explicit PlaylistModel(QObject *parent = 0);
  enum Column {
      Title = 0,
      ColumnCount
  };

signals:

public slots:
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QMediaPlaylist *playlist() const;
  void setPlaylist(QMediaPlaylist *playlist);
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);

private slots:
  void beginInsertItems(int start, int end);
  void endInsertItems();
  void beginRemoveItems(int start, int end);
  void endRemoveItems();
  void changeItems(int start, int end);

};

#endif // PLAYLISTMODEL_H
