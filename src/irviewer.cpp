#include "irviewer.h"
#include "ui_irviewer.h"

IRViewer::IRViewer(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::IRViewer) {
  m_ui->setupUi(this);
  connect(m_ui->pushButton, &QPushButton::clicked, this,
          &IRViewer::handleSearch);
  parser.reset(new IRParser);
  connect(parser.get(), &IRParser::finishParse, this, &IRViewer::handleParse);
  connect(parser.get(), &IRParser::finishFind, this, &IRViewer::handleFind);
  parser->start();
}

IRViewer::~IRViewer() = default;

void IRViewer::handleFind() {
  int pos = 0;
  m_ui->pushButton->setEnabled(true);
  ControlFlowGraph *newGraph = parser->GetGraph();
  if (!newGraph) {
    m_ui->plainTextEdit->setPlainText(tr("未找到函数"));
    return;
  }
  scene.reset(new QGraphicsScene);
  m_ui->graphicsView->setScene(scene.get());
  graph.reset(newGraph);
  for (auto &i : graph->blockList) {
    scene->addItem(i.second);
    connect(i.second, &BlockNode::clicked, m_ui->plainTextEdit,
            &QPlainTextEdit::setPlainText);
    i.second->setPos(pos * (squareLen + 10), pos * (squareLen + 10));
    ++pos;
  }
  for (auto &i : graph->edgeList) {
    scene->addItem(i);
  }
  m_ui->plainTextEdit->setPlainText(tr("生成完成"));
}

void IRViewer::handleParse(bool ok) {
  if (ok) {
    m_ui->plainTextEdit->setPlainText(tr("准备就绪"));
    m_ui->pushButton->setEnabled(true);
  } else {
    m_ui->plainTextEdit->setPlainText(tr("发生错误"));
  }
}

void IRViewer::handleSearch() {
  parser->SetFunc(m_ui->lineEdit->text().toStdString());
  parser->start();
  m_ui->pushButton->setEnabled(false);
  m_ui->plainTextEdit->setPlainText(tr("正在查找..."));
}
