load("@aspect_rules_lint//format:defs.bzl", "format_multirun", "format_test")

package(default_visibility = ["//:__subpackages__"])

format_multirun(
    name = "format",
    c = "@llvm_toolchain_llvm//:bin/clang-format",
    cc = "@llvm_toolchain_llvm//:bin/clang-format",
    cuda = "@llvm_toolchain_llvm//:bin/clang-format",
    starlark = "@buildifier_prebuilt//:buildifier",
    visibility = ["//:__subpackages__"],
)
