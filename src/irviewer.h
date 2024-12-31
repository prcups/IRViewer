#ifndef IRVIEWER_H
#define IRVIEWER_H

#include "irparser.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QRandomGenerator>
#include <QScopedPointer>
#include <QString>
#include <QThread>

namespace Ui {
class IRViewer;
}

class IRViewer : public QMainWindow {
  Q_OBJECT

public:
  explicit IRViewer(QWidget *parent = nullptr);
  ~IRViewer() override;

private:
  QScopedPointer<Ui::IRViewer> m_ui;
  QScopedPointer<IRParser> parser;
  QScopedPointer<QGraphicsScene> scene;
  QScopedPointer<ControlFlowGraph> graph;
  void handleSearch();
private slots:
  void handleParse(bool ok);
  void handleFind();
};

#endif // IRVIEWER_H
