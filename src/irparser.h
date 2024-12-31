// The contents of this file are subject to the Mozilla Public License
// Version 1.1 (the "License"); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://www.mozilla.org/MPL/
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
// License for the specific language governing rights and limitations
// under the License.
//
// The Original Code is ______________________________________.
//
// The Initial Developer of the Original Code is ________________________.
// Portions created by ______________________ are Copyright (C) ______
// _______________________. All Rights Reserved.
//
// Contributor(s): ______________________________________.
//
// Alternatively, the contents of this file may be used under the terms
// of the _____ license (the  "[___] License"), in which case the
// provisions of [______] License are applicable instead of those
// above.  If you wish to allow use of your version of this file only
// under the terms of the [____] License and not to allow others to use
// your version of this file under the MPL, indicate your decision by
// deleting  the provisions above and replace  them with the notice and
// other provisions required by the [___] License.  If you do not delete
// the provisions above, a recipient may use your version of this file
// under either the MPL or the [___] License."

#ifndef IRPARSER_H
#define IRPARSER_H

#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/ToolOutputFile.h>

#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Utils.h>

#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Bitcode/BitcodeWriter.h>

#include <map>
#include <memory>
#include <string>

#include <QDebug>
#include <QGraphicsObject>
#include <QGraphicsSceneDragDropEvent>
#include <QList>
#include <QPainter>
#include <QThread>

static const int squareLen = 100;

class BlockEdge;

class BlockNode : public QGraphicsObject {
  Q_OBJECT
  QString info;

public:
  BlockNode(llvm::BasicBlock *block, const QString &info)
      : block(block), info(info) {}
  llvm::BasicBlock *block;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  QString getInfo();
  BlockEdge *edgeSrc = nullptr;
  BlockEdge *edgeDest = nullptr;
  QRectF boundingRect() const override;

signals:
  void clicked(const QString &info);
};
class BlockEdge : public QGraphicsObject {
public:
  BlockEdge(BlockNode *src, BlockNode *dest) : src(src), dest(dest) {}
  BlockNode *dest;
  BlockNode *src;
  BlockEdge *next;
  BlockEdge *priv;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;
  QRectF boundingRect() const override;
  void prepare();
};

struct ControlFlowGraph {
  std::map<llvm::BasicBlock *, BlockNode *> blockList;
  QList<BlockEdge *> edgeList;
  ControlFlowGraph(llvm::Function *func);
  void AddEdge(BlockNode *src, BlockNode *dest);
};

class IRParser : public QThread {
  Q_OBJECT
  bool ready = false;
  void run() override;
  llvm::LLVMContext Context;
  llvm::SMDiagnostic Err;
  std::string funcName;
  std::unique_ptr<llvm::Module> M;
  ControlFlowGraph *graph;

public:
  void SetFunc(const std::string &funcName);
  ControlFlowGraph *GetGraph();
signals:
  void finishParse(bool ok);
  void finishFind();
};

#endif // IRPARSER_H
