#!/bin/bash
# a simple bash script unit test. If test result is unequal zero then test has failed

if [ -z "$1" ]; then
    echo "Run runUnitTest.sh create[test]"
    exit 1
fi

if [ $1 = "create" ]; then
cd ../bin
./main MG5 ../example_input/mg5/pp2W2eN_5GeV_VvSq1em7/unweighted_events.lhe 9900012 1000 1 1 > ../unittest/testOutput.dat
cd ../unittest
elif [ $1 = "test" ]; then
cd ../bin
./main MG5 ../example_input/mg5/pp2W2eN_5GeV_VvSq1em7/unweighted_events.lhe 9900012 1000 1 1 > ../unittest/tempTestOutput.dat
cd ../unittest
diff tempTestOutput.dat testOutput.dat | wc -l
else
    echo "Run runUnitTest.sh create[test]"
fi
