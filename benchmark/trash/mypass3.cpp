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
//#include ""
using namespace llvm;
using namespace std;
namespace
{
  struct GetStride : public polly :: ScopPass
  {
    static char ID;

    GetStride() : polly::ScopPass(ID) {}

    bool runOnScop(Scop &S) override
    {
       //auto schedule = S.getSchedule();
       //auto m = schedule.extract_map(schedule.get_space());
       //get the scopinfo
      //ScopInfo *SI = getAnalysis<polly :: ScopInfoWrapperPass>().getSI();
      //polly :: ScopDetection &SD = getAnalysis<polly :: ScopDetectionWrapperPass>().getSD();
      
      //errs() << S << "\n";
      
      //const std :: vector<llvm :: Instruction*>vect;
      //const std :: vector<llvm :: Instruction*>vect2;
      Region &R = S.getRegion();
      errs() << R << "\n";
      errs() << "Hello\n";
      //errs() << R << "\n";
      //Scop* scop = ScopInfoRegionPass :: getScop();
      //const Scop* scop = polly::ScopInfo::getScop(R);
      for (auto i = S.begin(); i != S.end(); i++)
      //for(polly::ScopStmt *i : S.getStmtListFor(&R))
      //for(auto i : S.getStmtListFor(&R))
      {
        //polly :: ScopStmt :: printInstructions(raw_ostream &OS);
        //errs() << i;
        //polly :: ScopStmt &Stmt = dyn_cast<polly :: ScopStmt>i;
        //vect2 = (i->getInstructions());
        //vect.insert(vect.end(), (i->getInstructions()).begin(), (i->getInstructions()).end());  
        //const std vector<Instruction*>vect;
        //errs() << "Hello";
        //vect = i->getInstructions();
        //polly :: ScopStmt& Stmt = *i;
        //errs() << "hello";
        //errs() << i;
        //;
      }
      
      return false;
    }
   //void getAnalysisUsage(AnalysisUsage &AU) const  {
     //AU.addRequired<polly::ScopInfoRegionPass();
     //AU.addRequired<polly :: ScopDetectionWrapperPass>();
   //}

  };
} // namespace
char GetStride::ID = 0;
static RegisterPass<GetStride> X("getstride", "GetStride pass", false /* Only looks at CFG */,
                                 false /* Analysis Pass */);



