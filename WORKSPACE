load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
    name = "rules_cc",
    strip_prefix = "rules_cc-master",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/master.zip"],
)

git_repository(
    name = "gtest",
    commit = "3306848f697568aacf4bcca330f6bdd5ce671899",
    remote = "https://github.com/google/googletest",
)

http_archive(
    name = "avr_tools",
    strip_prefix = "bazel-avr-tools-master",
    urls = [
        "https://github.com/mihaigalos/bazel-avr-tools/archive/master.zip",
    ],
)

new_local_repository(
    name = "system_libs",
    build_file_content = """
cc_library(
    name = "dl",
    srcs = ["libdl.so.2"],
    visibility = ["//visibility:public"],
)
""",
    # pkg-config --variable=libdir x11
    path = "/lib/x86_64-linux-gnu/",
)

new_local_repository(
    name = "system_includes",
    build_file_content = """
cc_library(
    name = "arduino",
    srcs = ["Arduino.h"],
    visibility = ["//visibility:public"],
)
""",
    # pkg-config --variable=libdir x11
    path = "/opt/arduino-1.8.8/hardware/arduino/avr/cores/arduino",
)
