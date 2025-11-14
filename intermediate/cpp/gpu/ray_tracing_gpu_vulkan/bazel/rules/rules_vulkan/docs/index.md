<!-- Generated with Stardoc: http://skydoc.bazel.build -->

Definitions for the Vulkan SDK Bazel rules.

<a id="glsl_shader"></a>

## glsl_shader

<pre>
load("@rules_vulkan//vulkan:defs.bzl", "glsl_shader")

glsl_shader(<a href="#glsl_shader-name">name</a>, <a href="#glsl_shader-src">src</a>, <a href="#glsl_shader-hdrs">hdrs</a>, <a href="#glsl_shader-defines">defines</a>, <a href="#glsl_shader-includes">includes</a>, <a href="#glsl_shader-opts">opts</a>, <a href="#glsl_shader-stage">stage</a>, <a href="#glsl_shader-std">std</a>, <a href="#glsl_shader-target_env">target_env</a>, <a href="#glsl_shader-target_spv">target_spv</a>)
</pre>

Rule to compile GLSL shader.

**ATTRIBUTES**


| Name  | Description | Type | Mandatory | Default |
| :------------- | :------------- | :------------- | :------------- | :------------- |
| <a id="glsl_shader-name"></a>name |  A unique name for this target.   | <a href="https://bazel.build/concepts/labels#target-names">Name</a> | required |  |
| <a id="glsl_shader-src"></a>src |  Input GLSL shader source to compile   | <a href="https://bazel.build/concepts/labels">Label</a> | required |  |
| <a id="glsl_shader-hdrs"></a>hdrs |  List of header files dependencies to be included in the shader compilation   | <a href="https://bazel.build/concepts/labels">List of labels</a> | optional |  `[]`  |
| <a id="glsl_shader-defines"></a>defines |  List of macro defines   | List of strings | optional |  `[]`  |
| <a id="glsl_shader-includes"></a>includes |  Add directory to include search path to CLI   | List of strings | optional |  `[]`  |
| <a id="glsl_shader-opts"></a>opts |  Additional arguments to pass to the compiler   | List of strings | optional |  `[]`  |
| <a id="glsl_shader-stage"></a>stage |  Shader stage (vertex, vert, fragment, frag, etc)   | String | required |  |
| <a id="glsl_shader-std"></a>std |  Version and profile for GLSL input files.<br><br>Possible values are concatenations of version and profile, e.g. `310es`, `450core`, etc.   | String | optional |  `""`  |
| <a id="glsl_shader-target_env"></a>target_env |  Set the target client environment, and the semantics of warnings and errors.<br><br>An optional suffix can specify the client version.   | String | optional |  `""`  |
| <a id="glsl_shader-target_spv"></a>target_spv |  Set the SPIR-V version to be used for the generated SPIR-V module.<br><br>The default is the highest version of SPIR-V required to be supported for the target environment. For example, default for `vulkan1.0` is `spv1.0`.   | String | optional |  `""`  |


<a id="hlsl_shader"></a>

## hlsl_shader

<pre>
load("@rules_vulkan//vulkan:defs.bzl", "hlsl_shader")

hlsl_shader(<a href="#hlsl_shader-name">name</a>, <a href="#hlsl_shader-src">src</a>, <a href="#hlsl_shader-hdrs">hdrs</a>, <a href="#hlsl_shader-asm">asm</a>, <a href="#hlsl_shader-def_root_sig">def_root_sig</a>, <a href="#hlsl_shader-defines">defines</a>, <a href="#hlsl_shader-entry">entry</a>, <a href="#hlsl_shader-hash">hash</a>, <a href="#hlsl_shader-hlsl">hlsl</a>, <a href="#hlsl_shader-includes">includes</a>, <a href="#hlsl_shader-opts">opts</a>, <a href="#hlsl_shader-reflect">reflect</a>,
            <a href="#hlsl_shader-spirv">spirv</a>, <a href="#hlsl_shader-target">target</a>)
</pre>

Rule to compile HLSL shaders using DirectXShaderCompiler.

The target will output <name>.cso or <name>.spv (when targeting spirv) file with bytecode output.

**ATTRIBUTES**


