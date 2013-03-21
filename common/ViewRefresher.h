#include <string>
#include <QtDeclarative/QDeclarativeView>
#include <QObject>
#include <QMap>

using namespace std;

class ViewRefresher : public QObject {
  Q_OBJECT

public:
  void addWatch(QDeclarativeView *view, QString layout) {
    views[layout] = view;
  }
  void refreshAll();

public slots:
  void refreshView(QDeclarativeView* view, const QUrl &url);

private:
  QMap<QString, QDeclarativeView*> views;
};
