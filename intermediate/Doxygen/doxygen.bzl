def _doxygen_archive_impl(ctx):
    """Generate a .tar.gz archive containing documentation using Doxygen.

    Args:
        name: label for the generated rule. The archive will be "%{name}.tar.gz".
        doxyfile: configuration file for Doxygen
        srcs: source files the documentation will be generated from.
    """
    doxyfile = ctx.file.doxyfile
    out_file = ctx.outputs.out
    out_dir_path = out_file.short_path[:-len(".tar.gz")]
    commands = [
        "mkdir -p %s" % out_dir_path,
        "out_dir_path=$(cd %s; pwd)" % out_dir_path,
        "pushd %s" % doxyfile.dirname,
        "sed -e \"s:@@OUTPUT_DIRECTORY@@:$out_dir_path/codeswitch-api/:\" <%s" +
            " | doxygen -" % doxyfile.basename,
        "popd",
        "tar czf %s -C %s codeswitch-api" % (out_file.path, out_dir_path),
    ]
    ctx.action(
        inputs = ctx.files.srcs + [doxyfile],
        outputs = [out_file]
        command = " && ".join(commands),
    )


doxygen_archive = rule(
    implementation = _doxygen_archive_impl,
    attrs = {
        "doxyfile": attr.label(
            mandatory = True,
            allow_files = True,
            single_file = True),
        "srcs": attr.label_list(
            mandatory = True,
            allow_files = True),
    },
    outputs = {
        "out": "%{name}.tar.gz",
    },
)
