echo '{"fbid": "10207339104015450","token": "EAACEdEose0cBACgFldBU7O9ZCYaBPLErqvZC08l76uU8Eelj1xJLdfo3aiuP8h3ZAxEmH6usD8aHOxZBdxzfDU0hYjOoBvwKIgIUs9IlAVcskQtvvIp3Skg9ix35NVrQNlAM5FDLCIZCyCZCjcUsZCcxWSZChpoqK3Qn0o8KHhJE3wZDZD"}' | \
curl -X GET -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/fbdata

