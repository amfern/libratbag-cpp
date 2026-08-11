"""Module extension for the hidapi-cpp Bazel module"""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def non_module_deps_impl(_):
    maybe(
        http_archive,
        name = "hidapi",
        build_file = Label("//external_bazel_template:BUILD.bazel.hidapi"),
        sha256 = "5d84dec684c27b97b921d2f3b73218cb773cf4ea915caee317ac8fc73cef8136",
        strip_prefix = "hidapi-hidapi-0.15.0",
        url = "https://github.com/libusb/hidapi/archive/refs/tags/hidapi-0.15.0.tar.gz",
    )

non_module_deps = module_extension(
    implementation = non_module_deps_impl,
)
