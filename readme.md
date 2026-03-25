# exqudens-cpp-dds

## bash-test

- `pushd build/2/test/bin && ./test-app --gtest_list_tests && popd || popd`
- `pushd build/2/test/bin && ./test-app --gtest_filter=OtherUnitTests.test1 && popd || popd`
- `pushd build/2/test/bin && ./test-app --gtest_filter=OtherUnitTests.test2 && popd || popd`

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
