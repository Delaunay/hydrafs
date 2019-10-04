
SET(HYDRAFS_SRC
    bencode.cpp
    main.cpp
)

SET(HYDRAFS_HDS
    bencode.h
)



add_executable(main ${HYDRAFS_HDS} ${HYDRAFS_SRC})
target_link_libraries(main uv)
SET_PROPERTY(TARGET main PROPERTY CXX_STANDARD 17)

