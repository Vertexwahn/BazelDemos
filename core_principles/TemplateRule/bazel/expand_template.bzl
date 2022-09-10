# Copied from https://github.com/tensorflow/tensorflow/blob/master/third_party/common.bzl with minor modifications
# SPDX-License-Identifier: Apache-2.0

def expand_template_impl(ctx):
    ctx.actions.expand_template(
        template = ctx.file.template,
        output = ctx.outputs.out,
        substitutions = ctx.attr.substitutions,
    )

_expand_template = rule(
    implementation = expand_template_impl,
    attrs = {
        "template": attr.label(mandatory = True, allow_single_file = True),
        "substitutions": attr.string_dict(mandatory = True),
        "out": attr.output(mandatory = True),
    },
    output_to_genfiles = True,
)

def expand_template(name, template, substitutions, out):
    _expand_template(
        name = name,
        template = template,
        substitutions = substitutions,
        out = out,
    )