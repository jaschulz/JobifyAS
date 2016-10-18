echo '{"name": "Trainee Node developer3","description": "a Trainee Node developer3"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/job_positions/categories/software

