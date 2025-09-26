const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "db_zig_tutorial",
        .root_module = b.createModule(.{
            .root_source_file = b.path("src/main.zig"),
            .target = target,
            .optimize = optimize,
        }),
    });

    b.installArtifact(exe);

    const run_step = b.step("run", "Run the app");

    const run_cmd = b.addRunArtifact(exe);
    run_step.dependOn(&run_cmd.step);

    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const common = b.addModule("common", .{
        .root_source_file = b.path("src/common.zig"),
        .target = target,
    });

    const parser = b.addModule("parser", .{
        .root_source_file = b.path("src/parser.zig"),
        .target = target,
    });

    const utils = b.addModule("utils", .{
        .root_source_file = b.path("src/utils.zig"),
        .target = target,
    });

    const common_tests = b.addTest(.{
        .root_module = common,
    });

    const parser_tests = b.addTest(.{
        .root_module = parser,
    });

    const utils_test = b.addTest(.{
        .root_module = utils,
    });

    const run_common_tests = b.addRunArtifact(common_tests);
    const run_parser_tests = b.addRunArtifact(parser_tests);
    const run_utils_tests = b.addRunArtifact(utils_test);

    const exe_tests = b.addTest(.{
        .root_module = exe.root_module,
    });

    const run_exe_tests = b.addRunArtifact(exe_tests);

    const test_step = b.step("test", "Run tests");
    test_step.dependOn(&run_common_tests.step);
    test_step.dependOn(&run_exe_tests.step);
    test_step.dependOn(&run_parser_tests.step);
    test_step.dependOn(&run_utils_tests.step);
}
