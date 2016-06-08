cmake_minimum_required(VERSION 3.0)

project(Erzeuger)
set(SOURCE_FILES erzeuger_main.c updown.c updown.h ring_buffer.c ring_buffer.h)
add_executable(Erzeuger ${SOURCE_FILES})

project(Verbraucher)
set(SOURCE_FILES_VERBRAUCHER verbraucher_main.c updown.h updown.c ring_buffer.h ring_buffer.c)
add_executable(Verbraucher ${SOURCE_FILES_VERBRAUCHER})

project(SemaphoreTest)
set(SOURCE_FILES_SEM_TEST semtest.c updown.h updown.c)
add_executable(SemaphoreTest ${SOURCE_FILES_SEM_TEST})

project(RingbufferTest)
set(SORCE_FILES_RING_BUFFER_TEST ringtest.c ring_buffer.c ring_buffer.h updown.c updown.h)
add_executable(RingbufferTest ${SORCE_FILES_RING_BUFFER_TEST})