echo '{"name": "levelDB3","description": "NoSQL database"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/skills/categories/software

