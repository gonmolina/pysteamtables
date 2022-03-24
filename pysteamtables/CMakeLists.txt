set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(freesteam 
"lib/b23.c"
"lib/backwards.c"
"lib/bounds.c"
"lib/common.c"
"lib/derivs.c"
"lib/region1.c"
"lib/region2.c"
"lib/region3.c"
"lib/region4.c"
"lib/solver2.c"
"lib/steam.c"
"lib/steam_ph.c"
"lib/steam_ps.c"
"lib/steam_pT.c"
"lib/steam_pu.c"
"lib/steam_pv.c"
"lib/steam_Ts.c"
"lib/steam_Tx.c"
"lib/steam_uv.c"
"lib/surftens.c"
"lib/thcond.c"
"lib/viscosity.c"
"lib/zeroin.c")


target_include_directories(freesteam PRIVATE ${GSL_INCLUDE_DIRS})
target_link_libraries(freesteam -lm ${GSL_LIBRARIES})

add_executable(test "lib/test.c")
target_link_libraries(test freesteam)

add_cython_target(steam_tables OUTPUT_VAR X)
add_library(steam_tables MODULE ${X})
target_link_libraries(steam_tables freesteam)

python_extension_module(steam_tables)

install(TARGETS freesteam LIBRARY DESTINATION pysteamtables)
install(TARGETS steam_tables LIBRARY DESTINATION pysteamtables)



