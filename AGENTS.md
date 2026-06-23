# fdx-natives Agent Instructions

These instructions apply to the entire repository.

`AGENTS.md` is the coordination and enforcement layer. It is not the build
architecture source, dependency pin source, or release contract source.

## 1. Source of Truth

Use the canonical project files for durable project facts:

- `fdx-natives.toml`: the single editable source for dependency versions,
  dependency checksums, toolchain pins, and package version.
- `fdx-natives-lock.json`: generated lock manifest for the current pinned
  inputs. Refresh it through Gradle tasks instead of editing it by hand.
- `build.gradle.kts`: Gradle task topology, download/update/package behavior,
  manifest generation, and checksum verification.
- `native/CMakeLists.txt` and `native/fdx-natives-targets.cmake.in`: native
  build layout, staged package contents, and exported CMake imported targets.
- `smoke/`: link smoke tests that validate generated package imports.
- `README.md`: user-facing workflows and release-package usage.
- `.github/workflows/`: CI/release orchestration.
- `AGENTS.md`: process rules only.

When a dependency, toolchain, package layout, exported target, or public task
changes, update the canonical file first, then align README, workflow files, and
this process file only if needed.

## 2. Mandatory Working Contract

### Permission gate

- Do not start investigation, editing, code execution, test execution, download
  work, or validation without explicit user permission.
- If the scope is unclear, ask before touching files.
- Do not continue after the user says stop.

### Communication contract

Before every investigation or fix, state:

- what will be inspected or changed;
- why that target matters;
- how success will be measured.

During work, report intermediate findings when the result is ambiguous, risky,
blocked, expensive, or changes the next step.

### Temporary agent state

- At the start of every new chat/session in this repository, read
  `.agents/agents_memory.md` before investigation, edits, test execution, or
  validation. If the file is absent, treat that as no active recovery state.
- Use `.agents/agents_memory.md` only for the current active state: active
  request, current status, last completed step, next intended step, blockers,
  and validation state. Replace stale content instead of appending history.
- Before changing repository files, generated files, task wiring, docs, or
  validation state, update `.agents/agents_memory.md` with the intended change
  and current status.
- After finishing coding or any meaningful part of a solution, update
  `.agents/agents_memory.md` with what completed, what remains, validation
  evidence, and blockers.
- Use `.agents/agents_plan.md` when a task has multiple ordered solutions or
  steps. This file must be a detailed recovery plan, not a terse checklist. Each
  active step must include the status, objective, target files/modules/tasks,
  why the step matters, the exact intended action, dependencies or blockers, and
  the acceptance or validation evidence needed to mark it complete.
- Keep `.agents/agents_plan.md` limited to the current ordered work. Rewrite it
  when scope changes, update step status as each step completes, and clear it
  when no multi-step task is active.
- Keep `.agents/agents_memory.md` and `.agents/agents_plan.md` local and ignored
  by Git. They are recovery scratch files, not project source, history,
  architecture, or release contract.

### No guessing rule

- Do not invent task names, property names, target names, command shapes,
  version pins, classifier names, or package behavior.
- Confirm code and build solutions against project sources, Gradle task output,
  CMake files, generated manifests, or observed command output before claiming
  correctness.
- Before proposing or applying a fix, confirm:
  - referenced tasks and properties exist;
  - CMake target names match exported declarations;
  - platform/toolchain assumptions are explicit;
  - behavior assumptions are supported by source, docs, or observed output.

## 3. Native Dependency and Package Rules

- Keep all editable dependency/toolchain pins in `fdx-natives.toml`.
- Do not scatter FreeType versions, FreeType checksums, Dawn revisions, Android
  NDK pins, Android CMake pins, Android minSdk values, Emscripten versions, or
  package versions across build files.
- Update FreeType through `updateFreetype`; the task must resolve a configured
  mirror, compute SHA-256, update `fdx-natives.toml`, and refresh the lock
  manifest.
- Update Dawn/Tint through `updateDawn`; Dawn/Tint is pinned by exact commit
  revision, not semantic version.
- Update toolchain pins through `updateToolchainPin`.
- Do not commit downloaded third-party source trees, generated CMake build
  directories, package ZIPs, or unpacked package validation output.
- A release package must include checksum-verifiable artifacts, dependency pins,
  toolchain pins, classifier metadata, and the exported target list.
- Every package task must retain a link smoke test that imports the generated
  CMake targets before the package is considered valid.

### Exported target contract

Release packages must export these CMake imported targets unless the package
contract is intentionally changed and documented:

- `fdx_natives::freetype`
- `fdx_natives::tint`
- `fdx_natives::tint_api_helpers`
- `fdx_natives::tint_lang_core_ir_transform`
- `fdx_natives::tint_lang_glsl_writer_helpers`

## 4. Repository Synchronization Rules

When task wiring, classifiers, properties, target names, package layout,
release artifacts, dependency pins, toolchain pins, or externally visible
behavior change:

1. Update the canonical source listed in section 1.
2. Search for stale task names, property names, classifiers, pins, target names,
   and package layout descriptions.
3. Update all impacted README, workflow, CMake, Gradle, smoke-test, and manifest
   generation locations in the same change.
4. If a generic command remains available but changes meaning or becomes
   ambiguous, document explicit replacement commands wherever the generic
   command appears.
5. Treat generated build output and ignored IDE metadata as non-source unless
   the user explicitly asks to update them.

## 5. Validation Workflow

Before finishing any change, validate in this order:

1. Classify the change type:
   - process/docs
   - dependency pin
   - toolchain pin
   - Gradle task wiring
   - CMake package/export wiring
   - platform package build
   - release workflow
2. List every affected target:
   - files
   - tasks
   - classifiers
   - platforms
   - generated package artifacts
3. Choose and run the smallest validation that proves the changed behavior.
4. Confirm symbol/task/target correctness from source or command output before
   making claims.
5. Cross-check impacted docs and workflow files in one pass.
6. Report blockers exactly:
   - missing compiler/toolchain
   - missing Android SDK/NDK/CMake
   - missing Emscripten SDK
   - unsupported host platform
   - network/download failure
   - dependency checkout failure

### Suggested validation by scope

- Process/docs-only changes: targeted reading, stale-term search when relevant,
  and `git diff --check`.
- Gradle task wiring changes: run `./gradlew tasks --all`, the affected task
  with `--dry-run` when safe, and a real configuration task such as
  `printNativeDepsConfig`.
- Dependency pin changes: run the matching update/resolve task and verify the
  resulting checksum, revision, and lock manifest.
- CMake export/package changes: run the affected classifier package task so the
  link smoke test imports the generated targets.
- Cross-platform package contract changes: validate every affected classifier or
  report each unrun classifier as `NOT RUN` or `BLOCKED` with the exact reason.
- Release workflow changes: validate workflow syntax where possible and run the
  closest local Gradle task sequence that the workflow invokes.

Never claim full validation for a platform/classifier that was not run.

## 6. Final Reporting Checklist

Every final report should state:

- what changed;
- why it changed;
- validation run and result;
- platforms/classifiers not run and exact reasons;
- any remaining risks or follow-up work.

If ambiguity remains after one pass, propose the highest-confidence option and
the safer alternative. Reject ambiguous defaults that waste build cycles or
introduce undocumented package behavior.
