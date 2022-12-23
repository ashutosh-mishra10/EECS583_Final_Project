#include "llvm/Analysis/LoopPass.h"
#include "llvm/Pass.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/raw_ostream.h"
#include "polly/ScopInfo.h"
// #include "polly/isl/ast.h"
#include "polly/ScopPass.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Debug.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopIterator.h"
#include "polly/Support/ISLOStream.h"
#include <vector>
#include <array>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>
#include <polly/PolyhedralInfo.h>
#include <llvm/Analysis/LoopInfo.h>
#include "llvm/Analysis/ScalarEvolution.h"
// #include ""
using namespace llvm;
using namespace std;
using namespace polly;
#define DEBUG_TYPE "polyhedral-info"
namespace
{
  struct GetStride : public polly ::ScopPass
  {
    static char ID;

    GetStride() : polly::ScopPass(ID) {}

    bool isRegAcc(Instruction *current_instruction)
    {
      if ((current_instruction->getOpcode() != Instruction::Load) && (current_instruction->getOpcode() != Instruction::Store))
      {
        return false;
      }
      std ::vector<llvm ::Instruction *> Frontier;
      Instruction *getbound_ins;
      if (current_instruction->getOpcode() == Instruction::Load)
        getbound_ins = dyn_cast<Instruction>(current_instruction->getOperand(0));
      else
        getbound_ins = dyn_cast<Instruction>(current_instruction->getOperand(1));

      Frontier.push_back(getbound_ins);
      while (!Frontier.empty())
      {
        Instruction *curr_ins = Frontier[Frontier.size() - 1];
        errs() << *curr_ins << "\n";
        Frontier.pop_back();
        if (curr_ins->getOpcode() != Instruction::PHI)
        {
          if (curr_ins->getOpcode() == Instruction ::Alloca)
          {
            return true;
          }

          for (int op_idx = 0; op_idx < curr_ins->getNumOperands(); op_idx++)
          {
            Instruction *current_operand;
            if (current_operand = dyn_cast<Instruction>(curr_ins->getOperand(op_idx)))
            {
              Frontier.push_back(current_operand);
            }
          }
        }
      }
      return false;
    }