| Name  | Description | Type | Mandatory | Default |
| :------------- | :------------- | :------------- | :------------- | :------------- |
| <a id="hlsl_shader-name"></a>name |  A unique name for this target.   | <a href="https://bazel.build/concepts/labels#target-names">Name</a> | required |  |
| <a id="hlsl_shader-src"></a>src |  Input HLSL shader source file   | <a href="https://bazel.build/concepts/labels">Label</a> | required |  |
| <a id="hlsl_shader-hdrs"></a>hdrs |  List of header files dependencies to be included in the shader compilation   | <a href="https://bazel.build/concepts/labels">List of labels</a> | optional |  `[]`  |
| <a id="hlsl_shader-asm"></a>asm |  Output assembly code listing file to the specified path (-Fc <file>)   | String | optional |  `""`  |
| <a id="hlsl_shader-def_root_sig"></a>def_root_sig |  Read root signature from a #define (-rootsig-define <value>)   | String | optional |  `""`  |
| <a id="hlsl_shader-defines"></a>defines |  List of macro defines   | List of strings | optional |  `[]`  |
| <a id="hlsl_shader-entry"></a>entry |  Entry point name   | String | optional |  `"main"`  |
| <a id="hlsl_shader-hash"></a>hash |  Output shader hash to the specified file (-Fsh <file>)   | String | optional |  `""`  |
| <a id="hlsl_shader-hlsl"></a>hlsl |  HLSL version to use (2016, 2017, 2018, 2021)   | String | optional |  `""`  |
| <a id="hlsl_shader-includes"></a>includes |  List of directories to be added to the CLI to search for include files   | List of strings | optional |  `[]`  |
| <a id="hlsl_shader-opts"></a>opts |  Additional arguments to pass to the DXC compiler   | List of strings | optional |  `[]`  |
| <a id="hlsl_shader-reflect"></a>reflect |  Output reflection to the specified file (-Fre <file>)   | String | optional |  `""`  |
| <a id="hlsl_shader-spirv"></a>spirv |  Generate SPIR-V code   | Boolean | optional |  `False`  |
| <a id="hlsl_shader-target"></a>target |  Target profile (e.g., cs_6_0, ps_6_0, etc.)   | String | required |  |


<a id="shader_group"></a>

## shader_group

<pre>
load("@rules_vulkan//vulkan:defs.bzl", "shader_group")

shader_group(<a href="#shader_group-name">name</a>, <a href="#shader_group-deps">deps</a>, <a href="#shader_group-pkg_prefix">pkg_prefix</a>)
</pre>

`shader_group` is a rule to group multiple shaders together.

