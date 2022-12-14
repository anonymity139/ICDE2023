cmake_minimum_required(VERSION 3.15)
project(UserStudy)

set(CMAKE_CXX_STANDARD 14)
set(INC_DIR /usr/local/Cellar/glpk/5.0/include)
set(LINK_DIR /usr/local/Cellar/glpk/5.0/lib)
#set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(.)
include_directories(Others)
include_directories(qhull)
include_directories(QuadProg)
include_directories(structure)
include_directories(UH)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(glpk)

add_executable(run
        Others/lp.cpp
        Others/lp.h
        Others/operation.cpp
        Others/operation.h
        Others/pruning.cpp
        Others/pruning.h
        output/ext_pt.txt
        output/hyperplane_data.txt
        output/point.txt
        output/top.txt
        qhull/geom.cpp
        qhull/geom.h
        qhull/geom2.cpp
        qhull/global.cpp
        qhull/io.cpp
        qhull/io.h
        qhull/libqhull.cpp
        qhull/libqhull.h
        qhull/mem.cpp
        qhull/mem.h
        qhull/merge.cpp
        qhull/merge.h
        qhull/poly.cpp
        qhull/poly.h
        qhull/poly2.cpp
        qhull/qhull_a.h
        qhull/qhull_build.cpp
        qhull/qhull_build.h
        qhull/qset.cpp
        qhull/qset.h
        qhull/random.cpp
        qhull/random.h
        qhull/rboxlib.cpp
        qhull/stat.cpp
        qhull/stat.h
        qhull/user.cpp
        qhull/user.h
        qhull/usermem.cpp
        qhull/userprintf.cpp
        qhull/userprintf_rbox.cpp
        QuadProg/Array.cc
        QuadProg/Array.hh
        QuadProg/QuadProg++.cc
        QuadProg/QuadProg++.hh
        Ranking/active_ranking.cpp
        Ranking/active_ranking.h
        structure/cluster_h.cpp
        structure/cluster_h.h
        structure/cluster_t.cpp
        structure/cluster_t.h
        structure/data_struct.h
        structure/data_utility.cpp
        structure/data_utility.h
        structure/define.h
        structure/hyperplane.cpp
        structure/hyperplane.h
        structure/hyperplane_set.cpp
        structure/hyperplane_set.h
        structure/point_set.cpp
        structure/point_set.h
        structure/point_t.cpp
        structure/point_t.h
        structure/r_node.cpp
        structure/r_node.h
        structure/relational_graph.cpp
        structure/relational_graph.h
        structure/rtree.cpp
        structure/rtree.h
        structure/s_node.cpp
        structure/s_node.h
        structure/tuple_set.cpp
        structure/tuple_set.h
        structure/tuple_t.cpp
        structure/tuple_t.h
        UH/frame.cpp
        UH/frame.h
        UH/maxUtility.cpp
        UH/maxUtility.h
        main.cpp
        structure/inequality.cpp
        structure/inequality.h
        structure/rnode_tree.cpp
        structure/rnode_tree.h
        Adaptive/adaptive.cpp
        Adaptive/adaptive.h
        structure/Ctree.cpp
        structure/Ctree.h General/General.cpp General/General.h)

target_link_libraries(run glpk)