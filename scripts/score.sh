#!/bin/sh

BIN=${1}

DIR=tmp

mkdir ${DIR}

R=0
C=0

for i in `seq 0 29`
do
  BASE="testcase_r_${i}"
  ./${BIN} < data/${BASE}.in 1> ${DIR}/${BASE}.out
  SCORE=`./scripts/score_evaluator.out data/${BASE}.in ${DIR}/${BASE}.out`
  echo ${BASE} ${SCORE}
  scr=`echo ${SCORE} | cut -d ' ' -f2 | sed 's@/@@g'`
  R=`expr ${R} + ${scr}`
done

for i in `seq 0 29`
do
  BASE="testcase_c_${i}"
  ./${BIN} < data/${BASE}.in 1> ${DIR}/${BASE}.out
  SCORE=`./scripts/score_evaluator.out data/${BASE}.in ${DIR}/${BASE}.out`
  echo ${BASE} ${SCORE}
  scr=`echo ${SCORE} | cut -d ' ' -f2 | sed 's@/@@g'`
  C=`expr ${C} + ${scr}`
done

echo "random:   ${R}"
echo "complete: ${C}"
