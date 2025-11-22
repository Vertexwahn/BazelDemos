import mitsuba as mi

mi.set_variant('scalar_rgb')

scene = mi.load_file("/Users/vertexwahn/dev/Piper/devertexwahn/okapi/scenes/cornell_box/cornell_box.aov.integrator.spp1.mitsuba.xml")

ssp = 1

img = mi.render(scene, spp=ssp)

mi.Bitmap(img).write("cornell_box.aov.integrator.spp" + str(ssp) + ".aov.mitsuba.exr")
