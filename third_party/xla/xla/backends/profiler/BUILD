load("@rules_cc//cc:cc_library.bzl", "cc_library")
load(
    "//xla/tsl:tsl.bzl",
    "if_with_tpu_support",
    "internal_visibility",
)

# copybara:uncomment package(default_applicable_licenses = ["//tensorflow:license"])

package_group(
    name = "friends",
    packages = [
        "//xla/backends/profiler/cpu",
        "//xla/backends/profiler/gpu",
        "//xla/backends/profiler/tpu",
    ],
)

cc_library(
    name = "profiler_backends",
    # copybara:uncomment compatible_with = ["//buildenv/target:non_prod"],
    visibility = internal_visibility(["//xla:internal"]),
    deps = [
        "//xla/backends/profiler/cpu:host_tracer",
        "//xla/backends/profiler/cpu:metadata_collector",
    ] + if_with_tpu_support(
        [
            "//xla/backends/profiler/tpu:tpu_tracer",
        ],
    ),
    alwayslink = True,
)
