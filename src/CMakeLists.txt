project(src)

set(headers
    brute_force.h
    KDTree.h
    RTree.h
    geom.h
    utils.h
    array.h
    pqueue.h
    query_utils.h
)

set(sources
    brute_force.c
    KDTree.c
    RTree.c
    array.c
    pqueue.c
)

add_library(mylib ${headers} ${sources})
target_include_directories(mylib INTERFACE .)
target_link_libraries(mylib m)
add_executable(a.out main.c)
target_link_libraries(a.out mylib)
