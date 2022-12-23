//===-- Frequent Path Loop Invariant Code Motion Pass
//------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// EECS583 F22 - This pass can be used as a template for your Frequent Path LICM
//               homework assignment. The pass gets registered as "fplicm".
//
// This pass performs loop invariant code motion, attempting to remove as much
// code from the body of a loop as possible.  It does this by either hoisting
// code into the preheader block, or by sinking code to the exit blocks if it is
// safe.
//
////===----------------------------------------------------------------------===//
#include "llvm/ADT/StringRef.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopIterator.h"
#include "llvm/Analysis/LoopPass.h" //added
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"//added
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
//#include "polly/PolyhedralInfo.h"
#include <map>
/* *******Implementation Starts Here******* */
// include necessary header files
/* *******Implementation Ends Here******* */

using namespace llvm;

#define DEBUG_TYPE "fplicm"

namespace Correctness {
struct FPLICMPass : public LoopPass {
  static char ID;
  FPLICMPass() : LoopPass(ID) {}

  bool runOnLoop(Loop *L, LPPassManager &LPM) override {
    bool Changed = true;
    // LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().();

    /* *******Implementation Starts Here******* */
    // create a map
    std ::map<BasicBlock *, int> frequentblock;
    // std :: map <BasicBlock*, int> :: iterator itr;
    BlockFrequencyInfo &bfi =
        getAnalysis<BlockFrequencyInfoWrapperPass>().getBFI();
    BranchProbabilityInfo &bpi =
        getAnalysis<BranchProbabilityInfoWrapperPass>().getBPI();
    LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
    BasicBlock *header = L->getHeader();
    BasicBlock *current = header;
    // get the preheader
    BasicBlock *preheader = L->getLoopPreheader();
    // get the terminator of the loop preheader basic block
    // Instruction* preheader_terminator_inst = preheader -> getTerminator();
    frequentblock.insert({header, 1});
    int count;
    while (!(L->isLoopLatch(current)) && count != 0) {
      count = 0;
      for (BasicBlock *BasicBlockSuccessor : successors(current)) {
        if (bpi.getEdgeProbability(current, BasicBlockSuccessor) >=
            BranchProbability(4, 5)) {
          count++;
          current = BasicBlockSuccessor;
          frequentblock.insert({current, 1});
          break;
        }
      }
    }

    for (BasicBlock *BB : L->getBlocks()) {
      errs() << "\nEnter block\n";
      int c = 0;
      // iterate only through the frequent basic blocks and check for load
      // instructions
      if (frequentblock.find(BB) != frequentblock.end()) {
        // iterate through the instructions of the frequent basic block
        errs() << "\n found a frequent BB\n";
        for (BasicBlock ::iterator i = BB->begin(); i != BB->end(); i++) {
          // int flag = 0;
          // check if instruction is a load
          errs() << "\n   Instruction found\n";
          if (i->getOpcode() == Instruction ::Load) {
            errs() << "\n     Instruction is a load\n";
            if ((((i->getOperand(0))->getName()) == "j") ||
                (((i->getOperand(0))->getName()) == "k")) {
              // iterate through all the users of the present load instruction
              errs() << "\n       Instruction is a hoistable load\n";
              Value *loadOp = i->getOperand(0);
              int flag = 0;
              for (auto U : loadOp->users()) {
                std ::string userOp_name =
                    (dyn_cast<Instruction>(U))->getOpcodeName();
                errs() << "\n" << userOp_name << "\n";
                errs() << "\n           iterating through users for flag\n";
                // Check if the user is a store in frequent basic block
                if ((userOp_name == "store")) {
                  errs() << "\nStore is found among users\n";
                  // check if store belongs to frequent block
                  if (frequentblock.find(
                          (dyn_cast<Instruction>(U))->getParent()) !=
                      frequentblock.end()) {
                    errs() << "\nThis store belongs to the frequent path\n";
                    // update the flag and break
                    flag++;
                    break;
                  }
                }
              }

              // After the flag is set to 1 or remains zero run another for loop
              // only if the flag is 0
              errs() << "\nFlag:" << flag << "\n";
              if (flag == 0) {
                // errs() << "\nenter new era\n";
                Value *Loadsourceop = i->getOperand(0);
                for (auto U_se : Loadsourceop->users()) {
                  std ::string userOp_name2 =
                      (dyn_cast<Instruction>(U_se))->getOpcodeName();
                  if (userOp_name2 == "store") {
                    // errs() << "\nhahahahahah\n";
                    // if there is dependency
                    if ((dyn_cast<Instruction>(U_se))->getOperand(1) ==
                        i->getOperand(0)) {
                      // auto new_inst = i -> clone();
                      errs() << "\npresent load instruction before copying and "
                                "hoisting:"
                             << *i << "\n";
                      Value *load_value = i->getOperand(0);
                      // insert alloc instruction
                      AllocaInst *var = new AllocaInst(
                          dyn_cast<LoadInst>(i)->getType(), 0, nullptr,
                          dyn_cast<LoadInst>(i)->getAlign(), "var",
                          (&*((preheader->rbegin()))));
                      // insert load instruction
                      LoadInst *LD = new LoadInst(i->getType(), load_value, "",
                                                  &*((preheader->rbegin())));
                      // insert store instruction
                      StoreInst *ST =
                          new StoreInst(LD, var, &*((preheader->rbegin())));
                      // replace all uses with
                      load_value->replaceUsesOutsideBlock(var, preheader);
                      // errs() << "\npresent load instruction after copying and
                      // hoisting:" << *i << "\n"; print all the hoisted and
                      // created instructions in the preheader block:
                      errs() << "\n"
                             << *(++(++(++(preheader->rbegin())))) << "\n";
                      errs() << "\n" << *(++(++(preheader->rbegin()))) << "\n";
                      errs() << "\n" << *(++(preheader->rbegin())) << "\n";
                      errs() << "\n" << *((preheader->rbegin())) << "\n";
                      break;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*errs() << "\n=============================================================="
              "===============================\n";
    errs() << "\nprint everything\n";
    BasicBlock *printBB = L->getLoopPreheader();
    errs() << "\n\n" << printBB->getName();
    for (BasicBlock::iterator iter = printBB->begin(); iter != printBB->end();
         iter++) {
      errs() << "\n\t" << *iter;
    }
    for (Loop *L : LI) {
      for (BasicBlock *BB : L->getBlocks()) {
        errs() << "\n\n" << BB->getName();
        for (BasicBlock::iterator instrIter = BB->begin();
             instrIter != BB->end(); instrIter++) {
          errs() << "\n\t" << *instrIter;
        }
      }
    }
    errs() << "\n\n";*/
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* *******Implementation Ends Here******* */

    return Changed;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<BranchProbabilityInfoWrapperPass>();
    AU.addRequired<BlockFrequencyInfoWrapperPass>();
    AU.addRequired<LoopInfoWrapperPass>();
  }

private:
  /// Little predicate that returns true if the specified basic block is in
  /// a subloop of the current one, not the current one itself.
  bool inSubLoop(BasicBlock *BB, Loop *CurLoop, LoopInfo *LI) {
    assert(CurLoop->contains(BB) && "Only valid if BB is IN the loop");
    return LI->getLoopFor(BB) != CurLoop;
  }
};
} // end of namespace Correctness

char Correctness::FPLICMPass::ID = 0;
static RegisterPass<Correctness::FPLICMPass>
    X("fplicm-correctness",
      "Frequent Loop Invariant Code Motion for correctness test", false, false);























namespace Performance {
struct FPLICMPass : public LoopPass {
  static char ID;
  FPLICMPass() : LoopPass(ID) {}

  bool runOnLoop(Loop *L, LPPassManager &LPM) override {
    bool Changed = false;

    /* *******Implementation Starts Here******* */

    /* *******Implementation Ends Here******* */

    return Changed;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<BranchProbabilityInfoWrapperPass>();
    AU.addRequired<BlockFrequencyInfoWrapperPass>();
    AU.addRequired<LoopInfoWrapperPass>();
  }

private:
  /// Little predicate that returns true if the specified basic block is in
  /// a subloop of the current one, not the current one itself.
  bool inSubLoop(BasicBlock *BB, Loop *CurLoop, LoopInfo *LI) {
    assert(CurLoop->contains(BB) && "Only valid if BB is IN the loop");
    return LI->getLoopFor(BB) != CurLoop;
  }
};
} // end of namespace Performance

char Performance::FPLICMPass::ID = 0;
static RegisterPass<Performance::FPLICMPass>
    Y("fplicm-performance",
      "Frequent Loop Invariant Code Motion for performance test", false, false);
