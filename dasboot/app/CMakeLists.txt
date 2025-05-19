add_executable(dasbootd dasbootd.cpp)
apply_compile_flags(dasbootd)
target_link_libraries(dasbootd
	PRIVATE
	daemon
	cru
)

add_executable(dasboot dasbootc.cpp)
apply_compile_flags(dasboot)
target_link_libraries(dasboot
	PRIVATE
	cli
	controller
)
