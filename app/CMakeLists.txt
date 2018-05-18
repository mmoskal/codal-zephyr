set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR}/../)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(NONE)

target_include_directories(app PRIVATE
inc
..
../codal-core/inc/core
../codal-core/inc/driver-models
../codal-core/inc/drivers
../codal-core/inc/streams
../codal-core/inc/types
	)


set(CODAL_CORE_SRC_SKIPPED
../codal-core/source/core/CodalFiber.cpp
)

set(CODAL_CORE_SRC
../codal-core/source/core/CodalCompat.cpp
../codal-core/source/core/CodalComponent.cpp
../codal-core/source/core/CodalDevice.cpp
../codal-core/source/core/CodalDmesg.cpp
../codal-core/source/core/CodalHeapAllocator.cpp
../codal-core/source/core/CodalListener.cpp
../codal-core/source/core/CodalUtil.cpp
../codal-core/source/core/MemberFunctionCallback.cpp
../codal-core/source/driver-models/AbstractButton.cpp
../codal-core/source/driver-models/Accelerometer.cpp
../codal-core/source/driver-models/CodalUSB.cpp
../codal-core/source/driver-models/Compass.cpp
../codal-core/source/driver-models/Display.cpp
../codal-core/source/driver-models/I2C.cpp
../codal-core/source/driver-models/SPI.cpp
../codal-core/source/driver-models/Timer.cpp
../codal-core/source/drivers/AnalogSensor.cpp
../codal-core/source/drivers/AnimatedDisplay.cpp
../codal-core/source/drivers/AsciiKeyMap.cpp
../codal-core/source/drivers/Button.cpp
../codal-core/source/drivers/FXOS8700.cpp
../codal-core/source/drivers/FXOS8700Accelerometer.cpp
../codal-core/source/drivers/FXOS8700Magnetometer.cpp
../codal-core/source/drivers/GhostFAT.cpp
../codal-core/source/drivers/HID.cpp
../codal-core/source/drivers/HIDJoystick.cpp
../codal-core/source/drivers/HIDKeyboard.cpp
../codal-core/source/drivers/HIDMouse.cpp
../codal-core/source/drivers/LEDMatrix.cpp
../codal-core/source/drivers/LIS3DH.cpp
../codal-core/source/drivers/LinearAnalogSensor.cpp
../codal-core/source/drivers/MessageBus.cpp
../codal-core/source/drivers/MultiButton.cpp
../codal-core/source/drivers/NonLinearAnalogSensor.cpp
../codal-core/source/drivers/ST7735.cpp
../codal-core/source/drivers/StandardSPIFlash.cpp
../codal-core/source/drivers/TouchButton.cpp
../codal-core/source/drivers/TouchSensor.cpp
../codal-core/source/drivers/USBMSC.cpp
../codal-core/source/streams/DataStream.cpp
../codal-core/source/streams/LevelDetector.cpp
../codal-core/source/streams/LevelDetectorSPL.cpp
../codal-core/source/streams/StreamNormalizer.cpp
../codal-core/source/streams/Synthesizer.cpp
../codal-core/source/types/BitmapFont.cpp
../codal-core/source/types/CoordinateSystem.cpp
../codal-core/source/types/Event.cpp
../codal-core/source/types/Image.cpp
../codal-core/source/types/ManagedBuffer.cpp
../codal-core/source/types/ManagedString.cpp
../codal-core/source/types/Matrix4.cpp
../codal-core/source/types/RefCounted.cpp
../codal-core/source/types/RefCountedInit.cpp
)

set(CODAL_CORE_NOSRC
)

target_sources(app PRIVATE 
	src/main.cpp 
	src/zfiber.cpp 
	src/ZTimer.cpp 
	src/ZPin.cpp 
	src/codal_target_hal.cpp 
	${CODAL_CORE_SRC})
