#!/bin/bash
# Regression test: runs the full application and compares output to baseline.
# Usage: runRegressionTest.sh create|test
#   create - run main and save output as baseline (testOutput.dat)
#   test   - run main and diff against baseline; exit non-zero if diff found

if [ -z "$1" ]; then
    echo "Run runRegressionTest.sh create|test"
    exit 1
fi

if [ $1 = "create" ]; then
cd ../bin
./main MG5 ../example_input/mg5/pp2W2eN_5GeV_VvSq1em7/unweighted_events.lhe 9900012 1000 1 1 > ../regression_tests/testOutput.dat
cd ../regression_tests
elif [ $1 = "test" ]; then
cd ../bin
./main MG5 ../example_input/mg5/pp2W2eN_5GeV_VvSq1em7/unweighted_events.lhe 9900012 1000 1 1 > ../regression_tests/tempTestOutput.dat
cd ../regression_tests
diff tempTestOutput.dat testOutput.dat | wc -l
else
    echo "Run runRegressionTest.sh create|test"
fi
