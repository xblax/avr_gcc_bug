cmake_minimum_required(VERSION 3.7)
project(gcc_bug)

set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER arduino)
set(AVR_UPLOADTOOL_PORT /dev/ttyUSB0)
set(AVR_UPLOADTOOL_OPTIONS "-b 57600")
SET(AVR_MCU "atmega328p")
SET(AVR_MCU_SPEED "16000000L")

set(ARDUINO_SRC_PATH ${PROJECT_SOURCE_DIR}/libarduino)

# find executables in use
find_program(AVR_CC avr-gcc)
find_program(AVR_CXX avr-g++)
find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_SIZE_TOOL avr-size)
find_program(AVR_OBJDUMP avr-objdump)

# toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_C_COMPILER ${AVR_CC})
set(CMAKE_CXX_COMPILER ${AVR_CXX})

# definitions
set(COMMON_FLAGS "-mmcu=${AVR_MCU} -Os -g -ffunction-sections -fdata-sections -flto -Wl,--gc-section")

SET(CMAKE_C_FLAGS "-std=c99 ${COMMON_FLAGS}")
SET(CMAKE_CXX_FLAGS "-std=c++11 ${COMMON_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS ${COMMON_FLAGS})

add_definitions("-DF_CPU=${AVR_MCU_SPEED}")
#add_definitions("-mmcu=${AVR_MCU}")
#add_definitions("-ffunction-sections -fdata-sections")

# executable
set(elf_file ggc_bug.elf)
set(hex_file ggc_bug.hex)

file(GLOB ARDUINO_SRC ${ARDUINO_SRC_PATH}/*.cpp ${ARDUINO_SRC_PATH}/*.c)
ADD_EXECUTABLE(${elf_file} ${ARDUINO_SRC} main.cpp)
target_include_directories(${elf_file} PRIVATE ${ARDUINO_SRC_PATH})

add_custom_command(
        OUTPUT ${hex_file}
        COMMAND
	${AVR_OBJCOPY} -j .text -j .data -O ihex ${CMAKE_BINARY_DIR}/${elf_file} ${CMAKE_BINARY_DIR}/${hex_file}
        COMMAND
        ${AVR_SIZE_TOOL} ${AVR_SIZE_ARGS} ${CMAKE_BINARY_DIR}/${elf_file}
        DEPENDS ${elf_file}
)

# build hex file
add_custom_target(
        build
        DEPENDS ${hex_file}
        COMMENT "Build ${hex_file} for ${AVR_MCU}"
)

# flash with avrdude
add_custom_target(
        flash
        ${AVR_UPLOADTOOL} -p ${AVR_MCU} -c ${AVR_PROGRAMMER} ${AVR_UPLOADTOOL_OPTIONS}
        -U flash:w:${CMAKE_BINARY_DIR}/${hex_file}
        -P ${AVR_UPLOADTOOL_PORT}
        DEPENDS ${hex_file}
        COMMENT "Uploading ${hex_file} to ${AVR_MCU} using ${AVR_PROGRAMMER}"
)

