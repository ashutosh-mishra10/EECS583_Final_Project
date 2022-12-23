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
//#include ""
using namespace llvm;
using namespace std;
namespace
{
  struct GetStride : public polly :: ScopPass
  {
    static char ID;

    GetStride() : polly :: ScopPass(ID) {}

    bool runOnScop(Scop &S) override
    {
       //auto schedule = S.getSchedule();
       //auto m = schedule.extract_map(schedule.get_space());
       //get the scopinfo
      //ScopInfo *SI = getAnalysis<polly :: ScopInfoWrapperPass>().getSI();
      //polly :: ScopDetection &SD = getAnalysis<polly :: ScopDetectionWrapperPass>().getSD();
      
      errs() << S << "\n";
      
      //vector<int>vect;
      //Region &R = S.getRegion();
      //for (auto i = S.begin(); i != S.end(); i++)
      //for(polly::ScopStmt *i : S.getStmtListFor(&R))
      //{
        //polly :: ScopStmt :: printInstructions(raw_ostream &OS);
        //errs() << i;
        //polly :: ScopStmt &Stmt = dyn_cast<polly :: ScopStmt>i;
        //vect.push_back(polly :: ScopStmt :: getInstructions());
        //polly :: ScopStmt& Stmt = *i;
        //errs() << "hello";
        //errs() << Stmt;
        //;
      //}

      return false;
    }
   void getAnalysisUsage(AnalysisUsage &AU) const override {
  //   //AU.addRequired<>();
  //   //AU.addRequired<BlockFrequencyInfoWrapperPass>();
  //   //AU.addRequired<LoopInfoWrapperPass>();
     AU.addRequired<polly :: ScopInfoWrapperPass>();
     AU.addRequired<polly :: ScopDetectionWrapperPass>();
     //AU.addRequired<polly :: ScopPass ::  >
   }

  };
} // namespace
char GetStride::ID = 0;
static RegisterPass<GetStride> X("getstride", "GetStride pass", false /* Only looks at CFG */,
                                 false /* Analysis Pass */);



