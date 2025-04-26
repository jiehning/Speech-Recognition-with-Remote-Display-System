cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(ssd1306_tcp_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(ssd1306_tcp_test
	ssd1306.c
	tcp_server.c
	main.c
)

target_include_directories(ssd1306_tcp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(ssd1306_tcp_test pico_cyw43_arch_lwip_threadsafe_background pico_stdlib hardware_i2c)

pico_enable_stdio_usb(ssd1306_tcp_test 1)
pico_enable_stdio_uart(ssd1306_tcp_test 0)

pico_add_extra_outputs(ssd1306_tcp_test)

