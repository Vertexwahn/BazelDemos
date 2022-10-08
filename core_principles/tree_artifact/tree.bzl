"""Tree Demo

Tree Demo to understand tree artifacts better
"""

def _impl(ctx):
    tree = ctx.actions.declare_directory(ctx.attr.name + ".cc")
    ctx.actions.run_shell(
        inputs = [],
        outputs = [tree],
        arguments = [tree.path],
        progress_message = "Generating cc files into '%s'" % tree.path,
        command = """
mkdir {}
cd {}
echo "constexpr int CONSTANT1 = 42;" > dummy1.h
echo "constexpr int CONSTANT2 = 43;" > dummy2.h
touch dummy.cpp
""".format(tree.path, tree.path),
    )
    return [DefaultInfo(files=depset([tree]))]

tree = rule(
    implementation = _impl,
)