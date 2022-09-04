def _impl(ctx):
    print("muh")   

my_rule = rule(
     implementation=_impl,
)
