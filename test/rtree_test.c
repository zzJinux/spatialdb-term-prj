#include "RTree.h"
#include "geom.h"
#include "utils.h"
#include "query_utils.h"
#include "testutils.h"
#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    assert(argc == 4);
    FILE *listFile = fopen(argv[1], "r");
    assert(listFile != NULL);
    FILE *rqTestcase = fopen(argv[2], "r");
    assert(rqTestcase != NULL);
    FILE *kNNTestcase = fopen(argv[3], "r");
    assert(kNNTestcase != NULL);


    testQuery(listFile, rqTestcase, run_rt_rangeQuery);
    testQuery(listFile, kNNTestcase, run_rt_kNNQuery);

    fclose(listFile);
    fclose(kNNTestcase);
    fclose(rqTestcase);
    return 0;
}
