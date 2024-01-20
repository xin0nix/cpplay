load("@bazel_skylib//lib:paths.bzl", "paths")
load("@bazel_skylib//lib:subpackages.bzl", "subpackages")

def build_all_leetcode_problems(name = None):
    """Looks for all the subpackages and builds them in similar fashion.

    Args:
        name: not used
    """
    problems = subpackages.all()
    for name in problems:
        file_name = paths.basename(name)

        native.cc_test(
            name = "{}_test".format(file_name),
            size = "small",
            srcs = [
                "{}:solution_test.cpp".format(name),
            ],
            deps = [
                "@com_google_googletest//:gtest_main",
            ],
        )
