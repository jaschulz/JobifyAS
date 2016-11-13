echo '{"name": "HR","description": "It includes every position related to HR."}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/categories

