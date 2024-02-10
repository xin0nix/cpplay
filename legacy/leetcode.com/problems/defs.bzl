load("@bazel_skylib//lib:paths.bzl", "paths")
load("@bazel_skylib//lib:subpackages.bzl", "subpackages")

def build_all_leetcode_problems(name = None):
    """Looks for all the subpackages and builds them in similar fashion. Deprecated!

    Args:
        name: not used
    """
    problems = subpackages.all()
    for name in problems:
        file_name = paths.basename(name)
        native.cc_library(
            name = "{}_lib".format(file_name),
            srcs = [
                "{}:solution.cpp".format(name),
                "{}:solution.hpp".format(name),
            ],
            hdrs = [
                "{}:solution.hpp".format(name),
            ],
        )

        native.cc_test(
            name = "{}_test".format(file_name),
            size = "small",
            srcs = [
                "{}:solution_test.cpp".format(name),
            ],
            deps = [
                ":{}_lib".format(file_name),
                "@googletest//:gtest_main",
            ],
        )

        native.cc_test(
            name = "{}_benchmark".format(file_name),
            tags = ["manual"],
            srcs = [
                "{}:solution_benchmark.cpp".format(name),
            ],
            deps = [
                ":{}_lib".format(file_name),
                "@google_benchmark//:benchmark_main",
            ],
        )

        native.cc_binary(
            name = "{}_main".format(file_name),
            srcs = [
                "{}:main.cpp".format(name),
            ],
            deps = [
                ":{}_lib".format(file_name),
            ],
        )
