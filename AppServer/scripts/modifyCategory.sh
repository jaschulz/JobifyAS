echo '{"name": "HR","description": "It includes positions related to Human Resources."}' | \
curl -X PUT -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/categories/HR
