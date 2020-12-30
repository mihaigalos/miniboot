load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "rules_cc",
    commit = "02becfef8bc97bda4f9bb64e153f1b0671aec4ba",
    remote = "https://github.com/bazelbuild/rules_cc",
    shallow_since = "1597773444 -0700",
)

git_repository(
    name = "gtest",
    commit = "13a433a94dd9c7e55907d7a9b75f44ff82f309eb",
    remote = "https://github.com/google/googletest",
    shallow_since = "1591825059 -0400",
)

git_repository(
    name = "avr_tools",
    commit = "997c1096f95a12385addc9ecce7a6aae33ae933b",
    remote = "https://github.com/mihaigalos/bazel-avr-tools",
    shallow_since = "1609321613 +0100",
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

