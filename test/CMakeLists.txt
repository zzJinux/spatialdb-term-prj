add_executable(brute_force_test brute_force_test.c)
target_link_libraries(brute_force_test PRIVATE mylib)

add_executable(kdtree_test kdtree_test.c)
target_link_libraries(kdtree_test PRIVATE mylib)

add_executable(rtree_test rtree_test.c)
target_link_libraries(rtree_test PRIVATE mylib)

add_executable(pqueue_test pqueue_test.c)
target_link_libraries(pqueue_test PRIVATE mylib)

add_test(
    NAME brute_force_test
    COMMAND brute_force_test "example_list.txt" "example_RQ.txt" "example_kNN.txt"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(
    NAME kdtree_test
    COMMAND kdtree_test "example_list.txt" "example_RQ.txt" "example_kNN.txt"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(
    NAME rtree_test
    COMMAND rtree_test "example_list.txt" "example_RQ.txt" "example_kNN.txt"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(
    NAME pqueue_test
    COMMAND pqueue_test
)
