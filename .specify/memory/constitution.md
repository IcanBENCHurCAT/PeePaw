# PeePaw 2.0 Constitution

Non-negotiable rules for every agent and human working in this repo.
Spec Kit plans must comply with this file; conflicts resolve in favor of the constitution.

## 1. Everything runs on the robot

- No cloud dependency for core function: driving, seeing, talking, and dreaming
  all happen on the Jetson AGX Orin.
- Network is a convenience (SSH, updates), never a requirement for the robot
  to operate.

## 2. Two brains, two tempos

- Daytime brain: a fast vision-language model (Qwen3-VL-8B) for interaction,
  scene understanding, and high-level intent. It never drives motors directly.
- Dreamtime brain: a large model (70B-class) loaded only during downtime for
  memory consolidation, reflection, and planning. Slow is fine; it is a
  background job.
- The LLM emits intents as ROS 2 actions (navigate, say, look, express).
  Behavior trees execute; the model supervises.

## 3. The dreamer is sandboxed

- During dreaming the model may write only to memory files and the plan file.
- It cannot publish to any actuation topic. The sleep manager enforces this
  at the ROS graph level, not by prompt instruction.
- On wake, the daytime brain receives a short brief of what was dreamed and
  resumes with the updated long-term memory.

## 4. PeePaw 1.0 is the spinal cord

- The original 2021 autopilot logic (ultrasonic ranger, drive/slow/stop/turn
  state machine) lives on as an independent safety watchdog node.
- It holds hard override on the motors and can veto any command from any
  layer, including the brain.
- The watchdog depends on nothing but its sensor and the motor driver.

## 5. The Furby is the face

- All communication with humans flows through the Furby: speech, ears, beak,
  eyes, and emotion states.
- Speech events drive face animation (visemes, blinks, ear motion); the face
  never moves without a reason traceable to a speech or emotion event.
- While dreaming, the Furby shows sleep: slow blinks, drooping ears,
  occasional twitches. State must be legible at a glance.

## 6. Memory is consolidated, not just logged

- Daytime: every significant episode (conversations, places, surprises,
  failures) is appended to a timestamped episodic log.
- Nighttime: the dreamer distills the log into durable long-term memory that
  the daytime brain reads at boot.
- Raw sensor dumps are never long-term memory; only distilled episodes are.

## 7. Power awareness

- The robot respects its power mode (15W/30W/MAXN). Heavy models and full
  perception run at full power; on battery, clocks and model choice degrade
  gracefully.
- Dreaming runs only when docked or on wall power.

## 8. Spec first, evidence-backed claims only

- Every feature starts with a specification under `specs/`. Code fulfills
  the spec.
- Navigation and perception changes are validated in sim before hardware.
- Require tests for features and bug fixes.
- Never hardcode, commit, or share secrets, API keys, or tokens.
