"""bzl file to manage the workspace dependency of libratbag.
"""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def download_hidapi(build_file):
    """Download hidapi dependency.

    The build file differs for WORKSPACE and Bzlmod users due to a difference in
    the Boost library repository name.
    """

    maybe(
        http_archive,
        name = "hidapi",
        build_file = build_file,
        sha256 = "5d84dec684c27b97b921d2f3b73218cb773cf4ea915caee317ac8fc73cef8136",
        strip_prefix = "hidapi-hidapi-0.15.0",
        url = "https://github.com/libusb/hidapi/archive/refs/tags/hidapi-0.15.0.tar.gz",
    )

def _non_module_deps_impl(_):
    download_hidapi(Label("//external_bazel_template:BUILD.bazel.hidapi"))

non_module_deps = module_extension(
    implementation = _non_module_deps_impl,
)
