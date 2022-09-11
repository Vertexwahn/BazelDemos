from selenium import webdriver
from bs4 import BeautifulSoup
url = 'https://www.google.de'
options = webdriver.FirefoxOptions()
options.add_argument('--ignore-certificate-errors')
options.add_argument('--incognito')
options.add_argument('--headless')
driver = webdriver.Firefox(options=options)
driver.implicitly_wait(60)
driver.get(url)
soup = BeautifulSoup(driver.page_source, 'lxml')
driver.quit()