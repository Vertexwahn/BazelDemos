import okapi.python.okapi

d = okapi.python.okapi.ConstantSpectrum(1.4)
print("Result: " + str(d.evaluate(400)))
