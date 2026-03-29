# exqudens-cpp-dds

## bash-test

- `pushd build/2/test/bin && ./test-app --gtest_list_tests && popd || popd`
- `pushd build/2/test/bin && ./test-app --gtest_filter=PerformanceSystemTests.test1 && popd || popd`
- `pushd build/2/test/bin && ./test-app --gtest_filter=PerformanceSystemTests.test2 && popd || popd`

## how-to-test-all-presets

1. `git clean -xdf`
2. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --preset {} || exit 255"`
3. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --build --preset {} --target cmake-test || exit 255"`

## how-to-build-all-presets

1. `git clean -xdf`
2. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --preset {} || exit 255"`
3. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --build --preset {} --target cmake-install || exit 255"`

## how-to-export-all-presets

1. `conan list 'exqudens-cpp-dds/*'`
2. `conan remove -c 'exqudens-cpp-dds'`
3. `git clean -xdf`
4. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --preset {} || exit 255"`
5. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --build --preset {} --target conan-export || exit 255"`

## vscode

1. `git clean -xdf`
2. `cmake --preset ${preset}`
3. `cmake --build --preset ${preset} --target vscode`
4. use vscode debugger launch configurations: `test-app`

### extensions

For `command-variable-launch.json`
use [Command Variable](https://marketplace.visualstudio.com/items?itemName=rioj7.command-variable#pickstringremember) `version >= v1.69.0`

## windows

- set value `REALTIME_PRIORITY_CLASS` using [SetPriorityClass](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setpriorityclass)
- get current using [GetCurrentProcess](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getcurrentprocess)
- for threads begin [timeBeginPeriod](https://learn.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timebeginperiod)
- for threads end [timeEndPeriod](https://learn.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timeendperiod)

### additional

- [soft-real-time](https://learn.microsoft.com/en-us/windows/iot/iot-enterprise/soft-real-time/soft-real-time)
- [soft-real-time-device](https://learn.microsoft.com/en-us/windows/iot/iot-enterprise/soft-real-time/soft-real-time-device)
- [soft-real-time-application](https://learn.microsoft.com/en-us/windows/iot/iot-enterprise/soft-real-time/soft-real-time-application)
