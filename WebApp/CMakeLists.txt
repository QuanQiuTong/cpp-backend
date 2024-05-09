add_executable(
	WebApp
	
	user.cc
	problem.cc
	annotation.cc
	main.cpp
)

target_include_directories(
	WebApp PUBLIC
	
	../dependencies/inja/include
	../dependencies/inja/third_party/include
)

target_link_libraries(
	WebApp PUBLIC
	
	bserv
)
