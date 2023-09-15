import unittest
from testing.web import webtest

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

if __name__ == "__main__":
  unittest.main()
