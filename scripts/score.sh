#!/bin/sh

BIN=${1}

DIR=tmp

mkdir ${DIR}

for i in `seq 0 29`
do
  ./${BIN} < data/testcase_r_${i}.in 1> ${DIR}/testcase_r_${i}.out
  SCORE=`./scripts/score_evaluator.out data/testcase_r_${i}.in ${DIR}/testcase_r_${i}.out`
  echo ${SCORE}
done

for i in `seq 0 29`
do
  ./${BIN} < data/testcase_c_${i}.in 1> ${DIR}/testcase_c_${i}.out
  SCORE=`./scripts/score_evaluator.out data/testcase_c_${i}.in ${DIR}/testcase_c_${i}.out`
  echo ${SCORE}
done

rm -r ${DIR}
