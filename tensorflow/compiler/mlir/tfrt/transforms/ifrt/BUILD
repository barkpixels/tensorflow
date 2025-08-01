load("@llvm-project//mlir:tblgen.bzl", "gentbl_cc_library")
load(
    "@local_xla//xla/tsl/platform:build_config.bzl",
    "tf_proto_library",
)
load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("//tensorflow:tensorflow.bzl", "if_google", "tf_cc_test")

package(
    # copybara:uncomment default_applicable_licenses = ["//tensorflow:license"],
    default_visibility = [":friends"],
    licenses = ["notice"],
)

package_group(
    name = "friends",
    packages = [
        "//tensorflow/compiler/mlir/tfrt/...",
        "//tensorflow/core/tfrt/ifrt/...",
        "//tensorflow/core/tfrt/saved_model/tests/...",
    ] + if_google([
        "//learning/brain/tfrt/cpp_tests/...",
        "//learning/serving/servables/tfrt/...",
        "//learning/brain/tfrt/ifrt/...",
        "//learning/brain/tfrt/tfrt_session/...",
        # Allow visibility from the mlir language server.
        "//learning/brain/mlir/mlir_lsp_server/...",
        "//learning/infra/mira/experimental/orbax_model/serving/sidecar/...",
        "//smartass/brain/util/...",
    ]),
)

gentbl_cc_library(
    name = "pass_inc_gen",
    tbl_outs = {"passes.h.inc": [
        "-gen-pass-decls",
        "-name=TfrtIfrtServing",
    ]},
    tblgen = "@llvm-project//mlir:mlir-tblgen",
    td_file = "passes.td",
    deps = [
        "@llvm-project//mlir:PassBaseTdFiles",
    ],
)

