# Libratbag-cpp
Libratbag reimplementation in modern c++ 23

## Goals 

- cross platform linux, windows, osx
- Multi vendor support
- Support Mouse and keyboard


## Development

This repository uses [bazel](https://github.com/bazelbuild/bazel) 8.5 for building, testing, and releasing. 
All tool and LLVM toolchain installations are managed by Bazel. The project is compiled using LLVM-based toolchains.

### precommit hooks
This repository runs precommit hooks through bazel using the project
https://gitlab.arm.com/bazel/pre-commit-hooks

Install the hooks with `bazel run hooks:install`.
Run the hooks with `baze run hooks`.

### Build
``` shell
bazel build //ratbag/lib/...
```

### Tests
Run tests:
``` sh
bazel test //ratbag/...
bazel test --test_output=all //ratbag/...
```

### Generate compile_commnads.json:

Generate compile_commnads.json:
``` sh
bazel run @hedron_compile_commands//:refresh_all
```

### Run under debugger
``` sh
bazel run --sandbox_debug --config debug --run_under=lldb //ratbag/cli:cli
```
