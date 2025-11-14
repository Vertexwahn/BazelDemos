import unittest
from testing.web import webtest

from selenium import webdriver
from selenium.webdriver.common.keys import Keys

from PIL import Image
import imagehash

def compare_pixels(png1, png2):
  pixels1 = list(png1.getdata())
  pixels2 = list(png2.getdata())
  if len(pixels1) != len(pixels2):
    return -1 # different pixel count
  match = 0
  mismatch = 0
  for i in range(0, len(pixels1)):
    if pixels1[i] == pixels2[i]:
      match = match + 1
    else:
      mismatch = mismatch + 1
  return match/(match+mismatch)

class BrowserTest(unittest.TestCase):
  def setUp(self):
    self.driver = webtest.new_webdriver_session()

  def tearDown(self):
    try:
      self.driver.quit()
    finally:
      self.driver = None

  def testTitle(self):
    self.driver.get("https://www.python.org")
    self.assertEqual("Welcome to Python.org", self.driver.title)

  def testScreenshot2(self):
    self.driver.set_window_size(1920, 1080)
    self.driver.get("http://localhost:4059/static/index2.html")
    self.driver.get_screenshot_as_file("/home/vertexwahn/Desktop/reference_screenshot2.png")
    # do bazel run //:browser_test # does only generate a black image since the browser seems not support webgpu... 
    self.driver.get("http://localhost:4059/static/index2.html")
    self.driver.get_screenshot_as_file("screenshot2.png")
    
    png1 = Image.open("ref_screenshot2.png")
    png2 = Image.open("screenshot2.png")
    self.assertEqual(compare_pixels(png1, png2), 1)


if __name__ == "__main__":
  
  unittest.main()
