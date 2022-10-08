"""
    SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
"""

def _workspace_name_var_impl(ctx):
    print(ctx.workspace_name)

    return [
        platform_common.TemplateVariableInfo({
            "WORKSPACE_NAME": ctx.workspace_name,
        }),
    ]

workspace_name_var = rule(
    implementation = _workspace_name_var_impl,
    attrs = {},
)