    bool runOnScop(Scop &S) override
    {
      // auto schedule = S.getSchedule();
      // auto m = schedule.extract_map(schedule.get_space());
      // get the scopinfo
      // ScopInfo *SI = getAnalysis<polly :: ScopInfoWrapperPass>().getSI();
      // polly :: ScopDetection &SD = getAnalysis<polly :: ScopDetectionWrapperPass>().getSD();
      // errs() << S << "\n";
      // const std :: vector<llvm :: Instruction*>vect;
      // const std :: vector<llvm :: Instruction*>vect2;
      // Region &R = S.getRegion();
      // errs() << R << "\n";
      // errs() << "\nHello\n";
      // errs() << R << "\n";
      // Scop* scop = ScopInfoRegionPass :: getScop();
      // const Scop* scop = polly::ScopInfo::getScop(R);
      // LoopInfo* LI = S.getLI();
      // ScalarEvolution* SE = &getAnalysis<ScalarEvolutionWrapperPass>().getSE();
      // LI->
      // errs() << LI << "\n";
      ////////To be removed later/////////////////////
      //////////Remove later//////////////////////////

      // errs() << "\nScheduleTree:" << scheduleTree << "\n";

      errs() << "////////////////////////////\n";
      std ::vector<Instruction *> Vector_ins;
      std ::vector<Instruction *> Pruned_ins;

      for (auto BB : S.blocks())
      // for(auto L : S.)
      {
        // for(auto &inst : *BB)
        //{
        for (auto stmt : S.getStmtListFor(BB))
        {
          errs() << "////////////////////////////\n";
          auto inst_list = stmt->getInstructions();
          auto domain = stmt->getDomainStr();
          auto sr_loop = stmt->getSurroundingLoop();
          PHINode *ind_var = sr_loop->getCanonicalInductionVariable();
          auto loop_depth = sr_loop->getLoopDepth();
          // To be removed////////////////////////////////////////////////
          //  auto *PHInfo        = &getAnalysis<polly :: PolyhedralInfo>();
          //  auto *LoopInfo      = &getAnalysis<LoopInfoWrapperPass>(B).getLoopInfo();
          //  auto IsParallel = PHInfo->isParallel(sr_loop);
          ///////////////////////////////////////////////////////////////
          errs() << "\nScopStatement: " << *stmt;
          errs() << "\nDomainOfStatement: " << domain;
          errs() << "\nSorroundingLoop: " << *sr_loop;
          errs() << "\nLoopDepth: " << loop_depth;
          errs() << "\nInstructions:\n";
          for (auto inst : inst_list)
          {
            // errs() << "\nScop Statement: " << *stmt << "\nInstruction: " << *inst << "\nDomain: " << domain << "\nSorrounding Loop: " << *sr_loop
            // << "\nInductionVariable" << *ind_var;
            errs() << *inst << "\n";
            errs() << "\nOpCode: " << inst->getOpcodeName() << "\n";
            if (inst->getOpcode() == Instruction ::Call)
            {
              errs() << (inst->getValueName()) << "\n";
            }
          }
          errs() << "ScopEnd\n";
          errs() << "accessbegin\n";
          for (auto access : *stmt)
          {
            errs() << "\n";
            // errs() << "IsRead: " << access->isRead() << "\n";
            errs() << "Element_type: " << *(access->getElementType()) << "\n";
            errs() << "Value: " << *(access->getAccessValue()) << "\n";
            errs() << "Original_Base_Addr: " << *(access->getOriginalBaseAddr()) << "\n";
            errs() << "Array_ID: " << (access->getArrayId()).name() << "\n";
            errs() << "IsScalar: " << access->isScalarKind() << "\n";
            errs() << "IsReductionLike: " << access->isReductionLike() << "\n";
            errs() << "Access_Relation_String: " << access->getAccessRelationStr() << ", IsRead: " << access->isRead() << "\n";
            if (!access->isRead() && !access->isScalarKind())
            {
              std ::vector<llvm ::Instruction *> Frontier;
              Instruction *value_ins = access->getAccessInstruction();

              Frontier.push_back(value_ins);
              while (!Frontier.empty())
              {
                Instruction *current_ins = Frontier[Frontier.size() - 1];
                Frontier.pop_back();

                auto IT = std::find(Vector_ins.begin(), Vector_ins.end(), current_ins);
                if (IT == Vector_ins.end())
                {
                  if (current_ins->getOpcode() != Instruction::PHI)
                  {
                    if (isRegAcc(current_ins) == false)
                    {
                      Vector_ins.push_back(current_ins);
                      errs() << "FOUND THIS VECTORIZED INSTRUCTION: " << *current_ins << "\n";
                    }
                  }
                  if (current_ins->getOpcode() != Instruction::Load)
                  {
                    for (int op_idx = 0; op_idx < current_ins->getNumOperands(); op_idx++)
                    {
                      Instruction *current_operand;
                      if (current_operand = dyn_cast<Instruction>(current_ins->getOperand(op_idx)))
                      {
                        if (current_operand->getOpcode() != Instruction ::GetElementPtr)
                        {
                          Frontier.push_back(current_operand);
                        }
                      }
                    }
                  }
                }
              }
              // Value *operand_first    = value_ins->getOperand(0);
              Value *operand_second = value_ins->getOperand(1);
              Instruction *inst_defs = dyn_cast<Instruction>(operand_second);
            }
            // To be removed
            //  auto stride = access->getStride(S.getSchedule());
            // To be removed
          }
          errs() << "accessend\n";
        }
        // errs() << "StmtEnd\n";
        errs() << "////////////////////////////\n";
        // auto stmt = S.getStmtFor(&inst);
        // auto surrounding_loop = stmt->getSurroundingLoop();
        // errs() << "Instruction: " << inst << "\n";
        //  if(!stmt)
        //  {
        //    continue;
        //  }

        // auto inst_list = stmt->getInstructions();
        // // //errs() << inst_list << "\n";
        // for(auto vect : inst_list)
        // {
        //   errs()  << "\nInstruction: " << *vect << "\n Statement: " << *stmt << "\n";

        // }

        //}
        //  //polly :: ScopStmt :: printInstructions(raw_ostream &OS);
        // errs() << i;
        // polly :: ScopStmt &Stmt = dyn_cast<polly :: ScopStmt>i;
        // vect2 = (i->getInstructions());
        // vect.insert(vect.end(), (i->getInstructions()).begin(), (i->getInstructions()).end());
        // const std vector<Instruction*>vect;
        // polly::ScopStmt* stmt = &*i;
        // errs() << "\nHello\n";
        // errs() << stmt->getInstructions() <<"\n";
        // vect = i->getInstructions();
        // polly :: ScopStmt& Stmt = *i;
        // errs() << "hello";
        // errs() << i;
        //;
      }
      errs() << "////////////////////////////\n";
      return false;
    }
    // void getAnalysisUsage(AnalysisUsage &AU) const  {
    // AU.addRequired<polly::ScopInfoRegionPass();
    // AU.addRequired<polly :: ScopDetectionWrapperPass>();
    //}
    // void getAnalysisUsage(AnalysisUsage &AU) const
    // {
    //   ScopPass::getAnalysisUsage(AU);
    //   //AU.addRequired<ScalarEvolutionWrapperPass>();
    //  AU.addPreserved<LoopInfoWrapperPass>();
    //   AU.addRequired<polly :: PolyhedralInfo>();
    // }
  };
} // namespace
char GetStride::ID = 0;
static RegisterPass<GetStride> X("getstride", "GetStride pass", false /* Only looks at CFG */,
                                 false /* Analysis Pass */);
// INITIALIZE_PASS_BEGIN(PolyhedralInfo, "polyhedral-info",
//                       "Polly - Interface to polyhedral analysis engine", false,
//                       false);
// INITIALIZE_PASS_DEPENDENCY(LoopIcd ~nfoWrapper);
// INITIALIZE_PASS_END(PolyhedralInfo, "polyhedral-info",
//                     "Polly - Interface to polyhedral analysis engine", false,
//                     false)
