CLANG="/home/ashutosh/llvm-project/build/bin/clang"
OPT="/home/ashutosh/llvm-project/build/bin/opt"
PROFDATA="/home/ashutosh/llvm-project/build/bin/llvm-profdata"
clear
FILE=${1}

PROF_OPT="-pgo-instr-use -pgo-test-profile-file=pgo.profdata"
OPT_OPT="-enable-new-pm=0"

${CLANG} -O1 -emit-llvm ${FILE}.c -c -o ${FILE}.bc
#Scalar evolution pass
#${OPT} -enable-new-pm=0 -pgo-instr-gen -instrprof ${FILE}.bc -o ${FILE}.prof.bc
#${CLANG} -fprofile-instr-generate ${FILE}.prof.bc -o ${FILE}.prof
#./${FILE}.prof 
#llvm-profdata merge -output=pgo.profdata default.profraw
${OPT} -O1 -enable-new-pm=0 -load build/mypass/matmul.so -getstride < ${FILE}.bc > /dev/null
#${OPT} -O1 -dot-cfg -enable-new-pm=0 ${FILE}.bc > /dev/null
#cat .${FILE}.dot | dot -Tpdf > ${FILE}.pdf > /dev/null
#opt -enable-new-pm=0 -pgo-instr-use -pgo-test-profile-file=pgo.profdata -load ${PATH_MYPASS} ${NAME_MYPASS} < ${1}.bc > /dev/null





#${OPT} -enable-new-pm=0 -O1 -scalar-evolution -analyze ${FILE}.bc -o ${FILE}.scalar.bc


#${OPT} -O1 -enable-new-pm=0 -load build/mypass/matmul.so -getstride < ${FILE}.bc > /dev/null