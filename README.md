# ProjectAlign C++ Template

This repository is a production-style C++ template using CMake, with a library/application/test split that matches common industry layouts.

## Layout

- `CMakeLists.txt` - root build entry point
- `app/` - application executable
- `src/project_align/` - reusable library target
- `tests/` - unit tests using CTest
- `cmake/` - package config templates

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/app/project_align_app
```

Expected output:

```text
Hello, world!
```

## Notes

- The library target is named `ProjectAlign` and exposed as `ProjectAlign::ProjectAlign`.
- The application target is `project_align_app`.
- The test target is `project_align_tests` and is executed via CTest.
- The project can be installed as a CMake package using the generated config files.
