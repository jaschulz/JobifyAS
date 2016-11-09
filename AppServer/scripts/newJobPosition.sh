echo '{"name": "Teacher of Literature","description": "a Teacher of Literature"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/job_positions/categories/education