cc_library(
    name = "ifrt_constants",
    srcs = [],
    hdrs = ["ifrt_constants.h"],
    deps = [
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "ifrt_types",
    srcs = [],
    hdrs = ["ifrt_types.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//tensorflow/core:framework",
        "//tensorflow/core:protos_all_cc",
    ],
)

cc_library(
    name = "tf_ifrt_passes",
    srcs = [
        "lower_to_ifrt_restore_variable.cc",
        "rewrite_cluster_to_ifrt_call.cc",
        "sink_variable_as_named_array.cc",
        "tf_device_cleanup.cc",
        "tf_identity_propagation.cc",
        "tf_ifrt_passes.cc",
        "tf_restore_merging.cc",
        "tf_restore_pruning.cc",
        "tf_restore_splitting.cc",
    ],
    hdrs = [
        "tf_ifrt_passes.h",
    ],
    textual_hdrs = ["passes.h.inc"],
    deps = [
        ":ifrt_constants",
        ":pass_inc_gen",
        "//tensorflow/compiler/mlir/tensorflow",
        "//tensorflow/compiler/mlir/tensorflow:bridge_logger",
        "//tensorflow/compiler/mlir/tensorflow:device_util",
        "//tensorflow/compiler/mlir/tensorflow:dump_mlir_util",
        "//tensorflow/compiler/mlir/tensorflow:error_util",
        "//tensorflow/compiler/mlir/tensorflow:tensorflow_ops",
        "//tensorflow/compiler/mlir/tensorflow:tensorflow_types",
        "//tensorflow/compiler/mlir/tensorflow:tpu_rewrite_device_util",
        "//tensorflow/compiler/mlir/tensorflow:translate_utils",
        "//tensorflow/compiler/mlir/tensorflow/ir/host_runtime:tensorflow_tfrt_ops",
        "//tensorflow/compiler/mlir/tensorflow/transforms:tensorflow_passes",
        "//tensorflow/compiler/mlir/tensorflow/transforms/host_runtime:tpu_metadata_utils",
        "//tensorflow/compiler/mlir/tfrt:tf_to_tfrt",
        "//tensorflow/core:framework",
        "//tensorflow/core/platform:protobuf",
        "//tensorflow/core/platform:random",
        "//tensorflow/core/protobuf/tpu:compile_metadata_proto_cc",
        "//tensorflow/core/tfrt/ifrt:ifrt_config_proto_cc",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/log",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@llvm-project//llvm:Support",
        "@llvm-project//mlir:FuncDialect",
        "@llvm-project//mlir:IR",
        "@llvm-project//mlir:Pass",
        "@llvm-project//mlir:Support",
        "@llvm-project//mlir:Transforms",
        "@local_tsl//tsl/platform:protobuf",
        "@local_xla//xla:xla_data_proto_cc",
        "@local_xla//xla/service:computation_placer_hdr",
    ],
)

cc_library(
    name = "tf2hlo",
    srcs = ["tf2hlo.cc"],
    hdrs = ["tf2hlo.h"],
    deps = [
        ":ifrt_compilation_proto_cc",
        ":ifrt_constants",
        ":ifrt_types",
        "//tensorflow/compiler/jit:xla_cpu_jit",
        "//tensorflow/compiler/mlir/tensorflow:dump_mlir_util",
        "//tensorflow/compiler/mlir/tensorflow:serialize_mlir_module_utils",
        "//tensorflow/compiler/mlir/tf2xla/api/v2:legalize_tf",
        "//tensorflow/compiler/tf2xla:layout_util",
        "//tensorflow/compiler/tf2xla:xla_compiler",
        "//tensorflow/core:core_cpu_base",
        "//tensorflow/core:framework",
        "//tensorflow/core:lib_headers_for_pybind",
        "//tensorflow/core:protos_all_cc",
        "//tensorflow/core/protobuf/tpu:compile_metadata_proto_cc",
        "//tensorflow/core/protobuf/tpu:topology_proto_cc",
        "//tensorflow/core/tpu/kernels:tpu_compile_op_support",
        "//tensorflow/core/tpu/kernels/xla:host_compute_ops",
        "@com_google_absl//absl/log",
        "@com_google_absl//absl/log:check",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:span",
        "@llvm-project//llvm:Support",
        "@llvm-project//mlir:FuncDialect",
        "@llvm-project//mlir:IR",
        "@llvm-project//mlir:Pass",
        "@llvm-project//mlir:Support",
        "@local_tsl//tsl/platform:fingerprint",
        "@local_xla//xla:shape_util",
        "@local_xla//xla:xla_data_proto_cc",
        "@local_xla//xla/client:client_library",
        "@local_xla//xla/hlo/translate/hlo_to_mhlo:hlo_to_mlir_hlo",
        "@local_xla//xla/pjrt:pjrt_compiler",
        "@local_xla//xla/python/ifrt",
        "@local_xla//xla/service:computation_placer_hdr",
        "@local_xla//xla/service:hlo_proto_cc",
        "@local_xla//xla/stream_executor:platform_manager",
    ],
)

cc_library(
    name = "extract_callback",
    srcs = ["extract_callback.cc"],
    hdrs = ["extract_callback.h"],
    deps = [
        "//tensorflow/compiler/mlir/tensorflow:error_util",
        "//tensorflow/compiler/mlir/tensorflow:tensorflow_ops_a_m_inc_gen",
        "//tensorflow/compiler/mlir/tensorflow:tensorflow_ops_n_z_inc_gen",
        "//tensorflow/compiler/mlir/tensorflow:visitor",
        "//tensorflow/core:protos_all_cc",
        "@com_google_absl//absl/log",
        "@com_google_absl//absl/log:check",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@llvm-project//llvm:Support",
        "@llvm-project//mlir:FuncDialect",
        "@llvm-project//mlir:IR",
        "@llvm-project//mlir:Pass",
        "@llvm-project//mlir:Support",
    ],
)

tf_cc_test(
    name = "tf2hlo_test",
    srcs = [
        "tf2hlo_test.cc",
    ],
    data = [
        "//tensorflow/compiler/mlir/tfrt/transforms/ifrt/testdata",
    ],
    tags = ["no_oss"],
    deps = [
        ":ifrt_types",
        ":tf2hlo",
        "//tensorflow/compiler/mlir/tensorflow",
        "//tensorflow/compiler/tf2xla:xla_helpers",
        "//tensorflow/core:test",
        "//tensorflow/core/platform:resource_loader",
        "@com_google_absl//absl/log",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:span",
        "@com_google_googletest//:gtest_main",
        "@llvm-project//mlir:AllPassesAndDialects",
        "@llvm-project//mlir:IR",
        "@llvm-project//mlir:Parser",
        "@local_tsl//tsl/platform:protobuf",
        "@local_xla//xla/pjrt:pjrt_client",
        "@local_xla//xla/pjrt:pjrt_compiler",
        "@local_xla//xla/pjrt/gpu:se_gpu_pjrt_client",
        "@local_xla//xla/pjrt/plugin/xla_cpu:cpu_topology_description",
        "@local_xla//xla/python/ifrt",
        "@local_xla//xla/python/ifrt:mock",
        "@local_xla//xla/python/ifrt:test_util",
        "@local_xla//xla/python/pjrt_ifrt",
        "@local_xla//xla/python/pjrt_ifrt:tfrt_cpu_client_test_lib",
        "@local_xla//xla/service:computation_placer_hdr",
    ],
)

cc_library(
    name = "ifrt_backend_compiler",
    srcs = ["ifrt_backend_compiler.cc"],
    hdrs = ["ifrt_backend_compiler.h"],
    deps = [
        ":tf_ifrt_passes",
        "//tensorflow/compiler/mlir/tensorflow:dump_mlir_util",
        "//tensorflow/compiler/mlir/tensorflow:error_util",
        "//tensorflow/compiler/mlir/tensorflow:visitor",
        "//tensorflow/compiler/mlir/tf2xla/api/v2:cluster_tf",
        "//tensorflow/compiler/mlir/tfrt:backend_compiler",
        "//tensorflow/compiler/mlir/tfrt:tpu_passes",
        "//tensorflow/core/tfrt/ifrt:ifrt_executable_registry",
        "//tensorflow/core/tfrt/ifrt:ifrt_model_context",
        "//tensorflow/core/tfrt/ifrt:ifrt_serving_executable",
        "//tensorflow/core/tfrt/runtime",
        "@com_google_absl//absl/log",
        "@com_google_absl//absl/log:check",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@llvm-project//llvm:Support",
        "@llvm-project//mlir:FuncDialect",
        "@llvm-project//mlir:IR",
        "@llvm-project//mlir:Support",
        "@local_tsl//tsl/profiler/lib:traceme",
        "@local_xla//xla/tsl/platform:errors",
        "@local_xla//xla/tsl/platform:statusor",
    ],
)

tf_cc_test(
    name = "ifrt_backend_compiler_test",
    srcs = [
        "ifrt_backend_compiler_test.cc",
    ],
    data = [
        "//tensorflow/compiler/mlir/tfrt/transforms/ifrt/testdata",
    ],
    tags = ["no_oss"],
    deps = [
        ":ifrt_backend_compiler",
        "//tensorflow/compiler/mlir/tensorflow",
        "//tensorflow/core:test",
        "//tensorflow/core/platform:resource_loader",
        "//tensorflow/core/tfrt/graph_executor:graph_execution_options",
        "//tensorflow/core/tfrt/ifrt:ifrt_executable_registry",
        "//tensorflow/core/tfrt/ifrt:ifrt_model_context",
        "//tensorflow/core/tfrt/ifrt:ifrt_serving_core_selector",
        "//tensorflow/core/tfrt/runtime",
        "//tensorflow/core/tfrt/saved_model:saved_model_testutil",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
        "@com_google_googletest//:gtest_main",
        "@llvm-project//mlir:AllPassesAndDialects",
        "@llvm-project//mlir:FuncDialect",
        "@llvm-project//mlir:IR",
        "@llvm-project//mlir:Parser",
        "@local_xla//xla/python/ifrt",
        "@local_xla//xla/python/ifrt:test_util",
        "@local_xla//xla/python/pjrt_ifrt:tfrt_cpu_client_test_lib",
        "@local_xla//xla/tsl/framework/test_util:mock_serving_device_selector",
        "@local_xla//xla/tsl/platform:env",
        "@local_xla//xla/tsl/platform:status_matchers",
        "@local_xla//xla/tsl/platform:statusor",
        "@tf_runtime//:hostcontext",
    ],
)

tf_proto_library(
    name = "ifrt_compilation_proto",
    srcs = ["ifrt_compilation.proto"],
    protodeps = [
        "//tensorflow/compiler/tf2xla:host_compute_metadata_proto",
        "@local_xla//xla/service:hlo_proto",
        "//tensorflow/core:protos_all",
        "//tensorflow/core/protobuf/tpu:compile_metadata_proto",
    ],
)
