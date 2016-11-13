echo '' | \
curl -X DELETE -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/categories/health

