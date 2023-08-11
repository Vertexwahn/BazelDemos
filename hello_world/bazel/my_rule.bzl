"""
    SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
"""

def _impl(ctx):
    print("Hello World!")

my_rule = rule(
    implementation = _impl,
)