Similar to `filegroup`, but forwards providers to enable building shader libraries and databases. The motivation
for this rule is described in this [issue](https://github.com/bazelbuild/bazel/issues/8904).

The rule expects dependencies from shader compiler targets such as `hlsl_shader`, `glsl_shader`, `slang_shader`,
and `spirv_cross`. Shader groups can also depend on other shader groups, creating hierarchical structures. Each
`shader_group` accumulates ShaderInfo structures from all its dependencies and returns a ShaderGroupInfo provider
containing the collected shader information.

Common use cases include:
- Grouping related shaders together (e.g. vertex + fragment shader pair).
- Building large shader libraries or databases. See the `e2e/smoke` example for implementation details.

**Example:**

```starlark
load("@rules_vulkan//vulkan:defs.bzl", "hlsl_shader", "glsl_shader", "shader_group")

hlsl_shader(
    name = "vertex_shader",
    src = "vertex.hlsl",
    entry = "VSMain",
    target = "vs_6_0",
)

glsl_shader(
    name = "fragment_shader",
    src = "fragment.glsl",
    stage = "frag",
)

shader_group(
    name = "graphics_shaders",
    deps = [
        ":vertex_shader",
        ":fragment_shader",
    ],
    pkg_prefix = "shaders/",
)
```

**Rules_pkg Integration:**

The `shader_group` rule integrates seamlessly with `rules_pkg` for creating shader archives. When `pkg_prefix` is
specified, the rule provides `PackageFilesInfo` that can be consumed directly by `pkg_zip` and other packaging
rules.

The shader group acts as a replacement for `pkg_files`, automatically bundling all shader outputs (compiled
binaries, reflection data, assembly files, etc.) into the specified archive subdirectory. This eliminates the
need to manually specify each shader output file when creating packages.

```starlark
load("@rules_pkg//:pkg.bzl", "pkg_zip")

pkg_zip(
    name = "shader_database",
    srcs = [":graphics_shaders"],  # All shaders will be placed in shaders/ subdirectory
    out = "shaders.zip",
)
```

**ATTRIBUTES**


| Name  | Description | Type | Mandatory | Default |
| :------------- | :------------- | :------------- | :------------- | :------------- |
| <a id="shader_group-name"></a>name |  A unique name for this target.   | <a href="https://bazel.build/concepts/labels#target-names">Name</a> | required |  |
| <a id="shader_group-deps"></a>deps |  List of shader targets to group together.<br><br>Accepts individual shader targets (HLSL, GLSL, Slang, or spirv_cross) and other `shader_group` targets for hierarchical grouping.   | <a href="https://bazel.build/concepts/labels">List of labels</a> | optional |  `[]`  |
| <a id="shader_group-pkg_prefix"></a>pkg_prefix |  If using with `rules_pkg`, sub-directory in the destination archive   | String | optional |  `""`  |


<a id="slang_shader"></a>

## slang_shader

<pre>
load("@rules_vulkan//vulkan:defs.bzl", "slang_shader")

slang_shader(<a href="#slang_shader-name">name</a>, <a href="#slang_shader-srcs">srcs</a>, <a href="#slang_shader-out">out</a>, <a href="#slang_shader-hdrs">hdrs</a>, <a href="#slang_shader-defines">defines</a>, <a href="#slang_shader-depfile">depfile</a>, <a href="#slang_shader-entry">entry</a>, <a href="#slang_shader-includes">includes</a>, <a href="#slang_shader-lang">lang</a>, <a href="#slang_shader-opts">opts</a>, <a href="#slang_shader-profile">profile</a>, <a href="#slang_shader-reflect">reflect</a>,
             <a href="#slang_shader-stage">stage</a>, <a href="#slang_shader-target">target</a>)
</pre>

Rule to compile Slang shaders.

**ATTRIBUTES**


| Name  | Description | Type | Mandatory | Default |
| :------------- | :------------- | :------------- | :------------- | :------------- |
| <a id="slang_shader-name"></a>name |  A unique name for this target.   | <a href="https://bazel.build/concepts/labels#target-names">Name</a> | required |  |
| <a id="slang_shader-srcs"></a>srcs |  Slang input shader files   | <a href="https://bazel.build/concepts/labels">List of labels</a> | required |  |
| <a id="slang_shader-out"></a>out |  Specify a path where generated output should be written (-o <path>)   | String | optional |  `""`  |
| <a id="slang_shader-hdrs"></a>hdrs |  List of header files dependencies to be included in the shader compilation   | <a href="https://bazel.build/concepts/labels">List of labels</a> | optional |  `[]`  |
| <a id="slang_shader-defines"></a>defines |  Insert a preprocessor macro   | List of strings | optional |  `[]`  |
| <a id="slang_shader-depfile"></a>depfile |  Save the source file dependency list in a file (-depfile <name>.dep)   | String | optional |  `""`  |
| <a id="slang_shader-entry"></a>entry |  Entry point name   | String | optional |  `""`  |
| <a id="slang_shader-includes"></a>includes |  Add a path to CLI to be used to search #include or #import operations   | List of strings | optional |  `[]`  |
| <a id="slang_shader-lang"></a>lang |  Set source language for the shader (slang, hlsl, glsl, cpp, etc)   | String | optional |  `""`  |
| <a id="slang_shader-opts"></a>opts |  Additional arguments to pass to the compiler   | List of strings | optional |  `[]`  |
| <a id="slang_shader-profile"></a>profile |  Shader profile for code generation (sm_6_6, vs_6_6, glsl_460, etc)   | String | required |  |
| <a id="slang_shader-reflect"></a>reflect |  Emit reflection data in JSON format to a file   | String | optional |  `""`  |
| <a id="slang_shader-stage"></a>stage |  Stage of an entry point function (vertex, pixel, compute, etc)   | String | optional |  `""`  |
| <a id="slang_shader-target"></a>target |  Format in which code should be generated (hlsl, dxil, dxil-asm, glsl, spirv, metal, metallib, etc)   | String | required |  |


<a id="spirv_cross"></a>

## spirv_cross

<pre>
load("@rules_vulkan//vulkan:defs.bzl", "spirv_cross")

spirv_cross(<a href="#spirv_cross-name">name</a>, <a href="#spirv_cross-src">src</a>, <a href="#spirv_cross-out">out</a>, <a href="#spirv_cross-backend">backend</a>, <a href="#spirv_cross-entry">entry</a>, <a href="#spirv_cross-opts">opts</a>, <a href="#spirv_cross-stage">stage</a>)
</pre>

spirv-cross build target.

This rule allows invoking spirv-cross binary bundled with Vulkan SDK.

Additionally, it integrates with compiler rules (hlsl_shader, glsl_shader, etc) and can inherit entry point
and stages from `CompilerInfo`.

**ATTRIBUTES**


| Name  | Description | Type | Mandatory | Default |
| :------------- | :------------- | :------------- | :------------- | :------------- |
| <a id="spirv_cross-name"></a>name |  A unique name for this target.   | <a href="https://bazel.build/concepts/labels#target-names">Name</a> | required |  |
| <a id="spirv_cross-src"></a>src |  Input file   | <a href="https://bazel.build/concepts/labels">Label</a> | required |  |
| <a id="spirv_cross-out"></a>out |  Output artifact name. If not specified, will default to <name>.out   | String | optional |  `""`  |
| <a id="spirv_cross-backend"></a>backend |  Select backend for spirv-cross   | String | optional |  `""`  |
| <a id="spirv_cross-entry"></a>entry |  Use a specific entry point   | String | optional |  `""`  |
| <a id="spirv_cross-opts"></a>opts |  Additional arguments to pass to the spirv-cross binary   | List of strings | optional |  `[]`  |
| <a id="spirv_cross-stage"></a>stage |  Forces use of a certain shader stage   | String | optional |  `""`  |


<a id="vulkan_toolchain"></a>

## vulkan_toolchain

<pre>
load("@rules_vulkan//vulkan:defs.bzl", "vulkan_toolchain")

vulkan_toolchain(<a href="#vulkan_toolchain-name">name</a>, <a href="#vulkan_toolchain-dxc">dxc</a>, <a href="#vulkan_toolchain-env">env</a>, <a href="#vulkan_toolchain-glslc">glslc</a>, <a href="#vulkan_toolchain-slangc">slangc</a>, <a href="#vulkan_toolchain-spirv_cross">spirv_cross</a>)
</pre>



**ATTRIBUTES**


| Name  | Description | Type | Mandatory | Default |
| :------------- | :------------- | :------------- | :------------- | :------------- |
| <a id="vulkan_toolchain-name"></a>name |  A unique name for this target.   | <a href="https://bazel.build/concepts/labels#target-names">Name</a> | required |  |
| <a id="vulkan_toolchain-dxc"></a>dxc |  Path to dxc   | <a href="https://bazel.build/concepts/labels">Label</a> | required |  |
| <a id="vulkan_toolchain-env"></a>env |  Environment to be passed to executables.   | <a href="https://bazel.build/rules/lib/dict">Dictionary: String -> String</a> | optional |  `{}`  |
| <a id="vulkan_toolchain-glslc"></a>glslc |  Path to glslc   | <a href="https://bazel.build/concepts/labels">Label</a> | required |  |
| <a id="vulkan_toolchain-slangc"></a>slangc |  Path to slangc   | <a href="https://bazel.build/concepts/labels">Label</a> | required |  |
| <a id="vulkan_toolchain-spirv_cross"></a>spirv_cross |  Path to spirv_cross   | <a href="https://bazel.build/concepts/labels">Label</a> | required |  |


<a id="ShaderGroupInfo"></a>

## ShaderGroupInfo

<pre>
load("@rules_vulkan//vulkan:defs.bzl", "ShaderGroupInfo")

ShaderGroupInfo(<a href="#ShaderGroupInfo-list">list</a>)
</pre>

A collection of shader infos.

**FIELDS**

| Name  | Description |
| :------------- | :------------- |
| <a id="ShaderGroupInfo-list"></a>list |  List of ShaderInfo structures    |


<a id="ShaderInfo"></a>

## ShaderInfo

<pre>
load("@rules_vulkan//vulkan:defs.bzl", "ShaderInfo")

ShaderInfo(<a href="#ShaderInfo-binary">binary</a>, <a href="#ShaderInfo-assembly">assembly</a>, <a href="#ShaderInfo-reflection">reflection</a>, <a href="#ShaderInfo-hash">hash</a>, <a href="#ShaderInfo-depfile">depfile</a>, <a href="#ShaderInfo-entry">entry</a>, <a href="#ShaderInfo-stage">stage</a>, <a href="#ShaderInfo-defines">defines</a>, <a href="#ShaderInfo-target">target</a>)
</pre>

Shader metadata returned by the shader targets during compilation.

This is useful for building all kind of shader databases.

**FIELDS**

| Name  | Description |
| :------------- | :------------- |
| <a id="ShaderInfo-binary"></a>binary |  Binary output file containing the compiled shader bytecode    |
| <a id="ShaderInfo-assembly"></a>assembly |  Assembly output file (if generated, HLSL-specific)    |
| <a id="ShaderInfo-reflection"></a>reflection |  Reflection output file (if generated)    |
| <a id="ShaderInfo-hash"></a>hash |  Hash output file (if generated, HLSL-specific)    |
| <a id="ShaderInfo-depfile"></a>depfile |  Dependency output file (if generated, Slang-specific)    |
| <a id="ShaderInfo-entry"></a>entry |  Shader entry point function name    |
| <a id="ShaderInfo-stage"></a>stage |  Shader stage    |
| <a id="ShaderInfo-defines"></a>defines |  List of shader defines used during compilation    |
| <a id="ShaderInfo-target"></a>target |  Compilation target (note: this depends on compiler used)    |


<a id="download_sdk"></a>

## download_sdk

<pre>
load("@rules_vulkan//vulkan:defs.bzl", "download_sdk")

download_sdk(<a href="#download_sdk-name">name</a>, <a href="#download_sdk-build_file">build_file</a>, <a href="#download_sdk-repo_mapping">repo_mapping</a>, <a href="#download_sdk-urls">urls</a>, <a href="#download_sdk-version">version</a>, <a href="#download_sdk-windows_skip_runtime">windows_skip_runtime</a>)
</pre>

A rule to handle download and unpack of the SDK for each major platform (Windows, Linux, MacOS).

These rely on command line installation described in "Getting started" docs on LunarG.
- https://vulkan.lunarg.com/doc/view/1.3.283.0/mac/getting_started.html

**ATTRIBUTES**


| Name  | Description | Type | Mandatory | Default |
| :------------- | :------------- | :------------- | :------------- | :------------- |
| <a id="download_sdk-name"></a>name |  A unique name for this repository.   | <a href="https://bazel.build/concepts/labels#target-names">Name</a> | required |  |
| <a id="download_sdk-build_file"></a>build_file |  -   | <a href="https://bazel.build/concepts/labels">Label</a> | optional |  `"@rules_vulkan//vulkan/private:template.BUILD"`  |
| <a id="download_sdk-repo_mapping"></a>repo_mapping |  In `WORKSPACE` context only: a dictionary from local repository name to global repository name. This allows controls over workspace dependency resolution for dependencies of this repository.<br><br>For example, an entry `"@foo": "@bar"` declares that, for any time this repository depends on `@foo` (such as a dependency on `@foo//some:target`, it should actually resolve that dependency within globally-declared `@bar` (`@bar//some:target`).<br><br>This attribute is _not_ supported in `MODULE.bazel` context (when invoking a repository rule inside a module extension's implementation function).   | <a href="https://bazel.build/rules/lib/dict">Dictionary: String -> String</a> | optional |  |
| <a id="download_sdk-urls"></a>urls |  URLs and SHA256 checksum for each platform.<br><br>Refer to the module extension doc for specifics.   | <a href="https://bazel.build/rules/lib/dict">Dictionary: String -> String</a> | required |  |
| <a id="download_sdk-version"></a>version |  Vulkan SDK version to download and install.<br><br>This expects a version in the format of `1.4.313.0` or `1.4.313`. When 3 components are provided, `.0` will be appended automatically to make it 4 components.   | String | required |  |
| <a id="download_sdk-windows_skip_runtime"></a>windows_skip_runtime |  Do not download and install Vulkan runtime package (e.g. `vulkan-1.dll` dependency) on Windows.<br><br>When `True`, the downloader will put `vulkan-1.dll` into the repository root directory.<br><br>This is useful if there is a system-wide Vulkan runtime already installed, otherwise this might lead to link/runtime issues when building CC targets.   | Boolean | optional |  `False`  |


