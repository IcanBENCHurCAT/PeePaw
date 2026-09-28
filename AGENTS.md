# AGENTS.md - PeePaw 2.0

Operating notes for agents working on this repo. Minimal on purpose; grow it as the project does.

## Target hardware
- NVIDIA Jetson AGX Orin 64GB developer kit (owned)
- JetPack 7.2: Ubuntu 24.04, kernel 6.8, CUDA 13, TensorRT
- Verify 64GB board revision against the JetPack 7.2 release notes before flashing

## Software stack
- **ROS 2 Jazzy**, native install (not containers) on the Jetson
- **Nav2** for navigation (planner, controller, behavior trees)
- **Isaac ROS** for GPU perception: visual SLAM, NVBlox, TensorRT DNN nodes
- Build with **colcon**; test navigation in sim (Gazebo or Isaac Sim) before hardware runs

## Brains
- Daytime: Qwen3-VL-8B Q4 via **llama.cpp / llama-server** built with CUDA (sm_87)
- Dreamtime: 70B-class Q4_K_M, loaded only when docked/idle, unloaded on wake
- No cloud dependency for core function; everything runs on-device

## Low-level I/O
- **PCA9685** 16-ch PWM over I2C: drives motor PWM and all Furby servos
- **TB6612FNG** H-bridge drivers for the 4 drive motors and the Furby cam motor
- **Jetson.GPIO** for ultrasonic ranger, e-stop, buttons
- I2C: BNO085 IMU; I2S: MAX98357A amp + speaker, mic array over USB

## Voice
- STT: faster-whisper (streaming)
- TTS: Piper for speed, F5-TTS voice clone for character

## Workflow
- Spec-first: every feature gets a spec under `specs/` before code
- `.specify/memory/constitution.md` is law; conflicts resolve in its favor
- Safety watchdog node (PeePaw 1.0 reflex logic) is independent of the brain and can veto any motion
