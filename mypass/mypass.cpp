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
#include <map>
#include <string>
#include <iostream>
#include <sstream>
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
      std::map<std::string, polly::ScopArrayInfo *> array_information;
      for (auto arr : S.arrays())
      {
        array_information[arr->getName()] = arr;
      }
      // for(auto arr_1 : S.arrays())
      // {
      //   auto num_dim = arr_1->getNumberOfDimensions();
      //   errs() << arr_1->getName() << " ";
      //   for(int i= 0; i<num_dim; i++)
      //   {
      //     auto dim = arr_1->getDimensionSize(i);
      //     if(dim)
      //     {
      //       errs() << *dim << " ";
      //     }
      //     else
      //     {
      //       errs() << " * ";
      //     }
      //   }
      //   errs() << "\n";
      // }
      errs() << S << "\n";
      // errs() << "////////////////////////////\n";
      std ::vector<Instruction *> Vector_ins;
      std ::vector<Instruction *> Pruned_ins;
      std::map<std::string, std::string> instr_to_relation;
      std::map<std::string, polly::ScopArrayInfo *> instr_to_dimension;

      for (auto BB : S.blocks())
      // for(auto L : S.)
      {
        // for(auto &inst : *BB)
        //{
        for (auto stmt : S.getStmtListFor(BB))
        {
          // errs() << "////////////////////////////\n";
          auto inst_list = stmt->getInstructions();
          auto domain = stmt->getDomainStr();
          auto sr_loop = stmt->getSurroundingLoop();
          PHINode *ind_var = sr_loop->getCanonicalInductionVariable();
          auto loop_depth = sr_loop->getLoopDepth();
          // errs() << "\nScopStatement: " << *stmt;
          // errs() << "\nDomainOfStatement: " << domain;
          // errs() << "\nSorroundingLoop: " << *sr_loop;
          // errs() << "\nLoopDepth: " << loop_depth;
          // errs() << "\nInstructions:\n";
          for (auto inst : inst_list)
          {
            // errs() << *inst << "\n";
            // errs() << "\nOpCode: " << inst->getOpcodeName() << "\n";
            if (inst->getOpcode() == Instruction ::Call)
            {
              // errs() << (inst->getValueName()) << "\n";
            }
          }
          // errs() << "ScopEnd\n";
          // errs() << "accessbegin\n";
          for (auto access : *stmt)
          {
            // errs() << "\n";
            // errs() << "Element_type: " << *(access->getElementType()) << "\n";
            // if (access->getAccessValue())
            // errs() << "Value: " << *(access->getAccessValue()) << "\n";
            // errs() << "Original_Base_Addr: " << *(access->getOriginalBaseAddr()) << "\n";
            // errs() << "Array_ID: " << (access->getArrayId()).name() << "\n";
            // errs() << "IsScalar: " << access->isScalarKind() << "\n";
            // errs() << "IsReductionLike: " << access->isReductionLike() << "\n";
            // errs() << "Access_Relation_String: " << access->getAccessRelationStr() << ", IsRead: " << access->isRead() << "\n";
            if (access->getAccessValue())
            {
              if (access->isScalarKind() == false)
              {
                // auto access_instr = dyn_cast<Instruction>(access->getAccessValue());
                std::string temp;
                llvm::raw_string_ostream buffer(temp);
                buffer << *access->getAccessInstruction();
                // errs() << "HERE IS THE NAME: \"" << buffer.str() << "\"\n";
                // errs() << "ARRAY ID: \"" << access->getArrayId().get_name() << "\"\n";
                // errs() << "RAW INSTR: " << *access->getAccessValue() << "\n";
                if (access->isRead() == false)
                {
                  // errs() << "STORE INSTR: " << *access->getAccessInstruction() << "\n";
                }
                instr_to_relation[buffer.str()] = access->getAccessRelationStr();
                instr_to_dimension[buffer.str()] = array_information[access->getArrayId().get_name()];
              }
            }
            if (!access->isRead() && !access->isScalarKind())
            {
              std ::vector<llvm ::Instruction *> Frontier;
              Instruction *value_ins = access->getAccessInstruction();

              if (value_ins->getOpcode() == Instruction::Call)
              {
                Vector_ins.push_back(value_ins);
                errs() << "VECTOR_MEMCPY: " << *value_ins << " | DOMAIN: " << domain << " | DEPTH: " << loop_depth << "\n";
                continue;
              }

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

                      std::string temp;
                      llvm::raw_string_ostream buffer(temp);
                      buffer << *current_ins;
                      // errs() << "AND THIS IS THE NAME: \"" << buffer.str() << "\"\n";
                      if (current_ins->getOpcode() == Instruction::Load || current_ins->getOpcode() == Instruction::Store)
                        errs() << "VECTOR_ACC: ";
                      else
                        errs() << "VECTOR_OP: ";
                      errs() << *current_ins << " | DOMAIN: " << domain;
                      if (current_ins->getOpcode() == Instruction::Load || current_ins->getOpcode() == Instruction::Store)
                      {
                        errs() << " | ACCESS_REL: " << instr_to_relation[buffer.str()] << " | DIMS: ";
                        polly::ScopArrayInfo *array_inf = instr_to_dimension[buffer.str()];
                        for (int dim = 0; dim < array_inf->getNumberOfDimensions(); dim++)
                          if (array_inf->getDimensionSize(dim))
                            errs() << *(array_inf->getDimensionSize(dim)) << ",";
                          else
                            errs() << " *, ";
                      }
                      errs() << "| DEPTH: " << loop_depth;
                      
                      errs() << "\n";
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
              Value *operand_second = value_ins->getOperand(1);
              Instruction *inst_defs = dyn_cast<Instruction>(operand_second);
            }
          }
          // errs() << "accessend\n";
        }
        // errs() << "StmtEnd\n";
        // errs() << "////////////////////////////\n";
      }
      // errs() << "////////////////////////////\n";
      return false;
    }
  };
} // namespace
char GetStride::ID = 0;
static RegisterPass<GetStride> X("getstride", "GetStride pass", false /* Only looks at CFG */,
                                 false /* Analysis Pass */);
