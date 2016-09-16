import requests
import unittest
import json
import uuid
import ConfigParser

class PostTest(unittest.TestCase):
	def __init__(self, *args, **kwargs):
		super(PostTest, self).__init__(*args, **kwargs)
		self.__api_base_url = "http://localhost:8081/"
		self._url_login = "api/login"

	#Successful signup
	def test_1(self):
		payload = {"email" : "John@gmail.com",
			"password" : "Berlin"}
		r = requests.get(self.__api_base_url + self._url_login, json=payload)
		self.assertEqual(r.status_code, 200)
		data = json.loads(r.text)
		self.assertEqual(data["result"], "login Ok")
