import unittest
from testing.web import webtest

from selenium import webdriver
from selenium.webdriver.common.keys import Keys

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

  def testScreenshot(self):
    self.driver.set_window_size(1920, 1080)
    self.driver.get("https://www.python.org")
    self.driver.get_screenshot_as_file("screenshot.png")  

if __name__ == "__main__":
  unittest.main()