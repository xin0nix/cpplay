load("@bazel_skylib//lib:paths.bzl", "paths")

def leetcode_problem(name):
    native.cc_library(
        name = "{}_lib".format(name),
        srcs = [
            paths.join(name, "solution.cpp"),
            paths.join(name, "solution.hpp"),
        ],
        hdrs = [
            paths.join(name, "solution.hpp"),
        ],
    )

    native.cc_test(
        name = "{}_test".format(name),
        size = "small",
        srcs = [
            paths.join(name, "solution_test.cpp"),
        ],
        deps = [
            ":{}_lib".format(name),
            "@com_google_googletest//:gtest_main",
        ],
    )

    native.cc_binary(
        name = "{}_benchmark".format(name),
        srcs = [
            paths.join(name, "solution_benchmark.cpp"),
        ],
        deps = [
            ":{}_lib".format(name),
            "@com_google_google_benchmark//:benchmark_main",
        ],
    )

    native.cc_binary(
        name = "{}_main".format(name),
        srcs = [
            paths.join(name, "main.cpp"),
        ],
        deps = [
            ":{}_lib".format(name),
        ],
    )
