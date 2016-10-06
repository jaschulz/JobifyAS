import unittest
from PostTestCases import PostTest

if __name__ == '__main__':
	suite = unittest.TestSuite()

	tests = unittest.TestLoader().loadTestsFromTestCase(PostTest)
	suite.addTests(tests)

unittest.TextTestRunner().run(suite)
