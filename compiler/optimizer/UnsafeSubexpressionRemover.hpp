/*******************************************************************************
 * Copyright IBM Corp. and others 2014
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] https://openjdk.org/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
 *******************************************************************************/

#ifndef UNSAFESUBEXPRESSIONREMOVER_INCL
#define UNSAFESUBEXPRESSIONREMOVER_INCL

#include "compile/Compilation.hpp"
#include "infra/BitVector.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "optimizer/Optimization.hpp"
#include "optimizer/Optimization_inlines.hpp"

namespace TR { class TreeTop; }

namespace OMR
   {
   struct UnsafeSubexpressionRemover
      {
      TR::Optimization *_opt;
      TR_BitVector _visitedNodes;
      TR_BitVector _unsafeNodes; // Nodes whose mere evaluation is not safe

      UnsafeSubexpressionRemoval(TR::Optimization *opt):
         _opt(opt),
         _visitedNodes(opt->comp()->getNodeCount(), comp()->trMemory(), stackAlloc),
         _unsafeNodes (opt->comp()->getNodeCount(), comp()->trMemory(), stackAlloc)
         {}

      TR::Compilation *comp() { return _opt->comp(); }

      bool trace() { return _opt->trace(); }

      bool isVisited(TR::Node *node) { return  _visitedNodes.isSet(node->getGlobalIndex()); }

      bool isUnsafe(TR::Node *node)
         {
         TR_ASSERT(isVisited(node), "Cannot call isUnsafe on n%dn before anchorSafeChildrenOfUnsafeNodes", node->getGlobalIndex());
         return _unsafeNodes.isSet(node->getGlobalIndex());
         }

      void recordDeadUse(TR::Node *node)
         {
         _visitedNodes.set(node->getGlobalIndex());
         _unsafeNodes .set(node->getGlobalIndex());
         }

      void anchorSafeChildrenOfUnsafeNodes(TR::Node *node, TR::TreeTop *anchorPoint);

      bool anchorIfSafe(TR::Node *node, TR::TreeTop *anchorPoint);

      void eliminateStore(TR::TreeTop *treeTop);
      };
   }
#endif
