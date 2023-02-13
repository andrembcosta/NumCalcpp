#!/bin/bash 

#for all files on baseline folder
for filename in tests/baselines/*.txt; do
    #substitute "outcome" for "test" in filename
    testfilename=${filename/"tests/baselines/outcome"/test}
    echo "testing:" $testfilename
    #call program on test file, write result to tester.txt
    ./numerics < tests/$testfilename &> tester.txt
    #compare test result with baseline
    if diff -q $filename tester.txt
    then #failure detected
        echo "PASSED"
    else
        echo "FAILURE"
        rm tester.txt
        exit 1    
    fi
    rm tester.txt
done
#all tests have passed
echo "All tests were successful!"
exit 0 