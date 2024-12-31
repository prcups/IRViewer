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

#include "irparser.h"
#include <llvm/IR/DebugInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>

void IRParser::run() {
  if (ready) {
    llvm::Function *func = M->getFunction(funcName);
    if (func == nullptr)
      graph = nullptr;
    else
      graph = new ControlFlowGraph(func);
    emit finishFind();
  } else {
    M = llvm::parseIRFile("built-in.bc", Err, Context);
    if (!M)
      emit finishParse(false);
    else
      emit finishParse(true);
    ready = true;
  }
}

void IRParser::SetFunc(const std::string &funcName) {
  this->funcName = funcName;
}

ControlFlowGraph *IRParser::GetGraph() { return graph; }

ControlFlowGraph::ControlFlowGraph(llvm::Function *func) {
  for (auto &b : *func) {
    std::string str;
    llvm::raw_string_ostream stream(str);
    b.print(stream);
    auto node = new BlockNode(&b, QString::fromStdString(str));
    blockList[&b] = node;
  }
  for (auto &i : blockList) {
    for (auto si = succ_begin(i.first), se = succ_end(i.first); si != se;
         ++si) {
      llvm::BasicBlock *succ = *si;
      if (auto succObject = blockList[succ]) {
        AddEdge(i.second, succObject);
      }
    }
  }
}

void ControlFlowGraph::AddEdge(BlockNode *src, BlockNode *dest) {
  BlockEdge *edge = src->edgeSrc, *newEdge = new BlockEdge(src, dest);
  src->edgeSrc = newEdge;
  newEdge->next = edge;

  edge = dest->edgeDest;
  dest->edgeDest = newEdge;
  newEdge->priv = edge;

  edgeList.push_back(newEdge);
}

QRectF BlockNode::boundingRect() const {
  return QRect(0, 0, squareLen, squareLen);
}

QString BlockNode::getInfo() { return info; }

void BlockNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)

  painter->drawRect(boundingRect());
  painter->drawText(boundingRect(), Qt::TextWrapAnywhere, getInfo());
}

void BlockNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  Q_UNUSED(event)
  emit clicked(getInfo());
}

void BlockNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  prepareGeometryChange();
  for (BlockEdge *i = edgeSrc; i; i = i->next) {
    i->prepare();
  }
  for (BlockEdge *i = edgeDest; i; i = i->priv) {
    i->prepare();
  }
  setPos(event->scenePos() - event->lastPos());
}

void BlockEdge::prepare() { prepareGeometryChange(); }

QRectF BlockEdge::boundingRect() const {
  qreal minx =
            src->pos().x() < dest->pos().x() ? src->pos().x() : dest->pos().x(),
        miny =
            src->pos().y() < dest->pos().y() ? src->pos().y() : dest->pos().y(),
        maxx =
            src->pos().x() > dest->pos().x() ? src->pos().x() : dest->pos().x(),
        maxy =
            src->pos().y() > dest->pos().y() ? src->pos().y() : dest->pos().y();
  return QRectF(minx, miny, maxx + squareLen - minx, maxy + squareLen - miny);
}

void BlockEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)

  painter->drawLine(src->pos().x() + squareLen, src->pos().y() + squareLen,
                    dest->pos().x(), dest->pos().y());
}
