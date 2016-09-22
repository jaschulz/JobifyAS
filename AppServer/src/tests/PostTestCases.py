import requests
import unittest
import json
import uuid
import ConfigParser
import datetime

class PostTest(unittest.TestCase):
	def __init__(self, *args, **kwargs):
		super(PostTest, self).__init__(*args, **kwargs)
		self.__api_base_url = "http://localhost:8081/"
		self._url_registerUser = "api/users"

	#Successful signup
	def test_1(self):
		payload = {'email' : 'JohnDoe'+datetime.datetime.now().strftime("%Y%m%d%H%M%S")+'@gmail.com','password' : 'Berlin'}
		r = requests.post(self.__api_base_url + self._url_registerUser, json=payload)
		self.assertEqual(r.status_code, 200)
		data = json.loads(r.text)
		self.assertEqual(data["token"], "ok")

